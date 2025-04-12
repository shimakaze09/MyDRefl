//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <array>
#include <cmath>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Vec {
  float x;
  float y;

  float Norm2() const noexcept { return x * x + y * y; }

  void NormalizeSelf() noexcept {
    float n = std::sqrt(Norm2());
    assert(n != 0);
    x /= n;
    y /= n;
  }

  Vec& operator+=(const Vec& p) noexcept {
    x += p.x;
    y += p.y;
    return *this;
  }
};

int main() {
  {  // register Vec
    ReflMngr::Instance().RegisterType<Vec>();
    ReflMngr::Instance().AddConstructor<Vec, float, float>();
    ReflMngr::Instance().AddField<&Vec::x>("x");
    ReflMngr::Instance().AddField<&Vec::y>("y");
    ReflMngr::Instance().AddMethod<&Vec::Norm2>("Norm2");
    ReflMngr::Instance().AddMethod<&Vec::NormalizeSelf>("NormalizeSelf");
    ReflMngr::Instance().AddMethod<&Vec::operator+= >(
        StrIDRegistry::Meta::operator_assign_add);
  }

  auto v = ReflMngr::Instance().MakeShared(TypeID_of<Vec>, 1.f, 2.f);

  v->Invoke("NormalizeSelf");
  std::cout << v->Var("x") << ", " << v->Var("y") << std::endl;

  std::cout << v->DMInvoke("Norm2") << std::endl;

  auto w = v += Vec{10.f, 10.f};
  std::cout << w->Var("x") << ", " << w->Var("y") << std::endl;

  return 0;
}
