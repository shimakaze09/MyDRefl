#include "Bootstrap_helper.hpp"

using namespace Smkz;
using namespace Smkz::MyDRefl;

void Smkz::MyDRefl::ext::details::Bootstrap_helper_ranges_objecttree() {
  Mngr.RegisterType<ObjectTree>();
  Mngr.AddConstructor<ObjectTree, ObjectView>();
  Mngr.AddConstructor<ObjectTree, Type>();
  Mngr.AddConstructor<ObjectTree::iterator, ObjectView, bool>();
  Mngr.AddMethod<&ObjectTree::iterator::Valid>("Valid");
  Mngr.AddMethod<&ObjectTree::iterator::GetDeriveds>("GetDeriveds");
}
