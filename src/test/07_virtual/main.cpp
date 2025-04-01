//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <array>
#include <iostream>

using namespace My::MyDRefl;

struct A {
  float a{0.f};
};

struct B : virtual A {
  float b{0.f};
};

struct C : virtual A {
  float c{0.f};
};

struct D : B, C {
  float d{0.f};
};

int main() {
  size_t ID_A = ReflMngr::Instance().registry.Register("A");
  size_t ID_B = ReflMngr::Instance().registry.Register("B");
  size_t ID_C = ReflMngr::Instance().registry.Register("C");
  size_t ID_D = ReflMngr::Instance().registry.Register("D");
  size_t ID_a = ReflMngr::Instance().registry.Register("a");
  size_t ID_b = ReflMngr::Instance().registry.Register("b");
  size_t ID_c = ReflMngr::Instance().registry.Register("c");
  size_t ID_d = ReflMngr::Instance().registry.Register("d");
  size_t ID_float = ReflMngr::Instance().registry.Register("float");

  {  // register
    TypeInfo typeinfo_A{
        {},                                     // attrs
        {{ID_a, {{ID_float, offsetof(A, a)}}}}  // fieldinfos
    };
    TypeInfo typeinfo_B{
        {},                                                      // attrs
        {{ID_b, {{ID_float, field_offset_function<&B::b>()}}}},  // fieldinfos
        {},                                                      // methodinfos
        {{ID_A, BaseInfo::Make<B, A>()}},                        // baseinfos
    };
    TypeInfo typeinfo_C{
        {},                                                      // attrs
        {{ID_c, {{ID_float, field_offset_function<&C::c>()}}}},  // fieldinfos
        {},                                                      // methodinfos
        {{ID_A, BaseInfo::Make<C, A>()}},                        // baseinfos
    };
    TypeInfo typeinfo_D{
        {},                                                      // attrs
        {{ID_d, {{ID_float, field_offset_function<&D::d>()}}}},  // fieldinfos
        {},                                                      // methodinfos
        {
            {ID_B, BaseInfo::Make<D, B>()},
            {ID_C, BaseInfo::Make<D, C>()},
        },  //baseinfos
    };

    ReflMngr::Instance().typeinfos.emplace(ID_A, std::move(typeinfo_A));
    ReflMngr::Instance().typeinfos.emplace(ID_B, std::move(typeinfo_B));
    ReflMngr::Instance().typeinfos.emplace(ID_C, std::move(typeinfo_C));
    ReflMngr::Instance().typeinfos.emplace(ID_D, std::move(typeinfo_D));
  }

  D d;
  d.a = 1.f;
  d.b = 2.f;
  d.c = 3.f;
  d.d = 4.f;

  ObjectPtr ptr{ID_D, &d};

  ReflMngr::Instance().RWField(ptr, ID_a).As<float>() = 10.f;

  ReflMngr::Instance().ForEachRField(
      ptr, [](size_t typeID, const TypeInfo& typeinfo, size_t fieldID,
              const FieldInfo& fieldinfo, ConstObjectPtr field) {
        std::cout << ReflMngr::Instance().registry.Nameof(fieldID) << ": "
                  << field.As<float>() << std::endl;
      });

  return 0;
}
