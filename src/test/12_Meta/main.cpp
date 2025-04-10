//
// Created by Admin on 4/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Vec {
  float x;
  float y;

  Vec operator+(float k) const noexcept {
    Vec rst;
    rst.x = x + k;
    rst.y = y + k;
    return rst;
  }

  Vec operator+(const Vec& v) const noexcept {
    Vec rst;
    rst.x = x + v.x;
    rst.y = y + v.y;
    return rst;
  }

  Vec operator-(const Vec& v) const noexcept {
    Vec rst;
    rst.x = x - v.x;
    rst.y = y - v.y;
    return rst;
  }

  Vec operator*(const Vec& v) const noexcept {
    Vec rst;
    rst.x = x * v.x;
    rst.y = y * v.y;
    return rst;
  }

  Vec operator/(const Vec& v) const noexcept {
    Vec rst;
    rst.x = x / v.x;
    rst.y = y / v.y;
    return rst;
  }

  Vec& operator+=(const Vec& v) noexcept {
    x += v.x;
    y += v.y;
    return *this;
  }

  float operator[](std::size_t v) const noexcept {
    if (v == 0)
      return x;
    else if (v == 1)
      return y;
    else
      return 0.f;
  }
};

int main() {
  ReflMngr::Instance().RegisterType<Vec>();
  ReflMngr::Instance().AddConstructor<Vec, float, float>();
  ReflMngr::Instance().AddField<&Vec::x>("x");
  ReflMngr::Instance().AddField<&Vec::y>("y");
  ReflMngr::Instance()
      .AddMethod<MemFuncOf<Vec(float) const noexcept>::get(&Vec::operator+)>(
          StrIDRegistry::Meta::operator_add);

  SharedObject v = ReflMngr::Instance().MakeShared(TypeID_of<Vec>, 3.f, 4.f);

  ObjectPtr pv = v;
  SharedObject w0 = v + v;
  SharedObject w1 = v + pv;
  SharedObject w2 = v + 1.f;
  SharedObject w3 = v - pv;
  SharedObject w4 = v * pv;
  SharedObject w5 = v / pv;
  SharedObject w6 = v + Vec{1.f, 2.f};

  for (const auto& w : std::array{w0, w1, w2, w3, w4, w5, w6}) {
    for (const auto& [type, field, var] : w->GetTypeFieldRVars()) {
      std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
                << var << std::endl;
    }
  }

  SharedObject ele_1 = (v += Vec{10.f, 10.f})[static_cast<std::size_t>(1)];
  std::cout << "ele_1: " << ele_1 << std::endl;
}
