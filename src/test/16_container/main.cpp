#include <MyDRefl/MyDRefl.hpp>
#include <iostream>

using namespace Smkz;
using namespace Smkz::MyDRefl;

int main() {
  Mngr.RegisterType<std::vector<size_t>>();
  SharedObject vec = Mngr.MakeShared(Type_of<std::vector<size_t>>);
  for (size_t i = 0; i < 5; i++) vec.push_back(i);
  for (SharedObject ele : vec)
    std::cout << ele.GetType().GetName() << ": " << ele << std::endl;
  return 0;
}
