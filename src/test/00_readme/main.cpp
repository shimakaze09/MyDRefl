//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <cmath>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Vec {
  float x;
  float y;

  float norm() const { return std::sqrt(x * x + y * y); }
};

int main() {
  Mngr.RegisterType<Vec>();
  Mngr.AddField<&Vec::x>("x");
  Mngr.AddField<&Vec::y>("y");
  Mngr.AddMethod<&Vec::norm>("norm");

  SharedObject v = Mngr.MakeShared(TypeID_of<Vec>);
  std::cout << v.TypeName() << std::endl;  // prints "Vec"

  v.Var("x") = 3;
  v.Var("y") = 4;

  std::cout << "x: " << v.Var("x") << std::endl;
  std::cout << "norm: " << v.DMInvoke("norm") << std::endl;

  for (auto field : Mngr.GetFields(TypeID_of<Vec>))
    std::cout << Mngr.nregistry.Nameof(field.ID) << std::endl;

  for (auto method : Mngr.GetMethods(TypeID_of<Vec>))
    std::cout << Mngr.nregistry.Nameof(method.ID) << std::endl;

  for (const auto& [type, field, var] : v.GetTypeFieldVars()) {
    std::cout << Mngr.nregistry.Nameof(field.ID) << ": " << var << std::endl;
  }
}
