# Overview

Reflection is a fundamental language feature, and C++ lacks this capability in its core. MyDRefl is a dynamic reflection library that can represent C++ types almost perfectly at runtime, making it suitable for applications requiring reflection. In addition, users can register dynamic types without needing a C++ definition.

The library is divided into three main parts:

- Storing various information for the type: member variables, member functions, base classes, class name, size, etc.
- Registering type information
- Using type information: retrieving member variables, calling member functions, etc.

## 1. Storage

### 1.1 Member Variables

Member variables are also called fields. An object generally occupies a continuous block of memory, and a field can be accessed by an offset relative to that block. Therefore, an offset can be stored to represent field information. For more complex fields (not in a continuous space), an offset function like void*(void*) can be used (void is used for type erasure).

> Other categories of fields include:
> - Static fields: use a void* (a static field is a variable at a fixed position, so a pointer is sufficient)
> - Dynamic shared fields: “dynamic” here contrasts with “static.” A static field is part of the program itself, while a dynamic field is generated at runtime, allowing you to add variables similar to static fields dynamically. Because std::shared_ptr<void> is used, it's called “shared.”
> - Dynamic buffer fields: Similar to dynamic shared fields, but stored using std::aligned_storage_t<64> on the stack for better performance. Type erasure is used, so the type must be trivial (no special constructor or destructor, can be copied directly).

### 1.2 Member Functions

A member function can be represented by a function pointer. For type erasure, this library uses void(void* obj, void* result_buffer, ArgsView args) as a unified function signature and stores it in a std::function<void(void*,void*,ArgsView)>.

### 1.3 Base Classes

A class can have one or more base classes. Typically, a base class resides in the same continuous block of memory, so obtaining a base class pointer from an object pointer involves an offset. However, because of virtual base classes, a more general approach is taken by using an offset function void*(void*).

### 1.4 Classes

Class information consists of a table of member variables (name -> variable), a table of member functions (name -> function), a base class table (class name -> base class), the class name, size, etc.

## 2. Registration

Registration can be done fully dynamically (no need for C++ type definitions).

It can also leverage C++ types and template metaprogramming to significantly simplify the process (for example, providing a member variable pointer and a name is enough to register a field).

## 3. Usage

Essentially, the dynamic library can do almost anything that C++ can do. All interfaces reside in My::MyDRefl::ReflMngr. For convenience, an ObjectView is provided, containing some ReflMngr interfaces. ObjectView itself does not manage object memory, so it is called a “view.” For memory management, there is SharedObject, which manages object memory.

Some basic operations are described in README.md.

Additional details:
- Retrieving a member variable will search the inheritance hierarchy.
- Calling a member function will search the inheritance hierarchy, decide on an overloaded function by parameter types, and can convert arguments dynamically during the call (e.g., if the function requires an int but a float is provided, the library will convert the parameter).

## 4. Extension

### 4.1 Bootstrapping

We have registered most of the library’s types into the dynamic reflection library, completing its bootstrapping process.

Enable the `My_MyDRefl_Build_ext_Bootstrap` option in CMake to build the related project.