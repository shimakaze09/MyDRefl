//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

int main() {
  auto& global =
      ReflMngr::Instance().typeinfos.at(TypeID{TypeIDRegistry::Meta::global});
  SharedBlock block = MakeSharedBlock<std::string>("My");
  global.fieldinfos.emplace(
      ReflMngr::Instance().GenerateDynamicField<const std::string>("author",
                                                                   "My"));
  std::cout << ReflMngr::Instance().RVar(StrID{"author"}).As<std::string>()
            << std::endl;
}
