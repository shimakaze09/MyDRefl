//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <array>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct A {
  float a{0.f};
};

struct B : virtual A {
  float b{0.f};
};

struct C : virtual A {
  float c{0.f};
};

struct D : B, C {
  float d{0.f};
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

  d->RWVar("a") = 1.f;
  d->RWVar("b") = 3.f;
  d->RWVar("c") = 4.f;
  d->RWVar("d") = 5.f;

  for (const auto& [type, field, var] : d->GetTypeFieldRVars()) {
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": " << var
              << std::endl;
  }

  return 0;
}
