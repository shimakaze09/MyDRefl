//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

int main() {
  ReflMngr::Instance().AddDynamicField<const std::string>(GlobalID, "author",
                                                          "My");
  ReflMngr::Instance().RegisterTypePro<int>();
  ReflMngr::Instance().AddDynamicField<const size_t>(TypeID::of<int>, "bits",
                                                     sizeof(int) * 8);

  std::cout
      << ReflMngr::Instance().RVar(GlobalID, StrID{"author"}).As<std::string>()
      << std::endl;

  std::cout
      << ReflMngr::Instance().RVar(TypeID::of<int>, StrID{"bits"}).As<size_t>()
      << std::endl;
}
