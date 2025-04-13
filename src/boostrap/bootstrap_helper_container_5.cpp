#include "bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::details::bootstrap_helper_container_5() {
  Mngr.RegisterType<
      std::vector<std::tuple<InfoTypePair, InfoFieldPair, ObjectView>>>();
}
