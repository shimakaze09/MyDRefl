//
// Created by Admin on 11/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>
#include <tuple>

using namespace My;
using namespace My::MyDRefl;

int main() {
  ReflMngr::Instance().RegisterType<std::tuple<int, float, double>>();

  SharedObject v = ReflMngr::Instance().MakeShared(
      TypeID_of<std::tuple<int, float, double>>);
  std::cout << v->TypeName() << std::endl;

  v->Var("__0") = 1;
  v->Var("__1") = 2;
  v->Var("__2") = 3;

  std::cout << "__0: " << v->Var("__0") << std::endl;
  std::cout << "__1: " << v->tuple_get(1) << std::endl;
  std::cout << "__2: " << v->tuple_get(2) << std::endl;

  for (auto field : ReflMngr::Instance().GetFields(
           TypeID_of<std::tuple<int, float, double>>))
    std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << std::endl;

  for (auto method : ReflMngr::Instance().GetMethods(
           TypeID_of<std::tuple<int, float, double>>))
    std::cout << ReflMngr::Instance().nregistry.Nameof(method.ID) << std::endl;

  for (const auto& [type, field, var] : v->GetTypeFieldVars()) {
    std::cout << ReflMngr::Instance().tregistry.Nameof(
                     field.info.fieldptr.GetValueID())
              << " " << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
              << var << std::endl;
  }
}
