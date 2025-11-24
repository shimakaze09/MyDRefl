#include "Bootstrap_helper.hpp"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_info_1() {
  Mngr.RegisterType<AttrSet>();

  Mngr.AddField<&FieldInfo::attrs>("attrs");

  Mngr.AddField<&MethodInfo::attrs>("attrs");

  Mngr.AddField<&TypeInfo::attrs>("attrs");
}
