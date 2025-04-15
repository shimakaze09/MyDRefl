#include <MyDRefl/MyDRefl.h>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct C {
  C() { std::cout << "C ctor" << std::endl; }

  ~C() { std::cout << "C dtor" << std::endl; }
};

int main() {
  Mngr.RegisterType<C>();

  SharedObject c = Mngr.MakeShared(Type_of<C>);

  return 0;
}
