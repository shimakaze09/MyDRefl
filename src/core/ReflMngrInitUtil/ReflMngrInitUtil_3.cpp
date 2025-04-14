#include "ReflMngrInitUtil.h"

using namespace My::MyDRefl;

void My::MyDRefl::details::ReflMngrInitUtil_3(ReflMngr& mngr) {
  mngr.RegisterType<bool>();
  mngr.RegisterType<float>();
  mngr.RegisterType<double>();
}