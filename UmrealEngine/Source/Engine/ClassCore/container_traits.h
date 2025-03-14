#pragma once
#include <array>
#include <vector>
#include <type_traits>

namespace type_utils
{
	template <typename T>
	constexpr bool is_std_array_v = false;

	template <typename T, std::size_t N>
	constexpr bool is_std_array_v<std::array<T, N>> = true;

	template <typename T>
	constexpr bool is_std_vector_v = false;

	template <typename T, typename Alloc>
	constexpr bool is_std_vector_v<std::vector<T, Alloc>> = true;
}