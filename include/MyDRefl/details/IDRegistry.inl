//
// Created by Admin on 2/04/2025.
//

#pragma once

#include <cassert>
#include <cstring>

namespace My::MyDRefl {
template <typename T>
IDRegistry<T>::IDRegistry()
    : id2name{&resource}
#ifndef NDEBUG
      ,
      unmanagedIDs{&resource}
#endif  // !NDEBUG
{
}

template <typename T>
void IDRegistry<T>::RegisterUnmanaged(T ID, std::string_view name) {
  auto target = id2name.find(ID);
  if (target != id2name.end()) {
    assert(target->second == name);
    return;
  } else
    target = id2name.emplace_hint(target, ID, name);
  target->second = name;

#ifndef NDEBUG
  unmanagedIDs.insert(ID);
#endif  // !NDEBUG
}

template <typename T>
void IDRegistry<T>::Register(T ID, std::string_view name) {
  auto target = id2name.find(ID);
  if (target != id2name.end()) {
    assert(target->second == name);
    return;
  }

  if (name.empty()) {
    id2name.emplace_hint(target, ID, std::string_view{});
    return;
  }

  assert(name.data());

  auto buffer =
      reinterpret_cast<char*>(resource.allocate(name.size(), alignof(char)));
  std::memcpy(buffer, name.data(), name.size());

  id2name.emplace_hint(target, ID, std::string_view{buffer, name.size()});

#ifndef NDEBUG
  unmanagedIDs.erase(ID);
#endif  // !NDEBUG
}

template <typename T>
void IDRegistry<T>::UnregisterUnmanaged(T ID) noexcept {
  auto target = id2name.find(ID);
  if (target == id2name.end())
    return;

  assert(IsUnmanaged(ID));

  id2name.erase(ID);
}

template <typename T>
void IDRegistry<T>::Clear() noexcept {
  id2name.clear();
#ifndef NDEBUG
  unmanagedIDs.clear();
#endif  // !NDEBUG
  resource.release();
}

#ifndef NDEBUG
template <typename T>
bool IDRegistry<T>::IsUnmanaged(T ID) const noexcept {
  return unmanagedIDs.find(ID) != unmanagedIDs.end();
}

template <typename T>
void IDRegistry<T>::ClearUnmanaged() noexcept {
  for (const auto& ID : unmanagedIDs)
    id2name.erase(ID);
  unmanagedIDs.clear();
}
#endif  // !NDEBUG

template <typename T>
bool IDRegistry<T>::IsRegistered(T ID) const noexcept {
  return id2name.find(ID) != id2name.end();
}

template <typename T>
std::string_view IDRegistry<T>::Nameof(T ID) const noexcept {
  auto target = id2name.find(ID);
  if (target != id2name.end())
    return target->second;
  else
    return {};
}
}  // namespace My::MyDRefl
