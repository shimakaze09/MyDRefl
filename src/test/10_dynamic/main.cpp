//
// Created by Admin on 2/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

using namespace My;
using namespace My::MyDRefl;

enum class Color { RED, GREEN, BLUE };

int main() {
  ReflMngr::Instance().AddDynamicField<const std::string>(GlobalID, "author",
                                                          "My");
  ReflMngr::Instance().AddDynamicField<const Color>(GlobalID, "theme",
                                                    Color::RED);
  ReflMngr::Instance().RegisterTypeAuto<int>();
  ReflMngr::Instance().AddDynamicField<const size_t>(TypeID_of<int>, "bits",
                                                     sizeof(int) * 8);

  std::cout << ReflMngr::Instance().RVar(GlobalID, "author") << std::endl;
  std::cout << (ReflMngr::Instance().RVar(GlobalID, "theme").As<Color>() ==
                Color::RED)
            << std::endl;

  std::cout << ReflMngr::Instance().RVar(TypeID_of<int>, "bits") << std::endl;
}
