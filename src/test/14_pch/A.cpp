#include "A.hpp"

#include <MyDRefl/MyDRefl.hpp>

void RegisterA() {
  My::MyDRefl::Mngr.RegisterType<A>();
  My::MyDRefl::Mngr.AddField<&A::data>("data");
}
