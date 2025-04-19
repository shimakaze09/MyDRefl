#include "Bootstrap_helper.hpp"

using namespace Smkz;
using namespace Smkz::MyDRefl;

void Smkz::MyDRefl::ext::details::Bootstrap_helper_reflmngr_1() {
  Mngr.AddField<&ReflMngr::typeinfos>("typeinfos");
}
