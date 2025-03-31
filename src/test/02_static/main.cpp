//
// Created by Admin on 31/03/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <cassert>
#include <iostream>

using namespace My::MyDRefl;

struct A {
  inline static int data = 2;
};

int main() {
  size_t ID_A = NameRegistry::Instance().Register("A");
  size_t ID_int = NameRegistry::Instance().Register("int");
  size_t ID_data = NameRegistry::Instance().Register("data");

  {  // register Point
    ObjectPtr ptr_data{ID_int, &A::data};
    StaticFieldInfo fieldinfo_data{ptr_data};
    TypeInfo typeinfo{{},  // attrs
                      {},  // fields
                      {    // static fields
                       {ID_data, fieldinfo_data}}};
    TypeInfoMngr::Instance().typeinfos.emplace(ID_A, std::move(typeinfo));
  }

  for (const auto& [ID_field, staticfieldinfo] :
       TypeInfoMngr::Instance().typeinfos.at(ID_A).staticfieldinfos) {
    auto field_name = NameRegistry::Instance().Nameof(ID_field);
    if (staticfieldinfo.objptr.GetID() == ID_int)
      std::cout << field_name << ": " << staticfieldinfo.objptr.As<int>()
                << std::endl;
  }
}
