#include "ReflMngrInitUtil.hpp"

using namespace My::MyDRefl;

void My::MyDRefl::details::ReflMngrInitUtil_5(ReflMngr& mngr) {
  mngr.RegisterType<std::string_view>();
  mngr.AddConstructor<std::string_view, const char* const&>();
  mngr.AddConstructor<std::string_view, const char* const&,
                      const std::string_view::size_type&>();
  mngr.AddConstructor<std::string_view, const std::string&>();
  mngr.AddConstructor<std::string_view, const std::pmr::string&>();
}
