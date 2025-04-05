//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Vec {
  float x;
  float y;

  float norm() const noexcept { return std::sqrt(x * x + y * y); }
};

int main() {
  ReflMngr::Instance().RegisterTypeAuto<Vec>();
  ReflMngr::Instance().AddField<&Vec::x>("x");
  ReflMngr::Instance().AddField<&Vec::y>("y");
  ReflMngr::Instance().AddMethod<&Vec::norm>("norm");

  SharedObject v = ReflMngr::Instance().MakeShared(TypeID::of<Vec>);
  std::cout << v->TypeName() << std::endl;  // prints "Vec"

  v->RWVar("x") = 3.f;
  v->RWVar("y") = 4.f;

  std::cout << "x: " << v->RVar("x") << std::endl;
  std::cout << "norm: " << v->DMInvoke("norm") << std::endl;

  for (auto field : ReflMngr::Instance().GetFields(TypeID::of<Vec>))
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << std::endl;

  for (auto method : ReflMngr::Instance().GetMethods(TypeID::of<Vec>))
    std::cout << ReflMngr::Instance().nregistry.Nameof(method.ID) << std::endl;

  for (const auto& [type, field, var] : v->GetTypeFieldRVars()) {
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
              << var << std::endl;
  }
}
