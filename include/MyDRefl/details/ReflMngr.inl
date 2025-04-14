#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

namespace My::MyDRefl::details {
template <typename ArgList>
struct GenerateMethodPtr_Helper;

template <typename... Args>
struct GenerateMethodPtr_Helper<TypeList<Args...>> {
  static ParamList GenerateParamList() noexcept(sizeof...(Args) == 0) {
    return ReflMngr::GenerateParamList<Args...>();
  }

  template <auto funcptr, size_t... Ns>
  static constexpr auto GenerateFunction(std::index_sequence<Ns...>) noexcept {
    using FuncPtr = decltype(funcptr);
    using Traits = FuncTraits<decltype(funcptr)>;
    if constexpr (std::is_member_function_pointer_v<FuncPtr>) {
      constexpr auto wrapped_func = [](void* obj, void* result_buffer,
                                       ArgsView args) {
        assert(((args.GetParamList()[Ns] == Type_of<Args>) && ...));
        constexpr auto f = wrap_function<funcptr>();
        f(obj, result_buffer, args.GetBuffer());
      };
      constexpr auto decayed_wrapped_func = DecayLambda(wrapped_func);
      return decayed_wrapped_func;
    } else if constexpr (is_function_pointer_v<FuncPtr>) {
      constexpr auto wrapped_func = [](void*, void* result_buffer,
                                       ArgsView args) {
        assert(((args.GetParamList()[Ns] == Type_of<Args>) && ...));
        constexpr auto f = wrap_function<funcptr>();
        f(nullptr, result_buffer, args.GetBuffer());
      };
      constexpr auto decayed_wrapped_func = DecayLambda(wrapped_func);
      decayed_wrapped_func;
    } else
      static_assert(always_false<FuncPtr>);
  }

  template <typename Func, size_t... Ns>
  static /*constexpr*/ auto GenerateMemberFunction(
      Func&& func, std::index_sequence<Ns...>) noexcept {
    using Traits = WrapFuncTraits<std::decay_t<Func>>;
    /*constexpr*/ auto wrapped_func =
        [wrapped_f = wrap_member_function(std::forward<Func>(func))](
            void* obj, void* result_buffer, ArgsView args) mutable {
          assert(((args.GetParamList()[Ns] == Type_of<Args>) && ...));
          wrapped_f(obj, result_buffer, args.GetBuffer());
        };

    return std::function{wrapped_func};
  }

  template <typename Func, size_t... Ns>
  static /*constexpr*/ auto GenerateStaticFunction(
      Func&& func, std::index_sequence<Ns...>) noexcept {
    /*constexpr*/ auto wrapped_func =
        [wrapped_f = wrap_static_function(std::forward<Func>(func))](
            void*, void* result_buffer, ArgsView args) mutable {
          assert(((args.GetParamList()[Ns] == Type_of<Args>) && ...));
          wrapped_f(nullptr, result_buffer, args.GetBuffer());
        };
    return std::function{wrapped_func};
  }
};

template <typename T, typename FixedArgList, typename... LeftArgs>
struct register_ctor_impl;

template <typename T, typename... FixedArgs>
struct register_ctor_impl<T, TypeList<FixedArgs...>> {
  static void run(ReflMngr& mngr) {
    if constexpr (type_ctor<T, FixedArgs...>)
      mngr.AddConstructor<T, FixedArgs...>();
  }
};

template <typename T, typename... FixedArgs, typename LeftArg0,
          typename... LeftArgs>
struct register_ctor_impl<T, TypeList<FixedArgs...>, LeftArg0, LeftArgs...> {
  static void run(ReflMngr& mngr) {
    register_ctor_impl<T, TypeList<FixedArgs..., const LeftArg0&>,
                       LeftArgs...>::run(mngr);
    if constexpr (!std::is_fundamental_v<LeftArg0>)
      register_ctor_impl<T, TypeList<FixedArgs..., LeftArg0&&>,
                         LeftArgs...>::run(mngr);
  }
};

template <typename T, typename... Args>
void register_ctor(ReflMngr& mngr) {
  register_ctor_impl<T, TypeList<>, Args...>::run(mngr);
}

template <template <typename> class get_size, std::size_t TargetIdx, typename T>
ObjectView runtime_get_impl(T&& obj, std::size_t i) {
  using U = std::remove_cvref_t<T>;
  if constexpr (TargetIdx == get_size<U>::value)
    return nullptr;
  else {
    if (i == TargetIdx)
      return ObjectView{std::get<TargetIdx>(std::forward<T>(obj))};
    else
      return runtime_get_impl<get_size, TargetIdx + 1>(std::forward<T>(obj), i);
  }
}

template <template <typename> class get_size, typename T>
ObjectView runtime_get(T&& obj, std::size_t i) {
  using U = std::remove_cvref_t<T>;
  assert(i < get_size<U>::value);
  return runtime_get_impl<get_size, 0>(std::forward<T>(obj), i);
}

template <template <typename> class get_size,
          template <std::size_t, typename> class get_type,
          std::size_t TargetIdx, typename T>
ObjectView runtime_get_impl(T&& obj, const Type& type) {
  using U = std::remove_cvref_t<T>;
  if constexpr (TargetIdx == get_size<U>::value)
    return nullptr;
  else {
    if (type == Type_of<typename get_type<TargetIdx, U>::type>)
      return ObjectView{std::get<TargetIdx>(std::forward<T>(obj))};
    else
      return runtime_get_impl<get_size, get_type, TargetIdx + 1>(
          std::forward<T>(obj), type);
  }
}

template <template <typename> class get_size,
          template <std::size_t, typename> class get_type, typename T>
ObjectView runtime_get(T&& obj, const Type& type) {
  return runtime_get_impl<get_size, get_type, 0>(std::forward<T>(obj), type);
}

template <std::size_t TargetIdx, typename T>
Type runtime_tuple_element_impl(std::size_t i) noexcept {
  if constexpr (TargetIdx == std::tuple_size_v<T>)
    return {};
  else {
    if (i == TargetIdx)
      return Type_of<std::tuple_element_t<TargetIdx, T>>;
    else
      return runtime_tuple_element_impl<TargetIdx + 1, T>(i);
  }
}

template <typename T>
Type runtime_tuple_element(std::size_t i) noexcept {
  assert(i < std::tuple_size_v<T>);
  return runtime_tuple_element_impl<0, T>(i);
}

template <typename T, std::size_t... Ns>
void register_tuple_elements(ReflMngr& mngr, std::index_sequence<Ns...>) {
  (mngr.RegisterType<std::tuple_element_t<Ns, T>>(), ...);
  register_ctor<T, std::tuple_element_t<Ns, T>...>(mngr);
}

template <std::size_t TargetIdx, typename T>
bool runtime_variant_holds_alternative_impl(const T& obj,
                                            const Type& type) noexcept {
  if constexpr (TargetIdx == std::variant_size_v<T>)
    return false;
  else {
    using Elem = std::variant_alternative_t<TargetIdx, T>;
    if (type == Type_of<Elem>)
      return std::holds_alternative<Elem>(obj);
    else
      return runtime_variant_holds_alternative_impl<TargetIdx + 1>(obj, type);
  }
}

template <typename T>
bool runtime_variant_holds_alternative(const T& obj,
                                       const Type& type) noexcept {
  return runtime_variant_holds_alternative_impl<0>(obj, type);
}

template <std::size_t TargetIdx, typename T>
Type runtime_variant_alternative_impl(std::size_t i) {
  if constexpr (TargetIdx == std::variant_size_v<T>)
    return {};
  else {
    if (i == TargetIdx)
      return Type_of<std::variant_alternative_t<TargetIdx, T>>;
    else
      return runtime_variant_alternative_impl<TargetIdx + 1, T>(i);
  }
}

template <typename T>
Type runtime_variant_alternative(std::size_t i) {
  assert(i < std::variant_size_v<T>);
  return runtime_variant_alternative_impl<0, T>(i);
}

template <typename T, std::size_t Idx>
void register_variant_ctor_assign(ReflMngr& mngr) {
  using Elem = std::variant_alternative_t<Idx, T>;
  if constexpr (type_ctor<T, const Elem&>)
    mngr.AddConstructor<T, const Elem&>();
  if constexpr (operator_assign<T, const Elem&>)
    mngr.AddMemberMethod(NameIDRegistry::Meta::container_assign,
                         [](T& t, const Elem& elem) -> T& { return t = elem; });

  if constexpr (!std::is_fundamental_v<Elem>) {
    if constexpr (type_ctor<T, Elem&&>)
      mngr.AddConstructor<T, Elem&&>();
    if constexpr (operator_assign<T, Elem&&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_assign,
          [](T& t, Elem&& elem) -> T& { return t = std::move(elem); });
  }
}

template <typename T, std::size_t... Ns>
void register_variant_alternatives(ReflMngr& mngr, std::index_sequence<Ns...>) {
  (mngr.RegisterType<std::variant_alternative_t<Ns, T>>(), ...);
  (register_variant_ctor_assign<T, Ns>(mngr), ...);
}

template <typename T>
struct TypeAutoRegister_Default {
  static void run(ReflMngr& mngr) {
    if constexpr (std::is_default_constructible_v<T>)
      mngr.AddConstructor<T>();
    if constexpr (type_ctor_copy<T>)
      mngr.AddConstructor<T, const T&>();
    if constexpr (type_ctor_move<T>)
      mngr.AddConstructor<T, T&&>();
    if constexpr (std::is_destructible_v<T> &&
                  !std::is_trivially_destructible_v<T>)
      mngr.AddDestructor<T>();

    if constexpr (std::is_pointer_v<T>) {
      mngr.RegisterType<std::remove_pointer_t<T>>();
      if constexpr (std::is_const_v<std::remove_pointer_t<T>>)
        mngr.AddConstructor<T, const std::add_pointer_t<std::remove_const_t<
                                   std::remove_pointer_t<T>>>&>();
    }

    if constexpr (operator_bool<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_bool,
                           [](const T& obj) { return static_cast<bool>(obj); });

    if constexpr (operator_plus<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_plus,
                           [](const T& lhs) { return +lhs; });
    if constexpr (operator_minus<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_minus,
                           [](const T& lhs) { return -lhs; });

    if constexpr (std::is_array_v<T> && std::rank_v<T> == 0) {
      using Ele = std::remove_extent_t<T>;
      mngr.AddConstructor<T, const std::add_pointer_t<Ele>&>();
      if constexpr (std::is_const_v<Ele>)
        mngr.AddConstructor<
            T, const std::add_pointer_t<std::remove_const_t<Ele>>&>();
    }

    if constexpr (operator_add<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_add,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs + rhs;
                           });
    if constexpr (operator_sub<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_sub,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs - rhs;
                           });
    if constexpr (operator_mul<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_mul,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs * rhs;
                           });
    if constexpr (operator_div<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_div,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs / rhs;
                           });
    if constexpr (operator_mod<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_mod,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs % rhs;
                           });

    if constexpr (operator_bnot<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_bnot,
                           [](const T& lhs) -> decltype(auto) { return ~lhs; });
    if constexpr (operator_band<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_band,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs & rhs;
                           });
    if constexpr (operator_bor<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_bor,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs & rhs;
                           });
    if constexpr (operator_bxor<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_bxor,
                           [](const T& lhs, const T& rhs) -> decltype(auto) {
                             return lhs & rhs;
                           });
    if constexpr (operator_lshift<const T&, const std::size_t&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_lshift,
          [](const T& lhs, const std::size_t& rhs) -> decltype(auto) {
            return lhs << rhs;
          });
    if constexpr (operator_rshift<const T&, const std::size_t&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_rshift,
          [](const T& lhs, const std::size_t& rhs) -> decltype(auto) {
            return lhs >> rhs;
          });

    if constexpr (operator_rshift<std::istream&, T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_lshift,
                           [](T& lhs, std::istream& rhs) -> decltype(auto) {
                             return rhs >> lhs;
                           });
    if constexpr (operator_rshift<std::istringstream&, T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_lshift,
          [](T& lhs, std::istringstream& rhs) -> decltype(auto) {
            return rhs >> lhs;
          });
    if constexpr (operator_rshift<std::ifstream&, T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_lshift,
                           [](T& lhs, std::ifstream& rhs) -> decltype(auto) {
                             return rhs >> lhs;
                           });
    if constexpr (operator_rshift<std::iostream&, T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_lshift,
                           [](T& lhs, std::iostream& rhs) -> decltype(auto) {
                             return rhs >> lhs;
                           });
    if constexpr (operator_rshift<std::stringstream&, T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_lshift,
          [](T& lhs, std::stringstream& rhs) -> decltype(auto) {
            return rhs >> lhs;
          });
    if constexpr (operator_rshift<std::fstream&, T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_lshift,
                           [](T& lhs, std::fstream& rhs) -> decltype(auto) {
                             return rhs >> lhs;
                           });

    if constexpr (operator_lshift<std::ostream&, const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_rshift,
          [](const T& lhs, std::ostream& rhs) -> decltype(auto) {
            return rhs << lhs;
          });
    if constexpr (operator_lshift<std::ostringstream&, const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_rshift,
          [](const T& lhs, std::ostringstream& rhs) -> decltype(auto) {
            return rhs << lhs;
          });
    if constexpr (operator_lshift<std::ofstream&, const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_rshift,
          [](const T& lhs, std::ofstream& rhs) -> decltype(auto) {
            return rhs << lhs;
          });
    if constexpr (operator_lshift<std::iostream&, const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_rshift,
          [](const T& lhs, std::iostream& rhs) -> decltype(auto) {
            return rhs << lhs;
          });
    if constexpr (operator_lshift<std::stringstream&, const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_rshift,
          [](const T& lhs, std::stringstream& rhs) -> decltype(auto) {
            return rhs << lhs;
          });
    if constexpr (operator_lshift<std::fstream&, const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_rshift,
          [](const T& lhs, std::fstream& rhs) -> decltype(auto) {
            return rhs << lhs;
          });

    if constexpr (operator_pre_inc<T&>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_pre_inc,
                           [](T& lhs) -> decltype(auto) { return ++lhs; });
    if constexpr (operator_post_inc<T&>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_post_inc,
                           [](T& lhs, int) -> decltype(auto) { return lhs++; });
    if constexpr (operator_pre_dec<T&>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_pre_dec,
                           [](T& lhs) -> decltype(auto) { return --lhs; });
    if constexpr (operator_post_dec<T&>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_post_dec,
                           [](T& lhs, int) -> decltype(auto) { return lhs--; });

    if constexpr (operator_assign_copy<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign,
          [](T& lhs, const T& rhs) -> T& { return lhs = rhs; });
    if constexpr (operator_assign_move<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign,
          [](T& lhs, T&& rhs) -> T& { return lhs = std::move(rhs); });
    if constexpr (operator_assign_add<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_add,
          [](T& lhs, const T& rhs) -> T& { return lhs += rhs; });
    if constexpr (operator_assign_sub<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_sub,
          [](T& lhs, const T& rhs) -> T& { return lhs -= rhs; });
    if constexpr (operator_assign_mul<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_mul,
          [](T& lhs, const T& rhs) -> T& { return lhs *= rhs; });
    if constexpr (operator_assign_div<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_div,
          [](T& lhs, const T& rhs) -> T& { return lhs /= rhs; });
    if constexpr (operator_assign_mod<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_mod,
          [](T& lhs, const T& rhs) -> T& { return lhs %= rhs; });
    if constexpr (operator_assign_band<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_band,
          [](T& lhs, const T& rhs) -> T& { return lhs &= rhs; });
    if constexpr (operator_assign_bor<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_bor,
          [](T& lhs, const T& rhs) -> T& { return lhs |= rhs; });
    if constexpr (operator_assign_bxor<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_bxor,
          [](T& lhs, const T& rhs) -> T& { return lhs ^= rhs; });
    if constexpr (operator_assign_lshift<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_lshift,
          [](T& lhs, const T& rhs) -> T& { return lhs <<= rhs; });
    if constexpr (operator_assign_lshift<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_assign_rshift,
          [](T& lhs, const T& rhs) -> T& { return lhs >>= rhs; });

    if constexpr (!IsContainerType<T> && operator_eq<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_eq,
                           [](const T& lhs, const T& rhs) {
                             return static_cast<bool>(lhs == rhs);
                           });
    if constexpr (!IsContainerType<T> && operator_ne<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_ne,
                           [](const T& lhs, const T& rhs) {
                             return static_cast<bool>(lhs != rhs);
                           });
    if constexpr (!IsContainerType<T> && operator_lt<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_lt,
                           [](const T& lhs, const T& rhs) {
                             return static_cast<bool>(lhs < rhs);
                           });
    if constexpr (!IsContainerType<T> && operator_gt<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_gt,
                           [](const T& lhs, const T& rhs) {
                             return static_cast<bool>(lhs > rhs);
                           });
    if constexpr (!IsContainerType<T> && operator_le<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_le,
                           [](const T& lhs, const T& rhs) {
                             return static_cast<bool>(lhs <= rhs);
                           });
    if constexpr (!IsContainerType<T> && operator_ge<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_ge,
                           [](const T& lhs, const T& rhs) {
                             return static_cast<bool>(lhs >= rhs);
                           });

    if constexpr (operator_subscript<T, const std::size_t>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_subscript,
          [](T& lhs, const std::size_t& rhs) -> decltype(auto) {
            return lhs[rhs];
          });
    if constexpr (operator_subscript<const T, const std::size_t>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_subscript,
          [](const T& lhs, const std::size_t& rhs) -> decltype(auto) {
            return lhs[rhs];
          });
    if constexpr (operator_deref<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_deref,
                           [](T& lhs) -> decltype(auto) { return *lhs; });
    if constexpr (operator_deref<const T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_deref,
                           [](const T& lhs) -> decltype(auto) { return *lhs; });

    // iterator
    if constexpr (std::input_iterator<T>) {
      mngr.AddMemberMethod(NameIDRegistry::Meta::iterator_advance,
                           [](T& lhs, const std::iter_difference_t<T>& rhs) {
                             std::advance(lhs, rhs);
                           });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::iterator_next,
          [](const T& lhs, const std::iter_difference_t<T>& rhs)
              -> decltype(auto) { return std::next(lhs, rhs); });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::iterator_prev,
          [](const T& lhs, const std::iter_difference_t<T>& rhs)
              -> decltype(auto) { return std::prev(lhs, rhs); });
      if constexpr (std::is_convertible_v<std::iter_difference_t<T>,
                                          std::size_t>) {
        mngr.AddMemberMethod(
            NameIDRegistry::Meta::iterator_distance,
            [](const T& lhs, const T& rhs) {
              return static_cast<std::size_t>(std::distance(lhs, rhs));
            });
      }
      if constexpr (std::random_access_iterator<T>) {
        mngr.AddMemberMethod(
            NameIDRegistry::Meta::operator_add,
            [](const T& lhs, const std::iter_difference_t<T>& rhs)
                -> decltype(auto) { return lhs + rhs; });
        mngr.AddMemberMethod(
            NameIDRegistry::Meta::operator_sub,
            [](const T& lhs, const std::iter_difference_t<T>& rhs)
                -> decltype(auto) { return lhs - rhs; });
      }
    }

    // pair

    if constexpr (IsPair<T>) {
      mngr.RegisterType<typename T::first_type>();
      mngr.RegisterType<typename T::second_type>();
      mngr.AddField<&T::first>("first");
      mngr.AddField<&T::second>("second");
    }

    // tuple

    if constexpr (IsTuple<T> && !IsArray<T>) {
      mngr.AddStaticMethod(Type_of<T>, NameIDRegistry::Meta::tuple_size, []() {
        return static_cast<std::size_t>(std::tuple_size_v<T>);
      });
      mngr.AddMemberMethod(NameIDRegistry::Meta::tuple_get,
                           [](T& t, const std::size_t& i) {
                             return runtime_get<std::tuple_size>(t, i);
                           });
      mngr.AddMemberMethod(NameIDRegistry::Meta::tuple_get,
                           [](const T& t, const std::size_t& i) {
                             return runtime_get<std::tuple_size>(t, i);
                           });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::tuple_get, [](T& t, const Type& type) {
            return runtime_get<std::tuple_size, std::tuple_element>(t, type);
          });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::tuple_get, [](const T& t, const Type& type) {
            return runtime_get<std::tuple_size, std::tuple_element>(t, type);
          });
      mngr.AddStaticMethod(
          Type_of<T>, NameIDRegistry::Meta::tuple_element,
          [](const std::size_t& i) { return runtime_tuple_element<T>(i); });
      register_tuple_elements<T>(
          mngr, std::make_index_sequence<std::tuple_size_v<T>>{});
    }

    // variant

    if constexpr (IsVariant<T>) {
      mngr.AddMemberMethod(NameIDRegistry::Meta::variant_index, [](const T& t) {
        return static_cast<std::size_t>(t.index());
      });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::variant_valueless_by_exception, [](const T& t) {
            return static_cast<bool>(t.valueless_by_exception());
          });
      mngr.AddStaticMethod(
          Type_of<T>, NameIDRegistry::Meta::variant_size,
          []() { return static_cast<std::size_t>(std::variant_size_v<T>); });
      mngr.AddMemberMethod(NameIDRegistry::Meta::variant_get,
                           [](T& t, const std::size_t& i) {
                             return runtime_get<std::variant_size>(t, i);
                           });
      mngr.AddMemberMethod(NameIDRegistry::Meta::variant_get,
                           [](const T& t, const std::size_t& i) {
                             return runtime_get<std::variant_size>(t, i);
                           });
      mngr.AddMemberMethod(NameIDRegistry::Meta::variant_holds_alternative,
                           [](const T& t, const Type& type) {
                             return runtime_variant_holds_alternative(t, type);
                           });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::variant_get, [](T& t, const Type& type) {
            return runtime_get<std::variant_size, std::variant_alternative>(
                t, type);
          });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::variant_get, [](const T& t, const Type& type) {
            return runtime_get<std::variant_size, std::variant_alternative>(
                t, type);
          });
      mngr.AddStaticMethod(Type_of<T>,
                           NameIDRegistry::Meta::variant_alternative,
                           [](const std::size_t& i) {
                             return runtime_variant_alternative<T>(i);
                           });
      mngr.AddMemberMethod(NameIDRegistry::Meta::variant_visit_get, [](T& t) {
        return runtime_get<std::variant_size>(t, t.index());
      });
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::variant_visit_get, [](const T& t) {
            return runtime_get<std::variant_size>(t, t.index());
          });
      register_variant_alternatives<T>(
          mngr, std::make_index_sequence<std::variant_size_v<T>>{});
    }

    // optional

    if constexpr (IsOptional<T>) {
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::optional_has_value,
          [](const T& t) { return static_cast<bool>(t.has_value()); });
      mngr.AddMemberMethod(NameIDRegistry::Meta::optional_value,
                           [](T& t) { return ObjectView{t.value()}; });
      mngr.AddMemberMethod(NameIDRegistry::Meta::optional_value,
                           [](const T& t) { return ObjectView{t.value()}; });
      mngr.AddMemberMethod(NameIDRegistry::Meta::optional_reset,
                           [](T& t) { t.reset(); });
    }

    // container

    // - ctor

    if constexpr (container_ctor_cnt<T>)
      mngr.AddConstructor<T, const typename T::size_type&>();
    if constexpr (container_ctor_clvalue<T>)
      mngr.AddConstructor<T, const typename T::value_type&>();
    if constexpr (container_ctor_rvalue<T>)
      mngr.AddConstructor<T, typename T::value_type&&>();
    if constexpr (container_ctor_cnt_value<T>)
      mngr.AddConstructor<T, const typename T::size_type&,
                          const typename T::value_type&>();
    if constexpr (container_ctor_ptr_cnt<T>)
      mngr.AddConstructor<T, const typename T::pointer_type&,
                          const typename T::size_type&>();
    if constexpr (container_ctor_ptr_ptr<T>)
      mngr.AddConstructor<T, const typename T::pointer_type&,
                          const typename T::pointer_type&>();

    // - assign

    if constexpr (container_assign<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_assign,
          [](T& lhs, const typename T::size_type& s,
             const typename T::value_type& v) { lhs.assign(s, v); });

    // - iterator

    if constexpr (container_begin<T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_begin,
          [](T& lhs) -> decltype(auto) { return std::begin(lhs); });
    if constexpr (container_begin<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_begin,
          [](const T& lhs) -> decltype(auto) { return std::begin(lhs); });
    if constexpr (container_cbegin<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_cbegin,
          [](const T& lhs) -> decltype(auto) { return std::cbegin(lhs); });
    if constexpr (container_end<T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_end,
          [](T& lhs) -> decltype(auto) { return std::end(lhs); });
    if constexpr (container_end<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_end,
          [](const T& lhs) -> decltype(auto) { return std::end(lhs); });
    if constexpr (container_cend<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_cend,
          [](const T& lhs) -> decltype(auto) { return std::cend(lhs); });

    if constexpr (container_rbegin<T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_rbegin,
          [](T& lhs) -> decltype(auto) { return std::rbegin(lhs); });
    if constexpr (container_rbegin<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_rbegin,
          [](const T& lhs) -> decltype(auto) { return std::rbegin(lhs); });
    if constexpr (container_crbegin<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_crbegin,
          [](const T& lhs) -> decltype(auto) { return std::rbegin(lhs); });
    if constexpr (container_rend<T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_rend,
          [](T& lhs) -> decltype(auto) { return std::rend(lhs); });
    if constexpr (container_rend<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_rend,
          [](const T& lhs) -> decltype(auto) { return std::rend(lhs); });
    if constexpr (container_crend<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_crend,
          [](const T& lhs) -> decltype(auto) { return std::crend(lhs); });

    // - element access

    if constexpr (container_at_size<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_at,
                           [](T& lhs, const std::size_t& n) -> decltype(auto) {
                             return lhs.at(n);
                           });
    if constexpr (container_at_size<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_at,
          [](const T& lhs, const std::size_t& n) -> decltype(auto) {
            return lhs.at(n);
          });

    if constexpr (container_at_key<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_at,
          [](T& lhs, const typename T::key_type& key) -> decltype(auto) {
            return lhs.at(key);
          });
    if constexpr (container_at_key<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_at,
          [](const T& lhs, const typename T::key_type& key) -> decltype(auto) {
            return lhs.at(key);
          });

    if constexpr (container_subscript_size<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::operator_subscript,
                           [](T& lhs, const get_container_size_type_t<T>& rhs)
                               -> decltype(auto) { return lhs[rhs]; });
    if constexpr (container_subscript_size<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_subscript,
          [](const T& lhs, const get_container_size_type_t<T>& rhs)
              -> decltype(auto) { return lhs[rhs]; });

    if constexpr (container_subscript_key_cl<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_subscript,
          [](T& lhs, const typename T::key_type& key) -> decltype(auto) {
            return lhs[key];
          });
    if constexpr (container_subscript_key_r<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::operator_subscript,
          [](T& lhs, typename T::key_type&& key) -> decltype(auto) {
            return lhs[std::move(key)];
          });

    if constexpr (container_data<T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_data,
          [](T& lhs) -> decltype(auto) { return std::data(lhs); });
    if constexpr (container_data<const T&>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_data,
          [](const T& lhs) -> decltype(auto) { return std::data(lhs); });

    if constexpr (container_front<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_front,
          [](T& lhs) -> decltype(auto) { return lhs.front(); });
    if constexpr (container_front<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_front,
          [](const T& lhs) -> decltype(auto) { return lhs.front(); });

    if constexpr (container_back<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_back,
                           [](T& lhs) -> decltype(auto) { return lhs.back(); });
    if constexpr (container_back<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_back,
          [](const T& lhs) -> decltype(auto) { return lhs.back(); });

    if constexpr (container_top<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_top,
                           [](T& lhs) -> decltype(auto) { return lhs.top(); });
    if constexpr (container_top<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_top,
          [](const T& lhs) -> decltype(auto) { return lhs.top(); });

    if constexpr (container_empty<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_empty,
          [](const T& lhs) { return static_cast<bool>(std::empty(lhs)); });

    if constexpr (container_size<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_size,
                           [](const T& lhs) {
                             return static_cast<std::size_t>(std::size(lhs));
                           });

    if constexpr (container_size_bytes<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_size_bytes,
                           [](const T& lhs) {
                             return static_cast<std::size_t>(lhs.size_bytes());
                           });

    if constexpr (container_resize_cnt<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_resize,
          [](T& lhs, const typename T::size_type& n) { lhs.resize(n); });

    if constexpr (container_resize_cnt_value<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_resize,
          [](T& lhs, const typename T::size_type& n,
             const typename T::value_type& value) { lhs.resize(n, value); });

    if constexpr (container_capacity<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_capacity,
                           [](const T& lhs) {
                             return static_cast<std::size_t>(lhs.capacity());
                           });

    if constexpr (container_bucket_count<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_bucket_count, [](const T& lhs) {
            return static_cast<std::size_t>(lhs.bucket_count());
          });

    if constexpr (container_reserve<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_reserve,
          [](T& lhs, const typename T::size_type& n) { lhs.reserve(n); });

    if constexpr (container_shrink_to_fit<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_shrink_to_fit,
                           [](T& lhs) { lhs.shrink_to_fit(); });

    // - modifiers

    if constexpr (container_clear<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_clear,
                           [](T& lhs) { lhs.clear(); });

    if constexpr (container_insert_clvalue<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert,
          [](T& lhs, const typename T::value_type& value) -> decltype(auto) {
            return lhs.insert(value);
          });

    if constexpr (container_insert_rvalue<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert,
          [](T& lhs, typename T::value_type&& value) -> decltype(auto) {
            return lhs.insert(std::move(value));
          });

    if constexpr (container_insert_rnode<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert,
          [](T& lhs, typename T::node_type&& node) -> decltype(auto) {
            return lhs.insert(std::move(node));
          });

    if constexpr (container_insert_citer_clvalue<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert,
          [](T& lhs, const typename T::const_iterator& iter,
             const typename T::value_type& value) -> decltype(auto) {
            return lhs.insert(iter, value);
          });

    if constexpr (container_insert_citer_rvalue<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert,
          [](T& lhs, const typename T::const_iterator& iter,
             typename T::value_type&& value) -> decltype(auto) {
            return lhs.insert(iter, std::move(value));
          });

    if constexpr (container_insert_citer_rnode<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_insert,
                           [](T& lhs, const typename T::const_iterator& iter,
                              typename T::node_type&& node) -> decltype(auto) {
                             return lhs.insert(iter, std::move(node));
                           });

    if constexpr (container_insert_citer_cnt<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert,
          [](T& lhs, const typename T::const_iterator& iter,
             const typename T::size_type& cnt,
             const typename T::value_type& value) -> decltype(auto) {
            return lhs.insert(iter, cnt, value);
          });

    if constexpr (container_insert_after_clvalue<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert_after,
          [](T& lhs, const typename T::const_iterator& pos,
             const typename T::value_type& value) -> decltype(auto) {
            return lhs.insert_after(pos, value);
          });

    if constexpr (container_insert_after_rvalue<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert_after,
          [](T& lhs, const typename T::const_iterator& pos,
             typename T::value_type&& value) -> decltype(auto) {
            return lhs.insert_after(pos, std::move(value));
          });

    if constexpr (container_insert_after_cnt<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_insert_after,
          [](T& lhs, const typename T::const_iterator& pos,
             const typename T::size_type& cnt,
             const typename T::value_type& value) -> decltype(auto) {
            return lhs.insert_after(pos, cnt, value);
          });

    if constexpr (container_erase_citer<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_erase,
          [](T& lhs, const typename T::const_iterator& rhs) -> decltype(auto) {
            return lhs.erase(rhs);
          });

    if constexpr (container_erase_key<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_erase,
          [](T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.erase(rhs);
          });

    if constexpr (container_erase_range_citer<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_erase,
          [](T& lhs, const typename T::const_iterator& start,
             const typename T::const_iterator& end) -> decltype(auto) {
            return lhs.erase(start, end);
          });

    if constexpr (container_erase_after<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_erase_after,
          [](T& lhs, const typename T::const_iterator& pos) -> decltype(auto) {
            return lhs.erase_after(pos);
          });

    if constexpr (container_erase_after_range<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_erase_after,
          [](T& lhs, const typename T::const_iterator& first,
             const typename T::const_iterator& last) -> decltype(auto) {
            return lhs.erase_after(first, last);
          });

    if constexpr (container_push_front_clvalue<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_push_front,
                           [](T& lhs, const typename T::value_type& value) {
                             lhs.push_front(value);
                           });

    if constexpr (container_push_front_rvalue<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_push_front,
                           [](T& lhs, typename T::value_type&& value) {
                             lhs.push_front(std::move(value));
                           });

    if constexpr (container_pop_front<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_pop_front,
                           [](T& lhs) { lhs.pop_front(); });

    if constexpr (container_push_back_clvalue<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_push_back,
                           [](T& lhs, const typename T::value_type& value) {
                             lhs.push_back(value);
                           });

    if constexpr (container_push_back_rvalue<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_push_back,
                           [](T& lhs, typename T::value_type&& value) {
                             lhs.push_back(std::move(value));
                           });

    if constexpr (container_pop_back<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_pop_back,
                           [](T& lhs) { lhs.pop_back(); });

    if constexpr (container_push_clvalue<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_push,
          [](T& lhs, const typename T::value_type& value) { lhs.push(value); });

    if constexpr (container_push_rvalue<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_push,
                           [](T& lhs, typename T::value_type&& value) {
                             lhs.push(std::move(value));
                           });

    if constexpr (container_pop<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_pop,
                           [](T& lhs) { lhs.pop(); });

    if constexpr (container_swap<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_swap,
                           [](T& lhs, T& rhs) { std::swap(lhs, rhs); });

    if constexpr (container_merge_l<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_merge,
                           [](T& lhs, T& rhs) { lhs.merge(rhs); });

    if constexpr (container_merge_r<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_merge,
                           [](T& lhs, T&& rhs) { lhs.merge(std::move(rhs)); });

    if constexpr (container_extract_citer<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_extract,
          [](T& lhs, const typename T::const_iterator& iter) -> decltype(auto) {
            return lhs.extract(iter);
          });

    if constexpr (container_extract_key<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_extract,
          [](T& lhs, const typename T::key_type& key) -> decltype(auto) {
            return lhs.extract(key);
          });

    // - list operations

    if constexpr (container_splice_after_l<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice_after,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T& other) { lhs.splice_after(pos, other); });

    if constexpr (container_splice_after_r<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_splice_after,
          [](T& lhs, const typename T::const_iterator& pos, T&& other) {
            lhs.splice_after(pos, std::move(other));
          });

    if constexpr (container_splice_after_it_l<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice_after,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T& other, const typename T::const_iterator& it) {
                             lhs.splice_after(pos, other, it);
                           });

    if constexpr (container_splice_after_it_r<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice_after,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T&& other, const typename T::const_iterator& it) {
                             lhs.splice_after(pos, std::move(other), it);
                           });

    if constexpr (container_splice_after_range_l<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice_after,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T& other, const typename T::const_iterator& first,
                              const typename T::const_iterator& last) {
                             lhs.splice_after(pos, other, first, last);
                           });

    if constexpr (container_splice_after_range_l<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_splice_after,
          [](T& lhs, const typename T::const_iterator& pos, T&& other,
             const typename T::const_iterator& first,
             const typename T::const_iterator& last) {
            lhs.splice_after(pos, std::move(other), first, last);
          });

    if constexpr (container_splice_l<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T& other) { lhs.splice(pos, other); });

    if constexpr (container_splice_r<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_splice,
          [](T& lhs, const typename T::const_iterator& pos, T&& other) {
            lhs.splice(pos, std::move(other));
          });

    if constexpr (container_splice_it_l<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T& other, const typename T::const_iterator& it) {
                             lhs.splice(pos, other, it);
                           });

    if constexpr (container_splice_it_r<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T&& other, const typename T::const_iterator& it) {
                             lhs.splice(pos, std::move(other), it);
                           });

    if constexpr (container_splice_range_l<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_splice,
                           [](T& lhs, const typename T::const_iterator& pos,
                              T& other, const typename T::const_iterator& first,
                              const typename T::const_iterator& last) {
                             lhs.splice(pos, other, first, last);
                           });

    if constexpr (container_splice_range_l<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_splice,
          [](T& lhs, const typename T::const_iterator& pos, T&& other,
             const typename T::const_iterator& first,
             const typename T::const_iterator& last) {
            lhs.splice(pos, std::move(other), first, last);
          });

    if constexpr (container_remove<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_remove,
                           [](T& lhs, const typename T::value_type& v) {
                             return static_cast<std::size_t>(lhs.remove(v));
                           });

    if constexpr (container_reverse<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_reverse,
                           [](T& lhs) { lhs.reverse(); });

    if constexpr (container_unique<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_unique, [](T& lhs) {
        return static_cast<std::size_t>(lhs.unique());
      });

    if constexpr (container_sort<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_sort,
                           [](T& lhs) { lhs.sort(); });

    // - lookup

    if constexpr (container_count<T>)
      mngr.AddMemberMethod(NameIDRegistry::Meta::container_count,
                           [](const T& lhs, const typename T::key_type& rhs) {
                             return static_cast<std::size_t>(lhs.count(rhs));
                           });

    if constexpr (container_find<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_find,
          [](T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.find(rhs);
          });
    if constexpr (container_find<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_find,
          [](const T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.find(rhs);
          });

    if constexpr (container_lower_bound<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_lower_bound,
          [](T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.lower_bound(rhs);
          });
    if constexpr (container_lower_bound<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_lower_bound,
          [](const T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.lower_bound(rhs);
          });

    if constexpr (container_upper_bound<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_upper_bound,
          [](T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.upper_bound(rhs);
          });
    if constexpr (container_upper_bound<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_upper_bound,
          [](const T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.upper_bound(rhs);
          });

    if constexpr (container_equal_range<T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_equal_range,
          [](T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.equal_range(rhs);
          });
    if constexpr (container_equal_range<const T>)
      mngr.AddMemberMethod(
          NameIDRegistry::Meta::container_equal_range,
          [](const T& lhs, const typename T::key_type& rhs) -> decltype(auto) {
            return lhs.equal_range(rhs);
          });

    if constexpr (IsVector<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Vector));
    else if constexpr (IsArray<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Array));
    else if constexpr (IsRawArray<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::RawArray));
    else if constexpr (IsDeque<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Deque));
    else if constexpr (IsForwardList<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::ForwardList));
    else if constexpr (IsList<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::List));
    else if constexpr (IsMap<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Map));
    else if constexpr (IsMultiMap<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::MultiMap));
    else if constexpr (IsSet<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Set));
    else if constexpr (IsMultiSet<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::MultiSet));
    else if constexpr (IsUnorderedMap<T>)
      mngr.AddTypeAttr(
          Type_of<T>,
          mngr.MakeShared(Type_of<ContainerType>, ContainerType::UnorderedMap));
    else if constexpr (IsUnorderedMultiMap<T>)
      mngr.AddTypeAttr(Type_of<T>,
                       mngr.MakeShared(Type_of<ContainerType>,
                                       ContainerType::UnorderedMultiMap));
    else if constexpr (IsUnorderedSet<T>)
      mngr.AddTypeAttr(
          Type_of<T>,
          mngr.MakeShared(Type_of<ContainerType>, ContainerType::UnorderedSet));
    else if constexpr (IsUnorderedMultiSet<T>)
      mngr.AddTypeAttr(Type_of<T>,
                       mngr.MakeShared(Type_of<ContainerType>,
                                       ContainerType::UnorderedMultiSet));
    else if constexpr (IsStack<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Stack));
    else if constexpr (IsPriorityQueue<T>)
      mngr.AddTypeAttr(Type_of<T>,
                       mngr.MakeShared(Type_of<ContainerType>,
                                       ContainerType::PriorityQueue));
    else if constexpr (IsQueue<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Queue));
    else if constexpr (IsPair<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Pair));
    else if constexpr (IsTuple<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Tuple));
    else if constexpr (IsSpan<T>)
      mngr.AddTypeAttr(Type_of<T>, mngr.MakeShared(Type_of<ContainerType>,
                                                   ContainerType::Span));

    // - type

    if constexpr (std::is_array_v<T>) {
      using value_type = std::remove_extent_t<T>;
      using pointer = value_type*;
      using const_pointer = const value_type*;
      mngr.RegisterType<value_type>();
      mngr.RegisterType<pointer>();
      mngr.RegisterType<const_pointer>();
    } else {
      if constexpr (container_key_type<T>)
        mngr.RegisterType<typename T::key_type>();
      if constexpr (container_mapped_type<T>)
        mngr.RegisterType<typename T::mapped_type>();
      if constexpr (container_value_type<T>)
        mngr.RegisterType<typename T::value_type>();
      if constexpr (container_size_type<T>)
        mngr.RegisterType<typename T::size_type>();
      if constexpr (container_difference_type<T>)
        mngr.RegisterType<typename T::difference_type>();
      if constexpr (!is_instance_of_v<T, std::allocator>) {
        if constexpr (container_pointer_type<T>)
          mngr.RegisterType<typename T::pointer>();
        if constexpr (container_const_pointer_type<T>)
          mngr.RegisterType<typename T::const_pointer>();
      }
      if constexpr (container_iterator<T>)
        mngr.RegisterType<typename T::iterator>();
      if constexpr (container_const_iterator<T>)
        mngr.RegisterType<typename T::const_iterator>();
      if constexpr (container_local_iterator<T>)
        mngr.RegisterType<typename T::local_iterator>();
      if constexpr (container_const_local_iterator<T>)
        mngr.RegisterType<typename T::const_local_iterator>();
      if constexpr (container_node_type<T>)
        mngr.RegisterType<typename T::node_type>();
      if constexpr (container_insert_return_type<T>) {
        mngr.RegisterType<typename T::insert_return_type>();
        mngr.AddField<&T::insert_return_type::position>("position");
        mngr.AddField<&T::insert_return_type::inserted>("inserted");
        mngr.AddField<&T::insert_return_type::node>("node");
      }

      if constexpr (container_iterator<T> && container_const_iterator<T>)
        mngr.AddConstructor<typename T::const_iterator,
                            const typename T::iterator&>();
      if constexpr (container_local_iterator<T> &&
                    container_const_local_iterator<T>)
        mngr.AddConstructor<typename T::const_local_iterator,
                            const typename T::local_iterator&>();
    }
  }
};

template <typename T>
struct TypeAutoRegister : TypeAutoRegister_Default<T> {};
};  // namespace My::MyDRefl::details

namespace My::MyDRefl {
//
// Factory
////////////

template <auto field_data>
FieldPtr ReflMngr::GenerateFieldPtr() {
  using FieldData = decltype(field_data);
  if constexpr (std::is_pointer_v<FieldData>) {
    using Value = std::remove_pointer_t<FieldData>;
    RegisterType<Value>();
    return {Type_of<Value>, ptr_const_cast(field_data)};
  } else if constexpr (std::is_member_object_pointer_v<FieldData>) {
    using Traits = member_pointer_traits<FieldData>;
    using Obj = typename Traits::object;
    using Value = typename Traits::value;

    RegisterType<Value>();
    if constexpr (has_virtual_base_v<Obj>) {
      return {Type_of<Value>, field_offsetor<field_data>()};
    } else {
      return {Type_of<Value>, field_forward_offset_value(field_data)};
    }
  } else if constexpr (std::is_enum_v<FieldData>) {
    using Value = std::remove_pointer_t<FieldData>;
    RegisterType<Value>();
    auto buffer = FieldPtr::ConvertToBuffer(field_data);
    return {Type_of<Value>, buffer};
  } else
    static_assert(always_false<FieldData>);
}

template <typename T>
FieldPtr ReflMngr::GenerateFieldPtr(T&& data) {
  using RawT = std::remove_cv_t<std::remove_reference_t<T>>;
  static_assert(!std::is_same_v<RawT, std::size_t>);
  if constexpr (std::is_member_object_pointer_v<RawT>) {
    using Traits = member_pointer_traits<RawT>;
    using Obj = typename Traits::object;
    using Value = typename Traits::value;
    tregistry.Register<Value>();
    RegisterType<Value>();
    if constexpr (has_virtual_base_v<Obj>) {
      return {Type_of<Value>, field_offsetor(data)};
    } else {
      return {Type_of<Value>, field_forward_offset_value(data)};
    }
  } else if constexpr (std::is_pointer_v<RawT> &&
                       !is_function_pointer_v<RawT> &&
                       !std::is_void_v<std::remove_pointer_t<RawT>>) {
    using Value = std::remove_pointer_t<RawT>;
    tregistry.Register<Value>();
    RegisterType<Value>();
    return {Type_of<Value>, ptr_const_cast(data)};
  } else if constexpr (std::is_enum_v<RawT>) {
    tregistry.Register<RawT>();
    RegisterType<RawT>();
    auto buffer = FieldPtr::ConvertToBuffer(data);
    return {Type_of<const RawT>, buffer};
  } else {
    using Traits = FuncTraits<RawT>;

    using ArgList = typename Traits::ArgList;
    static_assert(Length_v<ArgList> == 1);
    using ObjPtr = Front_t<ArgList>;
    static_assert(std::is_pointer_v<ObjPtr>);
    using Obj = std::remove_pointer_t<ObjPtr>;
    static_assert(!std::is_const_v<Obj>);

    using ValuePtr = typename Traits::Return;
    static_assert(std::is_pointer_v<ValuePtr>);
    using Value = std::remove_pointer_t<ValuePtr>;
    static_assert(!std::is_void_v<Value> && !std::is_volatile_v<Value>);

    tregistry.Register<Value>();
    RegisterType<Value>();

    auto offsetor = [f = std::forward<T>(data)](void* obj) -> void* {
      return f(reinterpret_cast<Obj*>(obj));
    };

    return {Type_of<Value>, offsetor};
  }
}

template <typename T, typename... Args>
FieldPtr ReflMngr::GenerateDynamicFieldPtr(Args&&... args) {
  static_assert(!std::is_reference_v<T> && !std::is_volatile_v<T>);
  using RawT = std::remove_const_t<T>;
  RegisterType<RawT>();
  AddConstructor<RawT, Args...>();
  if constexpr (FieldPtr::IsBufferable<RawT>()) {
    FieldPtr::Buffer buffer =
        FieldPtr::ConvertToBuffer(T{std::forward<Args>(args)...});
    return FieldPtr{Type_of<T>, buffer};
  } else
    return FieldPtr{MakeShared(Type_of<RawT>, std::forward<Args>(args)...)};
}

template <typename... Params>
ParamList ReflMngr::GenerateParamList() noexcept(sizeof...(Params) == 0) {
  if constexpr (sizeof...(Params) > 0) {
    static_assert(
        ((!std::is_const_v<Params> && !std::is_volatile_v<Params>) && ...),
        "parameter type shouldn't be const.");
    return {Type_of<Params>...};
  } else
    return {};
}

template <auto funcptr>
MethodPtr ReflMngr::GenerateMethodPtr() {
  using FuncPtr = decltype(funcptr);
  using Traits = FuncTraits<decltype(funcptr)>;
  using ArgList = typename Traits::ArgList;
  using Return = typename Traits::Return;
  using Helper = details::GenerateMethodPtr_Helper<ArgList>;
  constexpr MethodFlag flag =
      Traits::is_const ? MethodFlag::Const : MethodFlag::Variable;
  return {Helper::template GenerateFunction<funcptr>(
              std::make_index_sequence<Length_v<ArgList>>{}),
          flag, Type_of<Return>, Helper::GenerateParamList()};
}

template <typename T, typename... Args>
MethodPtr ReflMngr::GenerateConstructorPtr() {
  return GenerateMemberMethodPtr(
      [](T& obj, Args... args) { new (&obj) T{std::forward<Args>(args)...}; });
}

template <typename T>
MethodPtr ReflMngr::GenerateDestructorPtr() {
  return GenerateMemberMethodPtr([](const T& obj) {
    if constexpr (!std::is_trivially_destructible_v<T>)
      obj.~T();
  });
}

template <typename Func>
MethodPtr ReflMngr::GenerateMemberMethodPtr(Func&& func) {
  using Traits = details::WrapFuncTraits<std::decay_t<Func>>;
  using ArgList = typename Traits::ArgList;
  using Return = typename Traits::Return;
  using Helper = details::GenerateMethodPtr_Helper<ArgList>;
  constexpr MethodFlag flag =
      Traits::is_const ? MethodFlag::Const : MethodFlag::Variable;
  return {Helper::template GenerateMemberFunction(
              std::forward<Func>(func),
              std::make_index_sequence<Length_v<ArgList>>{}),
          flag, Type_of<Return>, Helper::GenerateParamList()};
}

template <typename Func>
MethodPtr ReflMngr::GenerateStaticMethodPtr(Func&& func) {
  using Traits = FuncTraits<std::decay_t<Func>>;
  using Return = typename Traits::Return;
  using ArgList = typename Traits::ArgList;
  using Helper = details::GenerateMethodPtr_Helper<ArgList>;
  return {Helper::template GenerateStaticFunction(
              std::forward<Func>(func),
              std::make_index_sequence<Length_v<ArgList>>{}),
          MethodFlag::Static, Type_of<Return>, Helper::GenerateParamList()};
}

//
// Modifier
/////////////

template <typename T>
void ReflMngr::RegisterType() {
  static_assert(!std::is_volatile_v<T>);
  if constexpr (std::is_void_v<T>)
    return;
  else {
    if constexpr (std::is_const_v<T>)
      RegisterType<std::remove_const_t<T>>();
    else if constexpr (std::is_reference_v<T>)
      RegisterType<std::remove_cvref_t<T>>();
    else {
      auto target = typeinfos.find(Type_of<T>);
      if (target != typeinfos.end())
        return;
      tregistry.Register<T>();
      typeinfos.emplace_hint(target, Type_of<T>,
                             TypeInfo{sizeof(T), alignof(T)});

      details::TypeAutoRegister<T>::run(*this);
    }
  }
}

template <auto field_data>
bool ReflMngr::AddField(Name name, AttrSet attrs) {
  using FieldData = decltype(field_data);
  if constexpr (std::is_enum_v<FieldData>) {
    return AddField(Type_of<std::remove_cv_t<FieldData>>, name,
                    {GenerateFieldPtr<field_data>(), std::move(attrs)});
  } else if constexpr (std::is_member_object_pointer_v<FieldData>) {
    return AddField(Type_of<member_pointer_traits_object<FieldData>>, name,
                    {GenerateFieldPtr<field_data>(), std::move(attrs)});
  } else
    static_assert(always_false<FieldData>,
                  "if field_data is a static field, use AddField(Type, name, "
                  "field_data, attrs)");
}

template <typename T,
          std::enable_if_t<!std::is_same_v<std::decay_t<T>, FieldInfo>, int>>
bool ReflMngr::AddField(Name name, T&& data, AttrSet attrs) {
  using RawT = std::remove_cv_t<std::remove_reference_t<T>>;
  if constexpr (std::is_member_object_pointer_v<RawT>)
    return AddField(Type_of<member_pointer_traits_object<RawT>>, name,
                    std::forward<T>(data), std::move(attrs));
  else if constexpr (std::is_enum_v<RawT>)
    return AddField(Type_of<RawT>, name, std::forward<T>(data),
                    std::move(attrs));
  else {
    using Traits = FuncTraits<RawT>;

    using ArgList = typename Traits::ArgList;
    static_assert(Length_v<ArgList> == 1);
    using ObjPtr = Front_t<ArgList>;
    static_assert(std::is_pointer_v<ObjPtr>);
    using Obj = std::remove_pointer_t<ObjPtr>;

    return AddField(Type_of<Obj>, name, std::forward<T>(data),
                    std::move(attrs));
  }
}

template <auto member_func_ptr>
bool ReflMngr::AddMethod(Name name, AttrSet attrs) {
  using MemberFuncPtr = decltype(member_func_ptr);
  static_assert(std::is_member_function_pointer_v<MemberFuncPtr>);
  using Obj = member_pointer_traits_object<MemberFuncPtr>;
  return AddMethod(Type_of<Obj>, name,
                   {GenerateMethodPtr<member_func_ptr>(), std::move(attrs)});
}

template <auto func_ptr>
bool ReflMngr::AddMethod(Type type, Name name, AttrSet attrs) {
  using FuncPtr = decltype(func_ptr);
  static_assert(is_function_pointer_v<FuncPtr>);
  return AddMethod(type, name,
                   {GenerateMethodPtr<func_ptr>(), std::move(attrs)});
}

template <typename T, typename... Args>
bool ReflMngr::AddConstructor(AttrSet attrs) {
  return AddMethod(Type_of<T>, NameIDRegistry::Meta::ctor,
                   {GenerateConstructorPtr<T, Args...>(), std::move(attrs)});
}

template <typename T>
bool ReflMngr::AddDestructor(AttrSet attrs) {
  return AddMethod(Type_of<T>, NameIDRegistry::Meta::dtor,
                   {GenerateDestructorPtr<T>(), std::move(attrs)});
}

template <typename Func>
bool ReflMngr::AddMemberMethod(Name name, Func&& func, AttrSet attrs) {
  return AddMethod(
      Type_of<typename details::WrapFuncTraits<std::decay_t<Func>>::Object>,
      name,
      {GenerateMemberMethodPtr(std::forward<Func>(func)), std::move(attrs)});
}

template <typename Derived, typename Base>
BaseInfo ReflMngr::GenerateBaseInfo() {
  return {inherit_cast_functions<Derived, Base>(), std::is_polymorphic_v<Base>,
          is_virtual_base_of_v<Base, Derived>};
}

template <typename Derived, typename... Bases>
bool ReflMngr::AddBases() {
  return (AddBase(Type_of<Derived>, Type_of<Bases>,
                  GenerateBaseInfo<Derived, Bases>()) &&
          ...);
}

//
// Invoke
///////////

template <typename... Args>
Type ReflMngr::IsInvocable(Type type, Name method_name, MethodFlag flag) const {
  constexpr Type argTypes[] = {Type_of<Args>...};
  return IsInvocable(type, method_name, std::span<const Type>{argTypes}, flag);
}

template <typename T>
T ReflMngr::BInvokeRet(ObjectView obj, Name method_name,
                       std::span<const Type> argTypes,
                       ArgPtrBuffer argptr_buffer, MethodFlag flag,
                       std::pmr::memory_resource* temp_args_rsrc) const {
  if constexpr (!std::is_void_v<T>) {
    using U =
        std::conditional_t<std::is_reference_v<T>, std::add_pointer_t<T>, T>;
    std::aligned_storage_t<sizeof(U), alignof(U)> result_buffer;
    Type result_type =
        BInvoke(obj, method_name, static_cast<void*>(&result_buffer), argTypes,
                argptr_buffer, flag, temp_args_rsrc);
    return MoveResult<T>(result_type, &result_buffer);
  } else
    BInvoke(obj, method_name, (void*)nullptr, argTypes, argptr_buffer, flag,
            temp_args_rsrc);
}

template <typename T, typename... Args>
T ReflMngr::BInvoke(ObjectView obj, Name method_name, MethodFlag flag,
                    std::pmr::memory_resource* temp_args_rsrc,
                    Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return BInvokeRet<T>(obj, method_name, std::span<const Type>{argTypes},
                         static_cast<ArgPtrBuffer>(argptr_buffer), flag,
                         temp_args_rsrc);
  } else
    return BInvokeRet<T>(obj, method_name, std::span<const Type>{},
                         static_cast<ArgPtrBuffer>(nullptr), flag,
                         temp_args_rsrc);
}

template <typename... Args>
SharedObject ReflMngr::MInvoke(ObjectView obj, Name method_name,
                               std::pmr::memory_resource* rst_rsrc,
                               std::pmr::memory_resource* temp_args_rsrc,
                               MethodFlag flag, Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return MInvoke(obj, method_name, rst_rsrc, temp_args_rsrc,
                   std::span<const Type>{argTypes},
                   static_cast<ArgPtrBuffer>(argptr_buffer), flag);
  } else
    return MInvoke(obj, method_name, rst_rsrc, temp_args_rsrc,
                   std::span<const Type>{}, static_cast<ArgPtrBuffer>(nullptr),
                   flag);
}

template <typename... Args>
SharedObject ReflMngr::Invoke(ObjectView obj, Name method_name,
                              Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return Invoke(obj, method_name, std::span<const Type>{argTypes},
                  static_cast<ArgPtrBuffer>(argptr_buffer));
  } else
    return Invoke(obj, method_name);
}

//
// Make
/////////

template <typename... Args>
bool ReflMngr::IsConstructible(Type type) const {
  constexpr Type argTypes[] = {Type_of<Args>...};
  return IsConstructible(type, std::span<const Type>{argTypes});
}

template <typename... Args>
bool ReflMngr::NonCopiedArgConstruct(ObjectView obj, Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return NonCopiedArgConstruct(obj, std::span<const Type>{argTypes},
                                 static_cast<ArgPtrBuffer>(argptr_buffer));
  } else
    return NonCopiedArgConstruct(obj);
}

template <typename... Args>
bool ReflMngr::Construct(ObjectView obj, Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return Construct(obj, std::span<const Type>{argTypes},
                     static_cast<ArgPtrBuffer>(argptr_buffer));
  } else
    return Construct(obj);
}

template <typename... Args>
ObjectView ReflMngr::MNonCopiedArgNew(Type type,
                                      std::pmr::memory_resource* rsrc,
                                      Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return MNonCopiedArgNew(type, rsrc, std::span<const Type>{argTypes},
                            static_cast<ArgPtrBuffer>(argptr_buffer));
  } else
    return MNonCopiedArgNew(type, rsrc);
}

template <typename... Args>
ObjectView ReflMngr::MNew(Type type, std::pmr::memory_resource* rsrc,
                          Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return MNew(type, rsrc, std::span<const Type>{argTypes},
                static_cast<ArgPtrBuffer>(argptr_buffer));
  } else
    return MNew(type, rsrc);
}

template <typename... Args>
SharedObject ReflMngr::MMakeShared(Type type, std::pmr::memory_resource* rsrc,
                                   Args&&... args) const {
  if constexpr (sizeof...(Args) > 0) {
    constexpr Type argTypes[] = {Type_of<decltype(args)>...};
    void* const argptr_buffer[] = {
        const_cast<void*>(reinterpret_cast<const void*>(&args))...};
    return MMakeShared(type, rsrc, std::span<const Type>{argTypes},
                       static_cast<ArgPtrBuffer>(argptr_buffer));
  } else
    return MMakeShared(type, rsrc);
}

template <typename... Args>
ObjectView ReflMngr::NonCopiedArgNew(Type type, Args&&... args) const {
  return MNonCopiedArgNew(type, &object_resource, std::forward<Args>(args)...);
}

template <typename... Args>
ObjectView ReflMngr::New(Type type, Args&&... args) const {
  return MNew(type, &object_resource, std::forward<Args>(args)...);
}

template <typename... Args>
SharedObject ReflMngr::MakeShared(Type type, Args&&... args) const {
  return MMakeShared(type, &object_resource, std::forward<Args>(args)...);
}

template <typename T, typename... Args>
ObjectView ReflMngr::NewAuto(Args... args) {
  static_assert(!std::is_const_v<T> && !std::is_volatile_v<T> &&
                !std::is_reference_v<T>);
  RegisterType<T>();
  AddConstructor<T, Args...>();
  return New(Type_of<T>, std::forward<Args>(args)...);
}

template <typename T, typename... Args>
SharedObject ReflMngr::MakeSharedAuto(Args... args) {
  static_assert(!std::is_const_v<T> && !std::is_volatile_v<T> &&
                !std::is_reference_v<T>);
  RegisterType<T>();
  AddConstructor<T, Args...>();
  return MakeShared(Type_of<T>, std::forward<Args>(args)...);
}
}  // namespace My::MyDRefl
