//
// Created by Admin on 31/03/2025.
//

#pragma once
 
 #include <string>
 #include <vector>
 #include <unordered_map>
 
 namespace My::MyDRefl {
 class NameRegistry {
 public:
   static NameRegistry& Instance() noexcept {
     static NameRegistry instance;
     return instance;
   }
 
   size_t Register(std::string_view name);
   size_t GetID(std::string_view name) const noexcept;
   bool IsRegistered(size_t ID) const noexcept { return ID < names.size(); }
   const auto& GetNameIDMap() const noexcept { return name2id; }
   std::string_view Nameof(size_t ID) const noexcept;
 
 private:
   std::vector<std::string> names;
   std::unordered_map<std::string_view, size_t> name2id;
 };
 }
