//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

enum class Color { Red, Green, Blue };

int main() {
  Mngr.AddDynamicField<const std::string>(GlobalID, "author", "My");
  Mngr.AddDynamicField<const Color>(GlobalID, "theme", Color::Red);
  Mngr.RegisterType<int>();
  Mngr.AddDynamicField<const size_t>(TypeID_of<int>, "bits", sizeof(int) * 8);

  std::cout << Mngr.Var(GlobalID, "author") << std::endl;
  std::cout << (Mngr.Var(GlobalID, "theme") == Color::Red) << std::endl;

  std::cout << Mngr.Var(TypeID_of<int>, "bits") << std::endl;
}
