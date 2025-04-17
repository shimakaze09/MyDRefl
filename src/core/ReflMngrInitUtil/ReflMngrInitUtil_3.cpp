#include "ReflMngrInitUtil.hpp"

using namespace My::MyDRefl;

void My::MyDRefl::details::ReflMngrInitUtil_3(ReflMngr& mngr) {
  mngr.RegisterType<bool>();
  mngr.RegisterType<std::nullptr_t>();
  mngr.RegisterType<void*>();
  mngr.RegisterType<float>();
  mngr.RegisterType<double>();
}
