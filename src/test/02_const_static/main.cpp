//
// Created by Admin on 31/03/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

struct A {
  int data{1};
  const int c_data{2};
  inline static int s_data{3};
  inline static const int sc_data{4};
};

int main() {
  ReflMngr::Instance().RegisterTypeAuto<A>();
  ReflMngr::Instance().AddField<&A::data>("data");
  ReflMngr::Instance().AddField<&A::c_data>("c_data");
  ReflMngr::Instance().AddField<&A::s_data>("s_data");
  ReflMngr::Instance().AddField<&A::sc_data>("sc_data");

  A a;
  auto ptr = Ptr(a);

  ReflMngr::Instance().ForEachField(TypeID_of<A>, [](TypeRef, FieldRef field) {
    auto field_name = ReflMngr::Instance().nregistry.Nameof(field.ID);
    std::cout << field_name << std::endl;
    return true;
  });

  ReflMngr::Instance().RWVar(ptr, "data") = 10;
  ReflMngr::Instance().RWVar(ptr, "s_data") = 20;

  ReflMngr::Instance().ForEachRVar(
      TypeID_of<A>, [](TypeRef, FieldRef field, ConstObjectPtr ptr) {
        std::cout << ptr << std::endl;
        return true;
      });

  return 0;
}
