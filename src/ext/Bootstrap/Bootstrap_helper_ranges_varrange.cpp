#include "Bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_ranges_varrange() {
  Mngr.RegisterType<VarRange>();
  Mngr.AddConstructor<VarRange, ObjectView, FieldFlag>();
  Mngr.AddConstructor<VarRange, ObjectView>();
  Mngr.AddMethod<&VarRange::GetType>("GetType");
  Mngr.AddMethod<&VarRange::GetFieldFlag>("GetFieldFlag");
  Mngr.AddConstructor<VarRange::iterator, Type, void*, bool>();
  Mngr.AddConstructor<VarRange::iterator, Type, void*, bool, CVRefMode>();
  Mngr.AddConstructor<VarRange::iterator, Type, void*, bool, CVRefMode,
                      FieldFlag>();
  Mngr.AddMethod<&VarRange::iterator::Valid>("Valid");
  Mngr.AddMethod<&VarRange::iterator::GetDeriveds>("GetDeriveds");
  Mngr.AddMethod<&VarRange::iterator::GetType>("GetType");
  Mngr.AddMethod<&VarRange::iterator::GetTypeInfo>("GetTypeInfo");
  Mngr.AddMethod<&VarRange::iterator::GetFieldInfo>("GetFieldInfo");
}
