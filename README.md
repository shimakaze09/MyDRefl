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
  float x;
  float y;
};

int main() {
  TypeID ID_Point = ReflMngr::Instance().tregistry.Register("Point");
  TypeID ID_float = ReflMngr::Instance().tregistry.Register("float");
  
  NameID ID_x = ReflMngr::Instance().nregistry.Register("x");
  NameID ID_y = ReflMngr::Instance().nregistry.Register("y");
  
  { // register Point
    TypeInfo typeinfo{{
        { ID_x, { { ID_float, offsetof(Point, x) } }},
        { ID_y, { { ID_float, offsetof(Point, y) } }}
    }};
    ReflMngr::Instance().typeinfos.emplace(ID_Point, std::move(typeinfo));
  }
  
  Point p;
  ObjectPtr ptr{ ID_Point, &p };
  ReflMngr::Instance().RWField(ptr, ID_x).As<float>() = 1.f;
  ReflMngr::Instance().RWField(ptr, ID_y).As<float>() = 2.f;
  
  ReflMngr::Instance().ForEachRField(
    ptr,
    [](TypeID typeID, const TypeInfo& typeinfo, NameID fieldID, const FieldInfo& fieldinfo, ConstObjectPtr field) {
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
- [ ] attr
- [ ] life cycle (ctor, dtor, copy, etc.)
- [ ] global