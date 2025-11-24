#include "ReflMngrInitUtil.hpp"

using namespace My::MyDRefl;

void My::MyDRefl::details::ReflMngrInitUtil_1(ReflMngr& mngr) {
  mngr.RegisterType<std::int8_t>();
  mngr.RegisterType<std::int16_t>();
  mngr.RegisterType<std::int32_t>();
  mngr.RegisterType<std::int64_t>();
}
