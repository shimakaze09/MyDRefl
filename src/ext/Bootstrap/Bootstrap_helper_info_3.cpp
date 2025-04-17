#include "Bootstrap_helper.hpp"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_info_3() {
  Mngr.AddField<&TypeInfo::methodinfos>("methodinfos");
}
