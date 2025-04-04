//
// Created by Admin on 11/03/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Point {
  float x, y;
};

int main() {
  ReflMngr::Instance().RegisterTypeAuto<Point>();
  ReflMngr::Instance().AddField<&Point::x>("x");
  ReflMngr::Instance().AddField<&Point::x>("y");

  auto p = ReflMngr::Instance().MakeShared<Point>();

  ReflMngr::Instance().RWVar(p, StrID{"x"}).As<float>() = 1.f;
  ReflMngr::Instance().RWVar(p, StrID{"y"}).As<float>() = 2.f;

  p->ForEachRVar([](TypeRef type, FieldRef field, ConstObjectPtr var) {
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
              << var.As<float>() << std::endl;
    return true;
  });
}
