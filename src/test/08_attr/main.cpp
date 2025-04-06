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
    Mngr->RegisterTypeAuto<MyInspector::Range>();
    Mngr->AddField<&MyInspector::Range::min_value>("min_value");
    Mngr->AddField<&MyInspector::Range::max_value>("max_value");
    Mngr->AddConstructor<MyInspector::Range, float, float>();

    Mngr->RegisterTypeAuto<MyInspector::A>();

    Mngr->RegisterTypeAuto<Point>();
    Mngr->AddConstructor<Point, float, float>();
    Mngr->AddField<&Point::x>(
        "x", {Mngr->MakeShared(TypeID_of<MyInspector::Range>, 1.f, 2.f)});
    Mngr->AddField<&Point::y>("y",
                              {Mngr->MakeShared(TypeID_of<MyInspector::A>)});
  }

  auto p = Mngr->MakeShared(TypeID_of<Point>, 1.f, 2.f);

  Mngr->RWVar(p, StrID{"x"}).As<float>() += 1.f;
  Mngr->RWVar(p, StrID{"y"}).As<float>() += 2.f;

  p->ForEachRVar([](TypeRef type, FieldRef field, ConstObjectPtr var) {
    for (const auto& attr : field.info.attrs) {
      std::cout << "[" << Mngr->tregistry.Nameof(attr.GetID()) << "]"
                << std::endl;
      attr->ForEachRVar([](TypeRef type, FieldRef field, ConstObjectPtr var) {
        std::cout << Mngr->nregistry.Nameof(field.ID) << ": " << var.As<float>()
                  << std::endl;
        return true;
      });
      std::cout << "------" << std::endl;
    }
    std::cout << Mngr->nregistry.Nameof(field.ID) << ": " << var.As<float>()
              << std::endl;
    return true;
  });
}
