//
// Created by Admin on 5/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

int main() {
  ReflMngr::Instance().RegisterTypeAuto<std::array<float, 5>>();
  SharedObject pArr =
      ReflMngr::Instance().MakeShared(TypeID::of<std::array<float, 5>>);
  for (auto ele : pArr.AsObjectPtr())
    std::cout << ele->TypeName() << ": " << ele->As<const float&>()
              << std::endl;
}