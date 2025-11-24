#include "Bootstrap_helper.hpp"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_reflmngr_2() {
  Mngr.RegisterType<std::span<const Type>>();
}
