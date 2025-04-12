#pragma once

#include <cstddef>

namespace My::MyDRefl {
static constexpr std::size_t MaxArgNum = 64;
static_assert(MaxArgNum <= 256);
}  // namespace My::MyDRefl
