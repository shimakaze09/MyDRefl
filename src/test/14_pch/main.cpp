#include <MyDRefl/MyDRefl.hpp>
#include <iostream>

#include "A.hpp"
#include "B.hpp"

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
