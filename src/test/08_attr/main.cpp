//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

namespace MyInspector {
struct Range {
  float min_value;
  float max_value;
};

class A {
 public:
  ~A() { std::cout << "delete A" << std::endl; }
};
}  // namespace MyInspector

struct Point {
  [[MyInspector::Range(1.f, 2.f)]]
  float x;
  [[MyInspector::A]]
  float y;
};

int main() {
  {  // register
    Mngr->RegisterType<MyInspector::Range>();
    Mngr->AddField<&MyInspector::Range::min_value>("min_value");
    Mngr->AddField<&MyInspector::Range::max_value>("max_value");
    Mngr->AddConstructor<MyInspector::Range, float, float>();

    Mngr->RegisterType<MyInspector::A>();

    Mngr->RegisterType<Point>();
    Mngr->AddConstructor<Point, float, float>();
    Mngr->AddField<&Point::x>(
        "x", {Mngr->MakeShared(TypeID_of<MyInspector::Range>, 1.f, 2.f)});
    Mngr->AddField<&Point::y>("y",
                              {Mngr->MakeShared(TypeID_of<MyInspector::A>)});
  }

  auto p = Mngr->MakeShared(TypeID_of<Point>, 1.f, 2.f);

  p->Var("x") += 1.f;
  p->Var("y") += 2.f;

  for (const auto& [type, field, var] : p->GetTypeFieldVars()) {
    for (const auto& attr : field.info.attrs) {
      std::cout << "[" << Mngr->tregistry.Nameof(attr.GetTypeID()) << "]"
                << std::endl;
      for (const auto& [type, field, var] : attr->GetTypeFieldVars()) {
        std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
                  << var << std::endl;
      }
      std::cout << "------" << std::endl;
    }
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": " << var
              << std::endl;
  }
}
