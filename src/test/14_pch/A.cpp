#include "A.h"

#include <MyDRefl/MyDRefl.h>

void RegisterA() {
  My::MyDRefl::Mngr.RegisterType<A>();
  My::MyDRefl::Mngr.AddField<&A::data>("data");
}
