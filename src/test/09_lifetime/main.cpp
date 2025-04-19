#include <MyDRefl/MyDRefl.hpp>
#include <iostream>

using namespace Smkz;
using namespace Smkz::MyDRefl;

struct C {
  C() { std::cout << "C ctor" << std::endl; }
  ~C() { std::cout << "C dtor" << std::endl; }
};

int main() {
  Mngr.RegisterType<C>();

  SharedObject c = Mngr.MakeShared(Type_of<C>);

  return 0;
}
