#include "Bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_ranges_fieldrange() {
  Mngr.RegisterType<FieldRange>();
  Mngr.AddConstructor<FieldRange, Type, FieldFlag>();
  Mngr.AddConstructor<FieldRange, Type>();
  Mngr.AddMethod<&FieldRange::GetType>("GetType");
  Mngr.AddConstructor<FieldRange::iterator, TypeTree::iterator>();
  Mngr.AddConstructor<FieldRange::iterator, TypeTree::iterator, FieldFlag>();
  Mngr.AddMethod<&FieldRange::iterator::Valid>("Valid");
  Mngr.AddMethod<&FieldRange::iterator::GetDeriveds>("GetDeriveds");
}
