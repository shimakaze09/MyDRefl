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

  for (const auto& [ID, info] : ReflMngr::Instance().typeinfos) {
    for (auto method : ReflMngr::Instance().GetMethods(ID))
      std::cout << ReflMngr::Instance().nregistry.Nameof(method.ID) << std::endl;
  }

  return 0;
}
