//
// Created by Admin on 11/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>
#include <tuple>

using namespace My;
using namespace My::MyDRefl;

int main() {
  Mngr.RegisterType<std::tuple<int, float, double>>();

  SharedObject v = Mngr.MakeShared(TypeID_of<std::tuple<int, float, double>>);
  std::cout << v.TypeName() << std::endl;

  for (auto method : Mngr.GetMethods(TypeID_of<std::tuple<int, float, double>>))
    std::cout << Mngr.nregistry.Nameof(method.ID) << std::endl;

  for (std::size_t i{0}; i < v.tuple_size(); i++)
    v.tuple_get(i) = i;

  for (std::size_t i{0}; i < v.tuple_size(); i++)
    std::cout << i << ": " << v.tuple_get(i) << std::endl;

  return 0;
}
