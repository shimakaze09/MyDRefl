#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

namespace UInspector {
struct Range {
  float min_value;
  float max_value;
};

class A {
 public:
  ~A() { std::cout << "delete A" << std::endl; }
};
}  // namespace UInspector

struct Point {
  [[UInspector::Range(1.f, 2.f)]]
  float x;
  [[UInspector::A]]
  float y;
};

int main() {
  {  // register
    Mngr->RegisterType<UInspector::Range>();
    Mngr->AddField<&UInspector::Range::min_value>("min_value");
    Mngr->AddField<&UInspector::Range::max_value>("max_value");
    Mngr->AddConstructor<UInspector::Range, float, float>();

    Mngr->RegisterType<UInspector::A>();

    Mngr->RegisterType<Point>();
    Mngr->AddConstructor<Point, float, float>();
    Mngr->AddField<&Point::x>(
        "x", {Mngr->MakeShared(Type_of<UInspector::Range>, 1.f, 2.f)});
    Mngr->AddField<&Point::y>("y", {Mngr->MakeShared(Type_of<UInspector::A>)});
  }

  auto p = Mngr->MakeShared(Type_of<Point>, 1.f, 2.f);

  p.Var("x") += 1.f;
  p.Var("y") += 2.f;

  for (const auto& [type, field, var] : p.GetTypeFieldVars()) {
    for (const auto& attr : field.info->attrs) {
      std::cout << "[" << attr.GetType().GetName() << "]" << std::endl;
      for (const auto& [type, field, var] : attr.GetTypeFieldVars()) {
        std::cout << field.name.GetView() << ": " << var << std::endl;
      }
      std::cout << "------" << std::endl;
    }
    std::cout << field.name.GetView() << ": " << var << std::endl;
  }
}
