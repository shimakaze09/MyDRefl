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
- manual (TODO)
- [overview](doc/overview.md)
- [introduction](doc/introduction.md)

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
Mngr.RegisterType<Vec>();
Mngr.AddField<&Vec::x>("x");
Mngr.AddField<&Vec::y>("y");
Mngr.AddMethod<&Vec::norm>("norm");
```

### Iterate over members

```c++
for (auto&& field : Mngr.GetFields(Type_of<Vec>))
  std::cout << field.name.GetView() << std::endl;

for (auto&& method : Mngr.GetMethods(Type_of<Vec>))
  std::cout << method.name.GetView() << std::endl;
```

### Constructing types

```c++
SharedObject v = Mngr.MakeShared(Type_of<Vec>);
std::cout << v.GetType().GetName() << std::endl; // prints "Vec"
```

### Set/get variables

```c++
v.Var("x") = 3;
v.Var("y") = 4;
std::cout << "x: " << v.Var("x") << std::endl;
```

### Invoke Methods

```c++
std::cout << "norm: " << v.Invoke("norm") << std::endl;
```

### Iterate over variables

```c++
for (auto&& [type, field, var] : v.GetTypeFieldVars())
  std::cout << field.name.GetView() << ": " << var << std::endl;
```

### other example

- [const & static](src/test/02_const_static/main.cpp)
- [method](src/test/03_method/main.cpp)
- [enum](src/test/04_enum/main.cpp)
- [overload](src/test/05_overload/main.cpp)
- [inheritance](src/test/06_inheritance/main.cpp)
- [virtual inheritance](src/test/07_virtual/main.cpp)
- [attr](src/test/08_attr/main.cpp)
- [lifetime (ctor, dtor)](src/test/09_lifecycle/main.cpp)
- [dynamic field](src/test/10_dynamic/main.cpp)
- [invoke](src/test/11_invoke/main.cpp)
- [meta function](src/test/12_Meta/main.cpp)
- [reference](src/test/13_ref/main.cpp)
- [serialize](src/test/15_serializer/main.cpp)
- [container](src/test/16_container/main.cpp)
- [compatible](src/test/17_compatible/main.cpp)
- [tuple](src/test/18_tuple/main.cpp)
- [pointer](src/test/19_pointer/main.cpp)
- [array](src/test/20_array/main.cpp)
- [string](src/test/21_string/main.cpp)
- [variant](src/test/22_variant/main.cpp)
- [optional](src/test/23_optional/main.cpp)
- [bootstrap](src/test/ext/00_bootstrap/main.cpp)
- [[data-driven] `RegisterType`](src/test/24_dd_type/main.cpp)

## Features

- global fields, methods or enums
- classes with **single**-, **multiple**- and **virtual**-inheritance
- constructors (arbitrary argument count) and destructors
- methods (**virtual**, **abstract**, **overloaded**, arbitrary argument count) : you can pass arguments by a buffer (on
  stack or heap)
- ability to invoke methods of classes from any arbitrary class level
- argument type auto **conversion** when invoking method
- no header pollution: the reflection information is created in the cpp file to minimize compile time when modifying the
  data
- working with custom types without the need of having the declaration of the type available at compile time (useful for
  plugins)
- possibility to add additional **attribute** to all reflection objects
- reference
- pointer, array
- standard container, iterator
- **meta** function
    - operations: `operator +`, `operator-`, ...
    - container: `begin`, `end`, `empty`, `size`, ...
- bootstrap
- **no** macro usage
- **no** rtti required
- **no** exceptions (this feature come with cost and is also regularly disabled on consoles)
- **no** external compiler or tool needed, only standard ISO C++20

## Compiler compatibility

- Clang/LLVM >= 10.0
- GCC >= 10.0
- MSVC >= 1926

> Tested platforms:
 >
 > - Windows 10: VS2019 16.8.5
 >
 > - Ubuntu 20: GCC 10.2, Clang 11.0
 >
 > - MacOS 11.0 : GCC 10.2
 >
 >   > AppleClang 12 and Clang 11 is not supported