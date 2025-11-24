#include <MyDRefl_ext/Bootstrap.h>

#include <MyDRefl/MyDRefl.hpp>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct A {
  float data;
};

int main() {
  Mngr.RegisterType<A>();
  Mngr.AddField<&A::data>("data");
  My_MyDRefl_ext_Bootstrap();
  A a;
  ObjectView{a}.Var("data") = 3;
  std::cout << a.data << std::endl;
  auto data = MngrView.Invoke<ObjectView>("Var", TempArgsView{a, Name{"data"}});
  data = 4;
  std::cout << a.data << std::endl;

  return 0;
}
