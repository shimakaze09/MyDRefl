//
// Created by Admin on 11/03/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <cassert>
#include <iostream>

using namespace My::MyDRefl;

struct Point {
  [[MyInspector::range(std::pair{0.f, 10.f})]]
  float x;
  float y;
};

int main() {
  size_t ID_Point = ReflMngr::Instance().registry.Register("Point");
  size_t ID_float = ReflMngr::Instance().registry.Register("float");
  size_t ID_x = ReflMngr::Instance().registry.Register("x");
  size_t ID_y = ReflMngr::Instance().registry.Register("y");
  size_t ID_MyInspector_range =
      ReflMngr::Instance().registry.Register("MyInspector_range");

  {  // register Point
    FieldPtr ptrX{ID_Point, ID_float, offsetof(Point, x)};
    FieldPtr ptrY{ID_Point, ID_float, offsetof(Point, y)};
    FieldInfo fieldinfoX{ptrX,
                         {// attrs
                          {ID_MyInspector_range, std::pair{0.f, 10.f}}}};
    FieldInfo fieldinfoY{ptrY};
    TypeInfo typeinfo{{},  // attrs
                      {    // fields
                       {ID_x, fieldinfoX},
                       {ID_y, fieldinfoY}}};
    ReflMngr::Instance().typeinfos.emplace(ID_Point, std::move(typeinfo));
  }

  Point p;
  ObjectPtr ptr{ID_Point, &p};
  ReflMngr::Instance()
      .typeinfos.at(ID_Point)
      .fieldinfos.at(ID_x)
      .fieldptr.Map(ptr)
      .As<float>() = 1.f;
  ReflMngr::Instance()
      .typeinfos.at(ID_Point)
      .fieldinfos.at(ID_y)
      .fieldptr.Map(ptr)
      .As<float>() = 2.f;

  for (const auto& [ID_field, fieldinfo] :
       ReflMngr::Instance().typeinfos.at(ID_Point).fieldinfos) {
    auto field = fieldinfo.fieldptr.Map(ptr);
    auto field_name = ReflMngr::Instance().registry.Nameof(ID_field);
    if (field.GetID() == ID_float) {
      std::cout << field_name << ": " << field.As<float>() << std::endl;
      if (fieldinfo.attrs.find(ID_MyInspector_range) != fieldinfo.attrs.end()) {
        const auto& r = std::any_cast<const std::pair<float, float>&>(
            fieldinfo.attrs.at(ID_MyInspector_range));
        std::cout << ReflMngr::Instance().registry.Nameof(ID_MyInspector_range)
                  << ": " << r.first << ", " << r.second << std::endl;
      }
    }
  }
}
