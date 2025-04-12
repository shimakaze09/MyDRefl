//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <array>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Vec {
  float x;
  float y;

  Vec& operator+=(const Vec& p) {
    std::cout << "Vec& operator+=(const Vec&)" << std::endl;
    x += p.x;
    y += p.y;
    return *this;
  }

  Vec& operator+=(float d) {
    std::cout << "Vec& operator+=(float)" << std::endl;
    x += d;
    y += d;
    return *this;
  }
};

int main() {
  {  // register Vec
    ReflMngr::Instance().RegisterType<Vec>();
    ReflMngr::Instance().AddField<&Vec::x>("x");
    ReflMngr::Instance().AddField<&Vec::y>("y");
    ReflMngr::Instance()
        .AddMethod<MemFuncOf<Vec&(const Vec&)>::get(&Vec::operator+=)>(
            StrIDRegistry::Meta::operator_assign_add);
    ReflMngr::Instance()
        .AddMethod<MemFuncOf<Vec&(float)>::get(&Vec::operator+=)>(
            StrIDRegistry::Meta::operator_assign_add);
  }

  auto obj = ReflMngr::Instance().MakeShared(TypeID_of<Vec>);

  {
    auto v = obj += Vec{10.f, 10.f};
    std::cout << v->Var("x") << ", " << v->Var("y") << std::endl;
  }

  {
    auto v = obj += 2.f;
    std::cout << v->Var("x") << ", " << v->Var("y") << std::endl;
  }

  return 0;
}
