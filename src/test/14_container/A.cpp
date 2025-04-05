//
// Created by Admin on 5/04/2025.
//

#include "A.h"

#include <MyDRefl/ReflMngr.h>

void RegisterA() {
  My::MyDRefl::Mngr->RegisterTypeAuto<A>();
  My::MyDRefl::Mngr->AddField<&A::data>("data");
}
