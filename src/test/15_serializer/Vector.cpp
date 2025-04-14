#include "Vector.h"

#include <MyDRefl/MyDRefl.h>

void RegisterVector() {
  My::MyDRefl::Mngr->RegisterType<Vector>();
  My::MyDRefl::Mngr->AddField<&Vector::data>("data");
}
