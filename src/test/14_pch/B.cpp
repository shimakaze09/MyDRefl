#include "B.hpp"

#include <MyDRefl/MyDRefl.hpp>

void RegisterB() {
  Smkz::MyDRefl::Mngr.RegisterType<B>();
  Smkz::MyDRefl::Mngr.AddField<&B::v0>("v0");
  Smkz::MyDRefl::Mngr.AddField<&B::v1>("v1");
  Smkz::MyDRefl::Mngr.AddField<&B::v2>("v2");
  Smkz::MyDRefl::Mngr.AddField<&B::v3>("v3");
  Smkz::MyDRefl::Mngr.AddField<&B::v4>("v4");
  Smkz::MyDRefl::Mngr.AddField<&B::v5>("v5");
}
