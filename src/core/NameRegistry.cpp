//
// Created by Admin on 31/03/2025.
//

#include <MyDRefl/NameRegistry.h>
 
 #include <cassert>
 
 using namespace My::MyDRefl;
 
size_t NameRegistry::Register(std::string_view name) {
  assert(!name.empty());
 
  auto target = name2id.find(name);
  if (target != name2id.end())
    return target->second;
 
  size_t idx = names.size();
  names.push_back(std::string{ name });
  name2id.emplace_hint(target, std::pair{ std::string_view{names[idx]}, idx });
 
  return idx;
}
 
size_t NameRegistry::GetID(std::string_view name) const noexcept {
  auto target = name2id.find(name);
  return target == name2id.end() ? static_cast<size_t>(-1) : target->second;
}
 
std::string_view NameRegistry::Nameof(size_t ID) const noexcept
{
  if (IsRegistered(ID))
    return names[ID];
  else
    return {};
}
