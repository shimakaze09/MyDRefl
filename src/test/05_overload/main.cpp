#include <MyDRefl/MyDRefl.h>

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
    Mngr.RegisterType<Vec>();
    Mngr.AddField<&Vec::x>("x");
    Mngr.AddField<&Vec::y>("y");
    Mngr.AddMethod<MemFuncOf<Vec&(const Vec&)>::get(&Vec::operator+=)>(
        NameIDRegistry::Meta::operator_assign_add);
    Mngr.AddMethod<MemFuncOf<Vec&(float)>::get(&Vec::operator+=)>(
        NameIDRegistry::Meta::operator_assign_add);
  }

  auto obj = Mngr.MakeShared(Type_of<Vec>);

  {
    auto v = obj += Vec{10.f, 10.f};
    std::cout << v.Var("x") << ", " << v.Var("y") << std::endl;
  }

  {
    auto v = obj += 2.f;
    std::cout << v.Var("x") << ", " << v.Var("y") << std::endl;
  }

  return 0;
}
