#include "Bootstrap_helper.hpp"

using namespace Smkz;
using namespace Smkz::MyDRefl;

void Smkz::MyDRefl::ext::details::Bootstrap_helper_ranges_span_derived() {
  Mngr.RegisterType<std::span<const Ranges::Derived>>();
}
