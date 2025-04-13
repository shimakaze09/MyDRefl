#include <MyDRefl/MyDRefl.h>
#include <cmath>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Arr {
  int data[5];
};

int main() {
  Mngr.RegisterType<Arr>();
  Mngr.AddField<&Arr::data>("data");

  std::cout << "//" << std::endl
            << "// int[5]" << std::endl
            << "///////////" << std::endl;
  for (auto&& method : Mngr.GetMethods(Type_of<int[5]>))
    std::cout << method.name.GetView() << std::endl;
  auto arr = Mngr.MakeShared(Type_of<Arr>);
  auto data = arr.Var("data");
  for (std::size_t i{0}; i < data.size(); ++i)
    data[i] = i;

  std::cout << "arr.data: ";
  for (SharedObject ele : data)
    std::cout << ele << " ";

  std::cout << std::endl;
  std::cout << "data.begin().next(1).next(2): " << *data.begin().next(1).next(2)
            << std::endl;

  return 0;
}
