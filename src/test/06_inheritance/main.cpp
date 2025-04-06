//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <array>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct A {
  float a;
};

struct B : A {
  float b;
};

struct C : A {
  float c;
};

struct D : B, C {
  float d;
};

int main() {
  {  // register
    ReflMngr::Instance().RegisterTypeAuto<A>();
    ReflMngr::Instance().AddField<&A::a>("a");

    ReflMngr::Instance().RegisterTypeAuto<B>();
    ReflMngr::Instance().AddBases<B, A>();
    ReflMngr::Instance().AddField<&B::b>("b");

    ReflMngr::Instance().RegisterTypeAuto<C>();
    ReflMngr::Instance().AddBases<C, A>();
    ReflMngr::Instance().AddField<&C::c>("c");

    ReflMngr::Instance().RegisterTypeAuto<D>();
    ReflMngr::Instance().AddBases<D, B, C>();
    ReflMngr::Instance().AddField<&D::d>("d");
  }

  auto d = ReflMngr::Instance().MakeShared(TypeID_of<D>);

  ReflMngr::Instance().RWVar(d, TypeID_of<B>, StrID{"a"}).As<float>() = 1.f;
  ReflMngr::Instance().RWVar(d, TypeID_of<C>, StrID{"a"}).As<float>() = 2.f;
  ReflMngr::Instance().RWVar(d, StrID{"b"}).As<float>() = 3.f;
  ReflMngr::Instance().RWVar(d, StrID{"c"}).As<float>() = 4.f;
  ReflMngr::Instance().RWVar(d, StrID{"d"}).As<float>() = 5.f;

  d->ForEachRVar([](TypeRef type, FieldRef field, ConstObjectPtr var) {
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
              << var.As<float>() << std::endl;
    return true;
  });

  return 0;
}
