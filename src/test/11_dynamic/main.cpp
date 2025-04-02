//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My::MyDRefl;

int main() {
  auto& global = ReflMngr::Instance().typeinfos.at(
      TypeRegistry::DirectGetID(TypeRegistry::Meta::global));
  SharedBlock block = MakeSharedBlock<std::string>("My");
  global.fieldinfos.emplace(
      ReflMngr::Instance().nregistry.GetID("author"),
      FieldInfo{ReflMngr::Instance().GenerateDynamicFieldPtr<const std::string>(
          "My")});
  std::cout << ReflMngr::Instance()
                   .RVar(ReflMngr::Instance().nregistry.DirectGetID("author"))
                   .As<std::string>()
            << std::endl;
}
