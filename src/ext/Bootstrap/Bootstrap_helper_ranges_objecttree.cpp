#include "Bootstrap_helper.h"

using namespace My;
using namespace My::MyDRefl;

void My::MyDRefl::ext::details::Bootstrap_helper_ranges_objecttree() {
  Mngr.RegisterType<ObjectTree>();
  Mngr.AddConstructor<ObjectTree, ObjectView>();
  Mngr.AddConstructor<ObjectTree::iterator, ObjectView, bool>();
  Mngr.AddMethod<&ObjectTree::iterator::Valid>("Valid");
  Mngr.AddMethod<&ObjectTree::iterator::GetDeriveds>("GetDeriveds");
}
