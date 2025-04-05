//
// Created by Admin on 5/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

int main() {
  ReflMngr::Instance().RegisterTypeAuto<std::array<float, 5>>();

  SharedObject arr =
      ReflMngr::Instance().MakeShared(TypeID::of<std::array<float, 5>>);
  for (SharedObject ele : arr)
    std::cout << ele->TypeName() << ": " << ele->As<float&>() << std::endl;
}