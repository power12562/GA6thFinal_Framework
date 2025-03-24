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
}

namespace hash_utils
{
    //hared_ptr<T> 내부의 raw 포인터(T*)를 기준으로 해싱하는 함수객체
    struct SharedPtrHash
    {
        template <typename T>
        std::size_t operator()(const std::shared_ptr<T>& ptr) const
        {
            return std::hash<T*>()(ptr.get()); 
        }
    };

    struct SharedPtrEqual
    {
        // shared_ptr<T> 와 shared_ptr<T> 비교
        template <typename T>
        bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const
        {
            return lhs.get() == rhs.get();
        }
    };
}