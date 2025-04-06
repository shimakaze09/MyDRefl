//
// Created by Admin on 5/04/2025.
//

#include <MyDRefl/MyDRefl.h>
#include <iostream>

#include "Vector.h"

using namespace My;
using namespace My::MyDRefl;

void Serializer(ConstObjectPtr obj) {
  if (type_name_is_arithmetic(obj->TypeName()))
    std::cout << obj;
  else {
    std::cout << "{" << "\"TYPE\":\"" << obj->TypeName() << "\",";
    auto iter = obj->GetType()->attrs.find(TypeID::of<ContainerType>);
    if (iter != obj->GetType()->attrs.end()) {
      if (*iter == ContainerType::VECTOR) {
        std::cout << "\"DATA\":[";
        for (size_t i = 0; i < obj->size(); i++) {
          Serializer(obj[i]->DereferenceAsConst());
          if (i + 1 != obj->size())
            std::cout << ",";
        }
        std::cout << "]";
      }
    } else {  // normal object
      size_t N = obj->GetRVars().size();
      size_t i = 0;
      obj->ForEachRVar(
          [N, &i](TypeRef type, FieldRef field, ConstObjectPtr var) {
            std::cout << "\"" << Mngr->nregistry.Nameof(field.ID) << "\":";
            Serializer(var);
            if (++i != N)
              std::cout << ",";
            return true;
          });
    }
    std::cout << "}";
  }
}

int main() {
  RegisterVector();

  Vector a;

  for (size_t i = 0; i < 10; i++) {
    std::vector<size_t> row;
    for (size_t j = 0; j < 10; j++)
      row.push_back(j);
    a.data.push_back(std::move(row));
  }

  Serializer(Ptr(a));
}
