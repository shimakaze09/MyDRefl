#include "ReflMngrInitUtil.h"

using namespace My::MyDRefl;

void My::MyDRefl::details::ReflMngrInitUtil_2(ReflMngr& mngr) {
  mngr.RegisterType<std::uint8_t>();
  mngr.RegisterType<std::uint16_t>();
  mngr.RegisterType<std::uint32_t>();
  mngr.RegisterType<std::uint64_t>();
}