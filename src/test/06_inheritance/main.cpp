//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <array>
#include <iostream>

using namespace My::MyDRefl;

struct A {
  float a;
};

struct B : A {
  float b;
};

struct C : A {
  float c;
};

struct D : B, C {
  float d;
};

int main() {
  auto ID_A = ReflMngr::Instance().tregistry.Register<A>();
  auto ID_B = ReflMngr::Instance().tregistry.Register<B>();
  auto ID_C = ReflMngr::Instance().tregistry.Register<C>();
  auto ID_D = ReflMngr::Instance().tregistry.Register<D>();
  auto ID_float = ReflMngr::Instance().tregistry.Register<float>();

  auto ID_a = ReflMngr::Instance().nregistry.Register("a");
  auto ID_b = ReflMngr::Instance().nregistry.Register("b");
  auto ID_c = ReflMngr::Instance().nregistry.Register("c");
  auto ID_d = ReflMngr::Instance().nregistry.Register("d");

  {  // register
    TypeInfo typeinfo_A{sizeof(A),
                        alignof(A),
                        {// fieldinfos
                         {ID_a, {{ID_float, offsetof(A, a)}}}}};
    TypeInfo typeinfo_B{sizeof(B),
                        alignof(B),
                        {// fieldinfos
                         {ID_b, {{ID_float, offsetof(B, b)}}}},
                        {},  // methodinfos
                        {    //baseinfos
                         {ID_A, {MakeBaseInfo<B, A>()}}}};
    TypeInfo typeinfo_C{sizeof(C),
                        alignof(C),
                        {// fieldinfos
                         {ID_c, {{ID_float, offsetof(C, c)}}}},
                        {},  // methodinfos
                        {    //baseinfos
                         {ID_A, {MakeBaseInfo<C, A>()}}}};
    TypeInfo typeinfo_D{sizeof(D),
                        alignof(D),
                        {// fieldinfos
                         {ID_d, {{ID_float, offsetof(D, d)}}}},
                        {},  // methodinfos
                        {
                            //baseinfos
                            {ID_B, {MakeBaseInfo<D, B>()}},
                            {ID_C, {MakeBaseInfo<D, C>()}},
                        }};

    ReflMngr::Instance().typeinfos.emplace(ID_A, std::move(typeinfo_A));
    ReflMngr::Instance().typeinfos.emplace(ID_B, std::move(typeinfo_B));
    ReflMngr::Instance().typeinfos.emplace(ID_C, std::move(typeinfo_C));
    ReflMngr::Instance().typeinfos.emplace(ID_D, std::move(typeinfo_D));
  }

  D d;
  d.C::a = 1.f;
  d.B::a = 2.f;
  d.b = 3.f;
  d.c = 4.f;
  d.d = 5.f;

  ObjectPtr ptr{ID_D, &d};

  ReflMngr::Instance()
      .RWVar(ReflMngr::Instance().StaticCast_DerivedToBase(ptr, ID_C), ID_a)
      .As<float>() = 10.f;

  ReflMngr::Instance().ForEachRVar(
      ptr, [](Type type, Field field, ConstObjectPtr var) {
        std::cout << ReflMngr::Instance().nregistry.Nameof(field.ID) << ": "
                  << var.As<float>() << std::endl;
      });

  return 0;
}
