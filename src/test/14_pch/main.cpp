//
// Created by Admin on 5/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>

#include "A.h"
#include "B.h"

using namespace My;
using namespace My::MyDRefl;

int main() {
  RegisterA();
  RegisterB();

  ReflMngr::Instance().RegisterTypeAuto<std::array<size_t, 5>>();

  for (const auto& [ID, info] : ReflMngr::Instance().typeinfos) {
    ReflMngr::Instance().ForEachMethod(ID, [](TypeRef type, MethodRef method) {
      std::cout << ReflMngr::Instance().tregistry.Nameof(type.ID) << ": "
                << ReflMngr::Instance().nregistry.Nameof(method.ID)
                << std::endl;
      return true;
    });
  }

  return 0;
}
