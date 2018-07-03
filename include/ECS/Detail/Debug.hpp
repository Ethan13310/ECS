// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

// Debug mode
#if defined(_DEBUG) || defined(ECS_DEBUG)

// Debug macro
#define printDebug(...) ecs::detail::debug(__VA_ARGS__)

namespace ecs
{
	namespace detail
	{
		// Print end of debug info
		inline void debug();

		// Print debug info
		template <class T, class... Args>
		void debug(T const &value, Args &&...args);
	}
}

#include <ECS/Detail/Debug.inl>

#else

// No debug in release mode
#define printDebug(...) do {} while (false)

#endif
