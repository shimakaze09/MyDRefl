//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My::MyDRefl;

struct Point {
  float x;
  float y;
};

int main() {
  auto ID_Point = ReflMngr::Instance().tregistry.Register("Point");
  auto ID_float = ReflMngr::Instance().tregistry.Register("float");

  auto ID_x = ReflMngr::Instance().nregistry.Register("x");
  auto ID_y = ReflMngr::Instance().nregistry.Register("y");

  auto ID_ctor = ReflMngr::Instance().nregistry.GetID(NameRegistry::Meta::ctor);
  auto ID_dtor = ReflMngr::Instance().nregistry.GetID(NameRegistry::Meta::dtor);

  {  // register Point
    TypeInfo typeinfo{sizeof(Point),
                      alignof(Point),
                      {// fieldinfos
                       {ID_x, {{ID_float, offsetof(Point, x)}}},
                       {ID_y, {{ID_float, offsetof(Point, y)}}}},
                      {// methods
                       {ID_ctor, {Method::GenerateDefaultConstructor<Point>()}},
                       {ID_dtor, {Method::GenerateDestructor<Point>()}}}};
    ReflMngr::Instance().typeinfos.emplace(ID_Point, std::move(typeinfo));
  }

  ObjectPtr p = ReflMngr::Instance().New(ID_Point);
  ReflMngr::Instance().RWField(p, ID_x).As<float>() = 1.f;
  ReflMngr::Instance().RWField(p, ID_y).As<float>() = 2.f;

  ReflMngr::Instance().ForEachRField(
      p, [](TypeFieldInfo info, ConstObjectPtr field) {
        std::cout << ReflMngr::Instance().nregistry.Nameof(info.fieldID) << ": "
                  << field.As<float>() << std::endl;
      });
  ReflMngr::Instance().Delete(p);
}
