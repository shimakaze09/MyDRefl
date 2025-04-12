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
    ReflMngr::Instance().RegisterType<A>();
    ReflMngr::Instance().AddField<&A::a>("a");

    ReflMngr::Instance().RegisterType<B>();
    ReflMngr::Instance().AddBases<B, A>();
    ReflMngr::Instance().AddField<&B::b>("b");

    ReflMngr::Instance().RegisterType<C>();
    ReflMngr::Instance().AddBases<C, A>();
    ReflMngr::Instance().AddField<&C::c>("c");

    ReflMngr::Instance().RegisterType<D>();
    ReflMngr::Instance().AddBases<D, B, C>();
    ReflMngr::Instance().AddField<&D::d>("d");
  }

  auto d = ReflMngr::Instance().MakeShared(TypeID_of<D>);
  d->Var(TypeID_of<B>, "a") = 1.f;
  d->Var(TypeID_of<C>, "a") = 2.f;
  d->Var("b") = 3.f;
  d->Var("c") = 4.f;
  d->Var("d") = 5.f;

  for (const auto& [type, field, var] : d->GetTypeFieldVars()) {
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": " << var
              << std::endl;
  }

  return 0;
}
