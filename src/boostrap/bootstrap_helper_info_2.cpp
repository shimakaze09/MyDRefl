#include "bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::details::bootstrap_helper_info_2() {
  Mngr.AddField<&TypeInfo::fieldinfos>("fieldinfos");
}
