#include "A.hpp"

#include <MyDRefl/MyDRefl.hpp>

void RegisterA() {
  Smkz::MyDRefl::Mngr.RegisterType<A>();
  Smkz::MyDRefl::Mngr.AddField<&A::data>("data");
}
