//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <array>
#include <iostream>

using namespace My::MyDRefl;

struct Vec {
  float x;
  float y;

  float Norm2() const noexcept { return x * x + y * y; }

  void NormalizeSelf() noexcept {
    float n = std::sqrt(Norm2());
    assert(n != 0);
    x /= n;
    y /= n;
  }

  Vec& operator+=(const Vec& p) noexcept {
    x += p.x;
    y += p.y;
    return *this;
  }
};

int main() {
  size_t ID_Vec = ReflMngr::Instance().registry.Register("Vec");
  size_t ID_const_Vec_ptr =
      ReflMngr::Instance().registry.Register("const Vec*");
  size_t ID_Vec_ptr = ReflMngr::Instance().registry.Register("Vec*");
  size_t ID_p = ReflMngr::Instance().registry.Register("p");
  size_t ID_float = ReflMngr::Instance().registry.Register("float");
  size_t ID_x = ReflMngr::Instance().registry.Register("x");
  size_t ID_y = ReflMngr::Instance().registry.Register("y");
  size_t ID_Norm2 = ReflMngr::Instance().registry.Register("Norm2");
  size_t ID_NormalizeSelf =
      ReflMngr::Instance().registry.Register("NormalizeSelf");
  size_t ID_operator_add_assign =
      ReflMngr::Instance().registry.Register("operator+=");

  {  // register Vec
    FieldPtr ptr_x{ID_float, offsetof(Vec, x)};
    FieldPtr ptr_y{ID_float, offsetof(Vec, y)};

    auto Norm2 = [](const void* obj, ArgsView, void* result_buffer) {
      *reinterpret_cast<float*>(result_buffer) =
          reinterpret_cast<const Vec*>(obj)->Norm2();
      return destructor<float>();
    };
    Method method_Norm2{Norm2, {}, {ID_float, sizeof(float), alignof(float)}};

    auto NormalizeSelf = [](void* obj, ArgsView, void*) {
      reinterpret_cast<Vec*>(obj)->NormalizeSelf();
      return destructor<void>();
    };
    Method method_NormalizeSelf{NormalizeSelf};

    auto operator_add_assign = [](void* obj, ArgsView args,
                                  void* result_buffer) {
      auto v = reinterpret_cast<Vec*>(obj);
      *reinterpret_cast<Vec**>(result_buffer) =
          &(*v += *args.At(0).As<const Vec*>());
      return destructor<Vec*>();
    };
    Method method_operator_add_assign{
        operator_add_assign,
        {{{ID_const_Vec_ptr, sizeof(const Vec*), alignof(const Vec*), ID_p}}},
        {ID_Vec_ptr, sizeof(Vec*), alignof(Vec*)}};

    FieldInfo fieldinfo_x{ptr_x};
    FieldInfo fieldinfo_y{ptr_y};
    MethodInfo methodinfo_NormalizeSelf{method_NormalizeSelf};
    MethodInfo methodinfo_operator_add_assign{method_operator_add_assign};
    MethodInfo methodinfo_Norm2{method_Norm2};

    TypeInfo typeinfo{
        {},  // attrs
        {    // fields
         {ID_x, fieldinfo_x},
         {ID_y, fieldinfo_y}},
        {// methods
         {ID_NormalizeSelf, methodinfo_NormalizeSelf},
         {ID_operator_add_assign, methodinfo_operator_add_assign},
         {ID_Norm2, methodinfo_Norm2}},
    };
    ReflMngr::Instance().typeinfos.emplace(ID_Vec, std::move(typeinfo));
  }

  Vec v{2.f, 3.f};
  ObjectPtr ptr{ID_Vec, &v};

  ReflMngr::Instance().Invoke(ptr, ID_NormalizeSelf, {}, nullptr, nullptr);
  std::cout << v.x << ", " << v.y << std::endl;

  {
    std::uint8_t result_buffer[sizeof(float)];
    auto [success, typeID, destructor] =
        ReflMngr::Instance().Invoke(ptr, ID_Norm2, {}, nullptr, result_buffer);
    assert(success);
    assert(typeID == ID_float);
    assert(!destructor);
    std::cout << *reinterpret_cast<float*>(result_buffer) << std::endl;
  }

  {
    Vec w{10.f, 10.f};
    std::uint8_t args_buffer[sizeof(const Vec*)];
    std::uint8_t result_buffer[sizeof(Vec*)];
    *reinterpret_cast<const Vec**>(args_buffer) = &w;
    auto [success, typeID, destructor] = ReflMngr::Instance().Invoke(
        ptr, ID_operator_add_assign, std::array{ID_const_Vec_ptr}, args_buffer,
        result_buffer);
    assert(success);
    assert(typeID == ID_Vec_ptr);
    assert(!destructor);
    auto pw = *reinterpret_cast<Vec**>(result_buffer);
    std::cout << pw->x << ", " << pw->y << std::endl;
  }

  return 0;
}
