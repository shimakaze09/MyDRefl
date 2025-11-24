#include "Bootstrap_helper.hpp"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_ranges_span_derived() {
  Mngr.RegisterType<std::span<const Ranges::Derived>>();
}
