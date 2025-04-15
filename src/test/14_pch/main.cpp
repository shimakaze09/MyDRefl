#include <MyDRefl/MyDRefl.h>
#include <iostream>

#include "A.h"
#include "B.h"

using namespace My;
using namespace My::MyDRefl;

int main() {
  RegisterA();
  RegisterB();

  for (const auto& [type, info] : Mngr.typeinfos) {
    for (const auto& [name, method] : MethodRange{type})
      std::cout << name.GetView() << std::endl;
  }

  return 0;
}
