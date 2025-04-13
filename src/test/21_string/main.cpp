#include <MyDRefl/MyDRefl.h>
#include <cmath>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

int main() {
  std::cout << "//" << std::endl
            << "// std::string" << std::endl
            << "////////////////" << std::endl;
  for (auto&& method : Mngr.GetMethods(Type_of<std::string>))
    std::cout << method.name.GetView() << std::endl;
  std::cout << "//" << std::endl
            << "// std::string_view" << std::endl
            << "/////////////////////" << std::endl;
  for (auto&& method : Mngr.GetMethods(Type_of<std::string_view>))
    std::cout << method.name.GetView() << std::endl;
  std::cout << "//" << std::endl
            << "// const char*" << std::endl
            << "/////////////////////" << std::endl;
  for (auto&& method : Mngr.GetMethods(Type_of<const char*>))
    std::cout << method.name.GetView() << std::endl;

  auto str = Mngr.MakeShared(Type_of<std::string>, "hello world");

  std::cout << str << std::endl;

  return 0;
}
