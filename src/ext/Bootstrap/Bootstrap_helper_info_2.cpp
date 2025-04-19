#include "Bootstrap_helper.hpp"

using namespace Smkz;
using namespace Smkz::MyDRefl;

void Smkz::MyDRefl::ext::details::Bootstrap_helper_info_2() {
  Mngr.AddField<&TypeInfo::fieldinfos>("fieldinfos");
}
