#include "Bootstrap_helper.hpp"

using namespace Smkz;
using namespace Smkz::MyDRefl;

void Smkz::MyDRefl::ext::details::Bootstrap_helper_ranges_methodrange() {
  Mngr.RegisterType<MethodRange>();
  Mngr.AddConstructor<MethodRange, ObjectView, MethodFlag>();
  Mngr.AddConstructor<MethodRange, ObjectView>();
  Mngr.AddConstructor<MethodRange, Type>();
  Mngr.AddConstructor<MethodRange::iterator, ObjectTree::iterator>();
  Mngr.AddConstructor<MethodRange::iterator, ObjectTree::iterator,
                      MethodFlag>();
  Mngr.AddMethod<&MethodRange::iterator::Valid>("Valid");
  Mngr.AddMethod<&MethodRange::iterator::GetDeriveds>("GetDeriveds");
  Mngr.AddMethod<&MethodRange::iterator::GetObjectView>("GetObjectView");
  Mngr.AddMethod<&MethodRange::iterator::GetTypeInfo>("GetTypeInfo");
}
