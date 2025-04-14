#include <MyDRefl/MyDRefl.h>
#include <cmath>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Funcs {
  void f(double) { std::cout << "f(double)" << std::endl; }

  void f(std::uint8_t&) { std::cout << "f(std::uint8_t&)" << std::endl; }

  void f(const std::uint8_t&) {
    std::cout << "f(const std::uint8_t&)" << std::endl;
  }

  void f(std::uint8_t&&) { std::cout << "f(std::uint8_t&&)" << std::endl; }

  void f(const std::uint8_t&&) {
    std::cout << "f(const std::uint8_t&&)" << std::endl;
  }

  void g(const char*) { std::cout << "f(const char*)" << std::endl; }

  void g(int (&&)[]) { std::cout << "f(int(&&)[])" << std::endl; }
};

int main() {
  Mngr->RegisterType<Funcs>();
  Mngr->AddMethod<MemFuncOf<Funcs, void(double)>::get(&Funcs::f)>("f");
  Mngr->AddMethod<MemFuncOf<Funcs, void(std::uint8_t&)>::get(&Funcs::f)>("f");
  Mngr->AddMethod<MemFuncOf<Funcs, void(const std::uint8_t&)>::get(&Funcs::f)>(
      "f");
  Mngr->AddMethod<MemFuncOf<Funcs, void(std::uint8_t&&)>::get(&Funcs::f)>("f");
  Mngr->AddMethod<MemFuncOf<Funcs, void(const std::uint8_t&&)>::get(&Funcs::f)>(
      "f");
  Mngr->AddMethod<MemFuncOf<Funcs, void(const char*)>::get(&Funcs::g)>("g");
  Mngr->AddMethod<MemFuncOf<Funcs, void(int (&&)[])>::get(&Funcs::g)>("g");

  SharedObject funcs = Mngr->MakeShared(Type_of<Funcs>);

  std::uint8_t i = 1;
  const std::uint8_t ci = 1;
  funcs.BInvoke<void>("f", 1.);
  funcs.BInvoke<void>("f", 1.f);
  funcs.BInvoke<void>("f", 1);
  funcs.BInvoke<void>("f", i);
  funcs.BInvoke<void>("f", ci);
  funcs.BInvoke<void>("f", std::move(i));
  funcs.BInvoke<void>("f", static_cast<const std::uint8_t&&>(ci));

  int arr_i[5];
  funcs.BInvoke<void>("g", "hello");  // const char(&)[6]
  funcs.BInvoke<void>("g", arr_i);    // int(&)[5]
}
