#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

enum class Color { Red, Green, Blue };

int main() {
  Mngr.AddDynamicField<const std::string>(Global, "author", "My");
  Mngr.AddDynamicField<const Color>(Global, "theme", Color::Red);
  Mngr.RegisterType<int>();
  Mngr.AddDynamicField<const size_t>(Type_of<int>, "bits", sizeof(int) * 8);

  std::cout << Mngr.Var(Global, "author") << std::endl;
  std::cout << (Mngr.Var(Global, "theme") == Color::Red) << std::endl;

  std::cout << Mngr.Var(Type_of<int>, "bits") << std::endl;
}
