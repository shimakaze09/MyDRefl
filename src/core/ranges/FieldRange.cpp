#include <MyDRefl/ranges/FieldRange.h>

#include <MyDRefl/ReflMngr.h>

using namespace My;
using namespace My::MyDRefl;

FieldRange::iterator::iterator(TypeTree::iterator typeiter, FieldFlag flag)
    : typeiter{std::move(typeiter)},
      flag{flag},
      mode{typeiter.Valid() ? 0 : -1} {
  if (typeiter.Valid())
    update();
}

void FieldRange::iterator::update() {
  switch (mode) {
    case 0:
      goto mode_0;
    case 1:
      goto mode_1;
    default:
      assert(false);
      return;
  }

mode_0:
  mode = 1;
  while (typeiter.Valid()) {
    if (!std::get<TypeInfo*>(*typeiter)) {
      ++typeiter;
      continue;
    }

    curfield = std::get<TypeInfo*>(*typeiter)->fieldinfos.begin();
    while (curfield != std::get<TypeInfo*>(*typeiter)->fieldinfos.end()) {
      if (enum_contain_any(flag, curfield->second.fieldptr.GetFieldFlag())) {
        return;  // yield
      mode_1:;
      }
      ++curfield;
    }

    ++typeiter;
  }

  mode = -1;
  return;  // stop
}

FieldRange::iterator& FieldRange::iterator::operator++() {
  update();
  return *this;
}

FieldRange::iterator FieldRange::iterator::operator++(int) {
  FieldRange::iterator iter = *this;
  (void)operator++();
  return iter;
}

namespace My::MyDRefl {
bool operator==(const FieldRange::iterator& lhs,
                const FieldRange::iterator& rhs) {
  assert(lhs.flag == rhs.flag);
  if (lhs.Valid()) {
    if (rhs.Valid()) {
      if (lhs.typeiter == rhs.typeiter)
        return lhs.curfield == rhs.curfield;
      else
        return false;
    } else
      return false;
  } else if (rhs.Valid())
    return false;
  else
    return lhs.typeiter == rhs.typeiter;
}

bool operator!=(const FieldRange::iterator& lhs,
                const FieldRange::iterator& rhs) {
  return !(lhs == rhs);
}
}  // namespace My::MyDRefl
