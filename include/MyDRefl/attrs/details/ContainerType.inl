#pragma once

#include "../../Util.h"

#include <set>

template <typename Key, typename Compare, typename Allocator>
struct My::MyDRefl::SpecializeIsSet<std::set<Key, Compare, Allocator>>
    : std::true_type {};

namespace My::MyDRefl {
template <typename T>
concept IsArray =
    true && container_begin<T> && container_begin<const T> &&
    container_cbegin<T>

    && container_end<T> && container_end<const T> && container_cend<T>

    && container_rbegin<T> && container_rbegin<const T> && container_crbegin<T>

    && container_rend<T> && container_rend<const T> && container_crend<T>

    && container_at_size<T> && container_at_size<const T>

    && container_subscript_size<T> && container_subscript_size<const T>

    && container_data<T> && container_data<const T>

    && container_front<T> && container_front<const T>

    && container_back<T> && container_back<const T>

    && container_empty<T> && container_size<T>

    && container_swap<T>;

template <typename T>
concept IsVector =
    IsArray<T> && container_resize_cnt<T> && container_resize_cnt_value<T> &&
    container_capacity<T> && container_reserve<T> && container_shrink_to_fit<T>

    && container_clear<T> && container_insert_citer_clvalue<T> &&
    container_insert_citer_rvalue<T> && container_insert_citer_size_value<T> &&
    container_erase_citer<T> && container_erase_range_citer<T> &&
    container_push_back_clvalue<T> && container_push_back_rvalue<T> &&
    container_pop_back<T>

    && container_get_allocator<T>;

template <typename T>
concept IsDeque =
    true && container_begin<T> && container_begin<const T> &&
    container_cbegin<T>

    && container_end<T> && container_end<const T> && container_cend<T>

    && container_rbegin<T> && container_rbegin<const T> && container_crbegin<T>

    && container_rend<T> && container_rend<const T> && container_crend<T>

    && container_at_size<T> && container_at_size<const T>

    && container_subscript_size<T> && container_subscript_size<const T>

    && container_front<T> && container_front<const T>

    && container_back<T> && container_back<const T>

    && container_empty<T> && container_size<T>

    && container_resize_cnt<T> && container_resize_cnt_value<T> &&
    container_shrink_to_fit<T> && container_clear<T> &&
    container_insert_citer_clvalue<T> && container_insert_citer_rvalue<T> &&
    container_insert_citer_size_value<T> && container_erase_citer<T> &&
    container_erase_range_citer<T> && container_push_front_clvalue<T> &&
    container_push_front_rvalue<T> && container_pop_front<T> &&
    container_push_back_clvalue<T> && container_push_back_rvalue<T> &&
    container_pop_back<T>

    && container_swap<T>

    && container_get_allocator<T>;

// TODO : list

template <typename T>
concept IsList = false;

template <typename T>
concept IsForwardList = false;

template <typename T>
concept IsMultiSet =
    true && container_begin<T> && container_begin<const T> &&
    container_cbegin<T>

    && container_end<T> && container_end<const T> && container_cend<T>

    && container_rbegin<T> && container_rbegin<const T> && container_crbegin<T>

    && container_rend<T> && container_rend<const T> && container_crend<T>

    && container_empty<T> && container_size<T>

    && container_clear<T> && container_insert_clvalue<T> &&
    container_insert_rvalue<T> && container_insert_rnode<T> &&
    container_insert_citer_clvalue<T> && container_insert_citer_rvalue<T> &&
    container_insert_citer_size_value<T> && container_insert_citer_rnode<T>

    && container_erase_citer<T> && container_erase_key<T> &&
    container_erase_range_citer<T>

    && container_swap<T> && container_merge_l<T> && container_merge_r<T> &&
    container_extract_citer<T> && container_extract_key<T>

    && container_count<T> && container_find<T> && container_find<const T> &&
    container_contains<T> && container_lower_bound<T> &&
    container_lower_bound<const T> && container_upper_bound<T> &&
    container_upper_bound<const T> && container_equal_range<T> &&
    container_equal_range<const T>

    && container_key_comp<T> && container_value_comp<T>

    && container_get_allocator<T>;

template <typename T>
concept IsSet = IsMultiSet<T> && SpecializeIsSet<T>::value;

template <typename T>
concept IsMultiMap = IsMultiSet<T> && container_mapped_type<T>;

template <typename T>
concept IsMap =
    IsMultiMap<T> && container_at_key<T> && container_at_key<const T> &&
    container_subscript_key_cl<T> && container_subscript_key_r<T>;

template <typename T>
concept IsUnorderedMultiSet =
    true && container_begin<T> && container_begin<const T> &&
    container_cbegin<T>

    && container_end<T> && container_end<const T> && container_cend<T>

    && container_empty<T> && container_size<T> && container_bucket_count<T> &&
    container_reserve<T>

    && container_clear<T> && container_insert_clvalue<T> &&
    container_insert_rvalue<T> && container_insert_rnode<T> &&
    container_insert_citer_clvalue<T> && container_insert_citer_rvalue<T> &&
    container_insert_citer_size_value<T> && container_insert_citer_rnode<T>

    && container_erase_citer<T> && container_erase_key<T> &&
    container_erase_range_citer<T>

    && container_swap<T> && container_merge_l<T> && container_merge_r<T> &&
    container_extract_citer<T> && container_extract_key<T>

    && container_count<T> && container_find<T> && container_find<const T> &&
    container_contains<T> && container_equal_range<T> &&
    container_equal_range<const T>

    && container_hash_function<T> && container_key_eq<T>

    && container_get_allocator<T>;

template <typename T>
concept IsUnorderedSet = IsUnorderedMultiSet<T>;

template <typename T>
concept IsUnorderedMultiMap =
    IsUnorderedMultiSet<T> && container_mapped_type<T>;

template <typename T>
concept IsUnorderedMap =
    IsUnorderedMultiMap<T> && container_at_key<T> &&
    container_at_key<const T> && container_subscript_key_cl<T> &&
    container_subscript_key_r<T>;

template <typename T>
concept IsTuple = true && tuple_size<T> && container_swap<T>;

template <typename T>
concept IsPair = IsTuple<T> && pair_first<T> && pair_second<T>;

// TODO : Stack, Queue

template <typename T>
concept IsStack = false;

template <typename T>
concept IsQueue = false;
}  // namespace My::MyDRefl
