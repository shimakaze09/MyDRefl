#pragma once

#include "../Util.h"

#include <set>

namespace My::MyDRefl {
// TODO: list, forward_list, stack, queue

// RawArray -> Array -> Vector
// MultiSet -> Set
// MultiSet -> MultiMap -> Map
// UnorderedMultiSet -> UnorderedSet
// UnorderedMultiSet -> UnorderedMultiMap -> UnorderedMap
// Tuple -> Pair
enum class ContainerType {
  RawArray,
  Array,
  Vector,
  Deque,
  ForwardList,
  List,
  Set,
  MultiSet,
  Map,
  MultiMap,
  UnorderedSet,
  UnorderedMultiSet,
  UnorderedMap,
  UnorderedMultiMap,
  Stack,
  Queue,
  Tuple,
  Pair,
};
MY_MYDREFL_ENUM_BOOL_OPERATOR_DEFINE(ContainerType)

// SpecializeIsSet<std::set<...>> is std::true_type
template <typename T>
struct SpecializeIsSet : std::false_type {};

// traits
// ref : https://zh.cppreference.com/w/cpp/container
/*
	template<typename T>
	concept Is* = ...;
	*/
}  // namespace My::MyDRefl

#include "details/ContainerType.inl"
