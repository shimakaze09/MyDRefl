#include <MyDRefl/MyDRefl.h>
#include <cmath>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

int main() {
  Mngr->RegisterType<int**>();
  int i = 0;
  int* pi = &i;
  int** ppi = &pi;

  std::cout << "//" << std::endl
            << "// int**" << std::endl
            << "//////////" << std::endl;
  for (auto&& method : Mngr->GetMethods(Type_of<int**>))
    std::cout << method.name.GetView() << std::endl;
  std::cout << "//" << std::endl
            << "// int*" << std::endl
            << "/////////" << std::endl;
  for (auto&& method : Mngr->GetMethods(Type_of<int*>))
    std::cout << method.name.GetView() << std::endl;

  SharedObject ref_i = **ObjectView{ppi};
  ref_i = 3;

  std::cout << i << std::endl;

  return 0;
}
