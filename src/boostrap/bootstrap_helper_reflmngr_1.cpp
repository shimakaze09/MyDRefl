#include "bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::details::bootstrap_helper_reflmngr_1() {
  Mngr.AddField<&ReflMngr::typeinfos>("typeinfos");
}
