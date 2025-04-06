//
// Created by Admin on 5/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>

using namespace My;
using namespace My::MyDRefl;

int main() {
  Mngr->RegisterTypeAuto<std::vector<size_t>>();
  SharedObject vec = Mngr->MakeShared(TypeID_of<std::vector<size_t>>);
  for (size_t i = 0; i < 5; i++)
    vec->push_back(i);
  for (SharedObject ele : vec)
    std::cout << ele->TypeName() << ": " << ele << std::endl;
  return 0;
}
