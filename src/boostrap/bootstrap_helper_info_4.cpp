#include "bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::details::bootstrap_helper_info_4() {
  Mngr.AddField<&TypeInfo::baseinfos>("baseinfos");
}
