#pragma once

#include <cstddef>
#include <tuple>
#include <utility>
#include <iostream>
#include <cassert>

namespace core
{
	template <typename Tuple, typename F, std::size_t ...Indices>
	void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>)
	{
		using swallow = int[];
		(void)swallow {
			1,
				(f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
		};
	}

	template <typename Tuple, typename F>
	void for_each(Tuple&& tuple, F&& f)
	{
		constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
		for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<N>{});
	}
}

// From http://stackoverflow.com/questions/3767869/adding-message-to-assert
#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            assert(0); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif