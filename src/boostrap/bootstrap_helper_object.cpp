#include "bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::details::bootstrap_helper_object() {
  Mngr.RegisterType<ObjectView>();
  Mngr.AddConstructor<ObjectView, Type, void*>();
  Mngr.AddConstructor<ObjectView, Type>();
  Mngr.AddConstructor<ObjectView, SharedObject>();
  Mngr.AddMethod<&ObjectView::GetType>("GetType");
  Mngr.AddMethod<&ObjectView::GetPtr>("GetPtr");

  Mngr.RegisterType<SharedObject>();
  Mngr.AddConstructor<SharedObject, Type, void*>();
  Mngr.AddConstructor<SharedObject, Type>();
  Mngr.AddConstructor<SharedObject, ObjectView>();
  Mngr.AddMethod<&SharedObject::Reset>("Reset");
  Mngr.AddMethod<&SharedObject::UseCount>("UseCount");
  Mngr.AddMethod<&SharedObject::IsObjectView>("IsObjectView");
  Mngr.AddMethod<&SharedObject::Swap>("Swap");
}
