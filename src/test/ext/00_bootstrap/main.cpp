#include <MyDRefl/MyDRefl.h>
#include <MyDRefl_ext/Bootstrap.h>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct A {
  float data;
};

int main() {
  Mngr->RegisterType<A>();
  Mngr->AddField<&A::data>("data");
  ext::Bootstrap();
  A a;
  ObjectView{a}.Var("data") = 3;
  std::cout << a.data << std::endl;
  auto data = MngrView.Invoke<ObjectView>("Var", ObjectView{a}, Name{"data"},
                                          FieldFlag::All);
  data = 4;
  std::cout << a.data << std::endl;

  return 0;
}
