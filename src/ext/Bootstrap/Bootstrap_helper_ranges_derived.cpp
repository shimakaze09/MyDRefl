#include "Bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_ranges_derived() {
  Mngr.RegisterType<Ranges::Derived>();
  Mngr.AddField<&Ranges::Derived::type>("type");
  Mngr.AddField<&Ranges::Derived::typeinfo>("typeinfo");
  Mngr.AddField<&Ranges::Derived::curbase>("curbase");
  Mngr.AddField<&Ranges::Derived::ptr>("ptr");
  Mngr.RegisterType<std::span<const Ranges::Derived>>();
}
