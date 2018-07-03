// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <iostream>
#include <utility>

inline void ecs::detail::debug()
{
	std::cout << std::endl;
}

template <class T, class... Args>
void ecs::detail::debug(T const &value, Args &&...args)
{
	std::cout << value;

	debug(std::forward<Args>(args)...);
}
