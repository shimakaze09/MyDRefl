#include "Bootstrap_helper.hpp"

using namespace Smkz;
using namespace Smkz::MyDRefl;

void Smkz::MyDRefl::ext::details::Bootstrap_helper_reflmngr_2() {
  Mngr.RegisterType<std::span<const Type>>();
}
