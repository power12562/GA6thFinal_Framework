#pragma once

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

    template <typename T>
    constexpr bool is_TProperty_v = false;

    template <typename owner_type, class getter, class setter>
    constexpr bool is_TProperty_v<TProperty<owner_type, getter, setter>> = true;
}