```

 __  __       ____  ____       __ _ 
|  \/  |_   _|  _ \|  _ \ ___ / _| |
| |\/| | | | | | | | |_) / _ \ |_| |
| |  | | |_| | |_| |  _ <  __/  _| |
|_|  |_|\__, |____/|_| \_\___|_| |_|
        |___/                       
                                                             

```

[![repo-size](https://img.shields.io/github/languages/code-size/shimakaze09/MyDRefl?style=flat)](https://github.com/shimakaze09/MyDRefl/archive/master.zip) [![tag](https://img.shields.io/github/v/tag/shimakaze09/MyDRefl)](https://github.com/shimakaze09/MyDRefl/tags) [![license](https://img.shields.io/github/license/shimakaze09/MyDRefl)](LICENSE)

# MyDRefl

> **My** **D**ynamic **R**eflection

## Feature

## Example

```c++
#include <MyDRefl/MyDRefl.h>
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
  size_t ID_MyInspector_range = ReflMngr::Instance().registry.Register("MyInspector_range");
  
  { // register Point
    TypeInfo typeinfo{
      {}, // attrs
      { // fields
        { ID_x, { { ID_float, offsetof(Point, x) }, { // attrs
          {ID_MyInspector_range, std::pair{0.f, 10.f}}
        }}},
        { ID_y, { { ID_float, offsetof(Point, y) } }}
      }
    };
    ReflMngr::Instance().typeinfos.emplace(ID_Point, std::move(typeinfo));
  }
  
  Point p;
  ObjectPtr ptr{ ID_Point, &p };
  ReflMngr::Instance().RWField(ptr, ID_x).As<float>() = 1.f;
  ReflMngr::Instance().RWField(ptr, ID_y).As<float>() = 2.f;
  
  ReflMngr::Instance().ForEachRField(
    ptr,
    [](size_t typeID, const TypeInfo& typeinfo, size_t fieldID, const FieldInfo& fieldinfo, ConstObjectPtr field) {
      for (const auto& [attrID, attr] : fieldinfo.attrs) {
        if (attrID == ReflMngr::Instance().registry.GetID("MyInspector_range")) {
          auto range = std::any_cast<std::pair<float, float>>(attr);
          std::cout
            << "[MyInspector_range]" << " "
            << range.first << ", " << range.second
            << std::endl;
        }
      }
      std::cout
        << ReflMngr::Instance().registry.Nameof(fieldID)
        << ": " << field.As<float>()
        << std::endl;
    }
  );
}
```

result is

```
[MyInspector_range] 0, 10
x: 1
y: 2
```

## Documentation

## Integration

## Compiler compatibility

## TODO

- [x] virtual
- [x] return
- [x] foreach
- [ ] life cycle (ctor, dtor, copy, etc.)
- [ ] global