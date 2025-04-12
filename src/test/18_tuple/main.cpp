#include <MyDRefl/MyDRefl.h>
#include <iostream>
#include <tuple>

using namespace My;
using namespace My::MyDRefl;

int main() {
  Mngr.RegisterType<std::tuple<int, float, double>>();

  SharedObject v = Mngr.MakeShared(Type_of<std::tuple<int, float, double>>);
  std::cout << v.GetType().GetName() << std::endl;

  for (const auto& method :
       Mngr.GetMethods(Type_of<std::tuple<int, float, double>>))
    std::cout << method.name.GetView() << std::endl;

  for (std::size_t i{0}; i < v.tuple_size(); i++)
    v.tuple_get(i) = i;

  for (std::size_t i{0}; i < v.tuple_size(); i++)
    std::cout << i << ": " << v.tuple_get(i) << std::endl;

  return 0;
}
