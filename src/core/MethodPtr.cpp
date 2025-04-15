#include <MyDRefl/MethodPtr.h>

using namespace My::MyDRefl;

MethodPtr::MethodPtr(Func func, MethodFlag flag, Type result_type,
                     ParamList paramList)
    : func{std::move(func)},
      flag{flag},
      result_type{result_type},
      paramList{std::move(paramList)} {
  assert(enum_single(flag));
}

void MethodPtr::Invoke(void* obj, void* result_buffer, ArgsView args) const {
  func(obj, result_buffer, args);
};

void MethodPtr::Invoke(const void* obj, void* result_buffer,
                       ArgsView args) const {
  if (flag == MethodFlag::Variable)
    return;
  func(const_cast<void*>(obj), result_buffer, args);
};

void MethodPtr::Invoke(void* result_buffer, ArgsView args) const {
  if (flag != MethodFlag::Static)
    return;
  func(nullptr, result_buffer, args);
};
