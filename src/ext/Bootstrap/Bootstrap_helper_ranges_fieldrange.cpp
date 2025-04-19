#include "Bootstrap_helper.hpp"

using namespace Smkz;
using namespace Smkz::MyDRefl;

void Smkz::MyDRefl::ext::details::Bootstrap_helper_ranges_fieldrange() {
  Mngr.RegisterType<FieldRange>();
  Mngr.AddConstructor<FieldRange, ObjectView, FieldFlag>();
  Mngr.AddConstructor<FieldRange, ObjectView>();
  Mngr.AddConstructor<FieldRange, Type>();
  Mngr.AddConstructor<FieldRange::iterator, ObjectTree::iterator>();
  Mngr.AddConstructor<FieldRange::iterator, ObjectTree::iterator, FieldFlag>();
  Mngr.AddMethod<&FieldRange::iterator::Valid>("Valid");
  Mngr.AddMethod<&FieldRange::iterator::GetDeriveds>("GetDeriveds");
  Mngr.AddMethod<&FieldRange::iterator::GetObjectView>("GetObjectView");
  Mngr.AddMethod<&FieldRange::iterator::GetTypeInfo>("GetTypeInfo");
}
