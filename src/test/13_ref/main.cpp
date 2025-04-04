//
// Created by Admin on 4/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct Data {
  Data(float value) : value{value} {}

  Data(const Data& f) : value{f.value} {
    std::cout << "copy ctor" << std::endl;
  }

  Data(Data&& f) noexcept : value{f.value} {
    std::cout << "move ctor" << std::endl;
  }

  Data& operator=(const Data& rhs) {
    value = rhs.value;
    return *this;
  }

  Data& operator=(Data&& rhs) noexcept {
    value = rhs.value;
    return *this;
  }

  float value;
};

struct A {
  A(Data& lref, Data&& rref) : lref{lref}, rref{std::move(rref)} {}

  Data& lref;
  Data&& rref;

  Data get() { return lref; }

  const Data& get_c() const { return lref; }

  Data& get_l() { return lref; }

  Data&& get_r() { return std::move(lref); }

  void set(Data d) { lref = d; }

  void set_c(const Data& d) { lref = d; }

  void set_l(Data& d) { lref = d; }

  void set_r(Data&& d) { lref = std::move(d); }
};

int main() {

  ReflMngr::Instance().RegisterTypeAuto<A, Data&, Data&&>();
  ReflMngr::Instance().AddField("lref", [](A* a) { return &a->lref; });
  ReflMngr::Instance().AddField("rref", [](A* a) { return &a->rref; });
  ReflMngr::Instance().AddMethod<&A::get>("get");
  ReflMngr::Instance().AddMethod<&A::get_c>("get_c");
  ReflMngr::Instance().AddMethod<&A::get_l>("get_l");
  ReflMngr::Instance().AddMethod<&A::get_r>("get_r");
  ReflMngr::Instance().AddMethod<&A::set>("set");
  ReflMngr::Instance().AddMethod<&A::set_c>("set_c");
  ReflMngr::Instance().AddMethod<&A::set_l>("set_l");
  ReflMngr::Instance().AddMethod<&A::set_r>("set_r");

  Data f = 1.f;
  Data g = 2.f;
  auto a = ReflMngr::Instance().MakeShared<Data&, Data&&>(TypeID::of<A>, f,
                                                          std::move(g));
  std::cout << "a.rref: " << a->RVar("rref").As<Data>().value << std::endl;
  std::cout << "a.lref: " << a->RVar("lref").As<Data>().value << std::endl;

  a->RWVar("lref").As<Data>().value = 2.f;
  a->RWVar("rref").As<Data>().value = 3.f;
  std::cout << "a.lref: " << a->RVar("lref").As<Data>().value << std::endl;
  std::cout << "a.rref: " << a->RVar("rref").As<Data>().value << std::endl;
  std::cout << "f: " << f.value << std::endl;
  std::cout << "g: " << g.value << std::endl;

  std::cout << "a.get_r(): " << a->Invoke<Data&&>("get_r").value << std::endl;
  std::cout << "a.get_c(): " << a->Invoke<const Data&>("get_c").value
            << std::endl;

  a->Invoke<void, Data>("set", g);
  a->Invoke<void, Data&&>("set_r", std::move(g));
  std::cout << "a.lref: " << a->Invoke<Data&&>("get_r").value << std::endl;
  std::cout << "a.get_c(): "
            << a->DMInvoke("get_c")->DereferenceAsConst().As<Data>().value
            << std::endl;
  return 0;
}
