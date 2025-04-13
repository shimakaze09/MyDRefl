#include "Bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_reflmngr_1() {
  Mngr.AddField<&ReflMngr::typeinfos>("typeinfos");
}
