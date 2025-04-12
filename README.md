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

Extremely **fast** and **powerful** C++20 dynamic reflection library

## Documentation [->](doc)

- [to-do](doc/todo.md)
- [change log](doc/change_log.md)

## How to Use

> the example code is [here](src/test/00_readme/main.cpp), other examples
> is [here](https://github.com/shimakaze09/MyDRefl#other-example)

Suppose you need to reflect `struct Vec`

```c++
struct Vec {
  float x;
  float y;
  float norm() const {
    return std::sqrt(x * x + y * y);
  }
};
```

### Manual registration

```c++
Mngr.RegisterTypePro<Vec>();
Mngr.AddField<&Vec::x>("x");
Mngr.AddField<&Vec::y>("y");
Mngr.AddMethod<&Vec::norm>("norm");
```

### Iterate over members

```c++
for (auto field : Mngr.GetFields(TypeID_of<Vec>))
  std::cout << Mngr.nregistry.Nameof(field.ID) << std::endl;

for (auto method : Mngr.GetMethods(TypeID_of<Vec>))
  std::cout << Mngr.nregistry.Nameof(method.ID) << std::endl;
```

### Constructing types

```c++
SharedObject v = Mngr.MakeShared(TypeID_of<Vec>);
std::cout << v.TypeName() << std::endl; // prints "Vec"
```

### Set/get variables

```c++
v.Var("x") = 3;
v.Var("y") = 4;
std::cout << "x: " << v.Var("x") << std::endl;
```

### Invoke Methods

```c++
std::cout << "norm: " << v.DMInvoke("norm") << std::endl;
```

### Iterate over variables

```c++
for (const auto& [type, field, var] : v.GetTypeFieldRVars()) {
  std::cout
    << Mngr.nregistry.Nameof(field.ID)
    << ": " << var
    << std::endl;
}
```

### other example

- [const & static](src/test/02_const_static/main.cpp)
- [method](src/test/03_method/main.cpp)
- [enum](src/test/04_enum/main.cpp)
- [overload](src/test/05_overload/main.cpp)
- [inheritance](src/test/06_inheritance/main.cpp)
- [virtual inheritance](src/test/07_virtual/main.cpp)
- [attr](src/test/08_attr/main.cpp)
- [lifecycle (malloc, free, ctor, dtor)](src/test/09_lifecycle/main.cpp)
- [dynamic field](src/test/10_dynamic/main.cpp)
- [invoke](src/test/11_invoke/main.cpp)
- [meta function](src/test/12_Meta/main.cpp)
- [reference](src/test/13_ref/main.cpp)
- [serialize](src/test/15_serializer/main.cpp)
- [container](src/test/16_container/main.cpp)

## Feature

- reflect global fields, methods or enums
- classes with **single**-, **multiple**- and **virtual**-inheritance
- constructors (arbitrary argument count) and destructors
- methods (**virtual**, **abstract**, **overloaded**, arbitrary argument count) : you can pass arguments by a buffer (on
  stack or heap)
- ability to invoke methods of classes from any arbitrary class level
- no header pollution: the reflection information is created in the cpp file to minimize compile time when modifying the
  data
- working with custom types without the need of having the declaration of the type available at compile time (useful for
  plugins)
- possibility to add additional **attribute** to all reflection objects
- reference/pointer
- standard container
- **meta** function: `operator +`, `operator-`, etc
- **no** macro usage
- **no** rtti required
- **no** exceptions (this feature come with cost and is also regularly disabled on consoles)
- **no** external compiler or tool needed, only standard ISO C++20

## Compiler compatibility

- Clang/LLVM >= 10.0
- GCC >= 10.0
- MSVC >= 1926