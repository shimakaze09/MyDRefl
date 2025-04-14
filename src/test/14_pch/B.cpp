#include "B.h"

#include <MyDRefl/MyDRefl.h>

void RegisterB() {
  My::MyDRefl::Mngr->RegisterType<B>();
  My::MyDRefl::Mngr->AddField<&B::v0>("v0");
  My::MyDRefl::Mngr->AddField<&B::v1>("v1");
  My::MyDRefl::Mngr->AddField<&B::v2>("v2");
  My::MyDRefl::Mngr->AddField<&B::v3>("v3");
  My::MyDRefl::Mngr->AddField<&B::v4>("v4");
  My::MyDRefl::Mngr->AddField<&B::v5>("v5");
}
