#include <MyDRefl/MyDRefl.h>
#include <iostream>

#include "A.h"
#include "B.h"

using namespace My;
using namespace My::MyDRefl;

int main() {
  RegisterA();
  RegisterB();

  for (const auto& [ID, info] : Mngr->typeinfos) {
    for (const auto& method : Mngr->GetMethods(ID))
      std::cout << method.name.GetView() << std::endl;
  }

  return 0;
}
