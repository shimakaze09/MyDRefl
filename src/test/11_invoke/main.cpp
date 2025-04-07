//
// Created by Admin on 4/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <cmath>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Vec {
  float x;
  float y;

  float norm() const noexcept { return std::sqrt(x * x + y * y); }

  Vec copy() const noexcept { return *this; }

  Vec operator+(const Vec& v) const noexcept {
    Vec rst;
    rst.x = x + v.x;
    rst.y = y + v.y;
    return rst;
  }
};

int main() {
  ReflMngr::Instance().RegisterType<Vec>();
  ReflMngr::Instance().AddField<&Vec::x>("x");
  ReflMngr::Instance().AddField<&Vec::y>("y");
  ReflMngr::Instance().AddMethod<&Vec::norm>("norm");
  ReflMngr::Instance().AddMethod<&Vec::copy>("copy");

  SharedObject v = ReflMngr::Instance().MakeShared(TypeID_of<Vec>);

  v->RWVar("x") = 3.f;
  v->RWVar("y") = 4.f;

  for (auto method : ReflMngr::Instance().GetMethods(TypeID_of<Vec>))
    std::cout << ReflMngr::Instance().nregistry.Nameof(method.ID) << std::endl;

  for (const auto& [type, field, var] : v->GetTypeFieldRVars()) {
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": " << var
              << std::endl;
  }

  auto w0 = v->MInvoke(StrIDRegistry::MetaID::operator_add,
                       std::pmr::get_default_resource(), v.As<Vec>());
  auto w1 = v->DMInvoke(StrIDRegistry::MetaID::operator_add, v.As<Vec>());
  auto w2 = v->ADMInvoke(StrIDRegistry::MetaID::operator_add, v);

  std::array arr_w = {w0, w1, w2};
  for (auto w : arr_w) {
    for (const auto& [type, field, var] : w->GetTypeFieldRVars()) {
      std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
                << var << std::endl;
    }
  }
}
