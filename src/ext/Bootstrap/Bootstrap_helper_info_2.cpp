#include "Bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_info_2() {
  Mngr.AddField<&TypeInfo::fieldinfos>("fieldinfos");
}
