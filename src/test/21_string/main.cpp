#include <MyDRefl/MyDRefl.hpp>
#include <cmath>
#include <iostream>

using namespace Smkz;
using namespace Smkz::MyDRefl;

int main() {
  std::cout << "//" << std::endl
            << "// std::string" << std::endl
            << "////////////////" << std::endl;
  for (auto&& [name, info] : MethodRange_of<std::string>)
    std::cout << name.GetView() << std::endl;
  std::cout << "//" << std::endl
            << "// std::string_view" << std::endl
            << "/////////////////////" << std::endl;
  for (auto&& [name, info] : MethodRange_of<std::string_view>)
    std::cout << name.GetView() << std::endl;
  std::cout << "//" << std::endl
            << "// const char*" << std::endl
            << "/////////////////////" << std::endl;
  for (auto&& [name, info] : MethodRange_of<const char*>)
    std::cout << name.GetView() << std::endl;

  auto str = Mngr.MakeShared(Type_of<std::string>, TempArgsView{"hello world"});

  std::cout << str << std::endl;

  return 0;
}
