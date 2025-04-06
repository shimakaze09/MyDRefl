//
// Created by Admin on 6/04/2025.
//

#include "Vector.h"
 
 #include <MyDRefl/MyDRefl.h>
 
 void RegisterVector() {
   My::MyDRefl::Mngr->RegisterTypeAuto<Vector>();
   My::MyDRefl::Mngr->AddField<&Vector::data>("data");
 }
