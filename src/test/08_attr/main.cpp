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
  [[MyInspector::range(1.f, 2.f)]]
  float x;
  [[MyInspector::A]]
  float y;
};

int main() {
  {  // register
    ReflMngr::Instance().RegisterTypeAuto<MyInspector::Range>();
    ReflMngr::Instance().AddField<&MyInspector::Range::min_value>("min_value");
    ReflMngr::Instance().AddField<&MyInspector::Range::max_value>("max_value");
    ReflMngr::Instance().AddConstructor<MyInspector::Range, float, float>();

    ReflMngr::Instance().RegisterTypeAuto<MyInspector::A>();

    ReflMngr::Instance().RegisterTypeAuto<Point>();
    ReflMngr::Instance().AddConstructor<Point, float, float>();
    ReflMngr::Instance().AddField<&Point::x>("x");
    ReflMngr::Instance().AddField<&Point::y>("y");
  }

  auto p = ReflMngr::Instance().MakeShared(TypeID::of<Point>, 1.f, 2.f);

  ReflMngr::Instance().RWVar(p, StrID{"x"}).As<float>() += 1.f;
  ReflMngr::Instance().RWVar(p, StrID{"y"}).As<float>() += 2.f;

  ReflMngr::Instance().ForEachRVar(p, [](TypeRef type, FieldRef field,
                                         ConstObjectPtr var) {
    for (const auto& attr : field.info.attrs) {
      std::cout << "[" << ReflMngr::Instance().tregistry.Nameof(attr.GetID())
                << "]" << std::endl;
      ReflMngr::Instance().ForEachRVar(
          attr, [](TypeRef type, FieldRef field, ConstObjectPtr var) {
            std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
                      << var.As<float>() << std::endl;
            return true;
          });
      std::cout << "------" << std::endl;
    }
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
              << var.As<float>() << std::endl;
    return true;
  });
}
