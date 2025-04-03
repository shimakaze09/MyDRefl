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
  auto ID_Point = ReflMngr::Instance().tregistry.Register<Point>();
  auto ID_float = ReflMngr::Instance().tregistry.Register<float>();
  auto ID_MyInspector_Range =
      ReflMngr::Instance().tregistry.Register<MyInspector::Range>();
  auto ID_MyInspector_A =
      ReflMngr::Instance().tregistry.Register<MyInspector::A>();

  auto ID_value = ReflMngr::Instance().nregistry.Register("value");
  auto ID_min_value = ReflMngr::Instance().nregistry.Register("min_value");
  auto ID_max_value = ReflMngr::Instance().nregistry.Register("max_value");

  {  // register MyInspector::Range
    TypeInfo typeinfo{
        sizeof(MyInspector::Range),
        alignof(MyInspector::Range),
        {
            // fields
            ReflMngr::Instance().GenerateField<&MyInspector::Range::min_value>(
                "min_value"),
            ReflMngr::Instance().GenerateField<&MyInspector::Range::max_value>(
                "max_value"),
        },
        {// methods
         ReflMngr::Instance().GenerateConstructor<MyInspector::Range>(),
         ReflMngr::Instance()
             .GenerateConstructor<MyInspector::Range, float, float>(),
         ReflMngr::Instance().GenerateDestructor<MyInspector::Range>()}};
    ReflMngr::Instance().typeinfos.emplace(ID_MyInspector_Range,
                                           std::move(typeinfo));
  }

  {  // register MyInspector::A
    TypeInfo typeinfo{
        sizeof(MyInspector::A),
        alignof(MyInspector::A),
        {},  // fields
        {    // methods
         ReflMngr::Instance().GenerateConstructor<MyInspector::A>(),
         ReflMngr::Instance().GenerateDestructor<MyInspector::A>()}};
    ReflMngr::Instance().typeinfos.emplace(ID_MyInspector_A,
                                           std::move(typeinfo));
  }
  {  // register Point
    TypeInfo typeinfo{
        sizeof(Point),
        alignof(Point),
        {// fields
         ReflMngr::Instance().GenerateField<&Point::x>(
             "x",
             {ReflMngr::Instance().MakeShared<MyInspector::Range>(1.f, 2.f)}),
         ReflMngr::Instance().GenerateField<&Point::y>(
             "y", {ReflMngr::Instance().MakeShared<MyInspector::A>()})}};
    ReflMngr::Instance().typeinfos.emplace(ID_Point, std::move(typeinfo));
  }

  Point p;
  ObjectPtr ptr{ID_Point, &p};
  ReflMngr::Instance().RWVar(ptr, StrID{"x"}).As<float>() = 1.f;
  ReflMngr::Instance().RWVar(ptr, StrID{"y"}).As<float>() = 2.f;

  ReflMngr::Instance().ForEachRVar(ptr, [](Type type, Field field,
                                           ConstObjectPtr var) {
    for (const auto& attr : field.info.attrs) {
      std::cout << "[" << ReflMngr::Instance().tregistry.Nameof(attr.GetID())
                << "]" << std::endl;
      ReflMngr::Instance().ForEachRVar(
          attr, [](Type type, Field field, ConstObjectPtr var) {
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
