// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <type_traits>

template <class T>
ecs::detail::TypeId ecs::getComponentTypeId() noexcept
{
	static_assert(std::is_base_of<Component, T>::value, "T must be a Component.");

	return detail::TypeInfo<Component>::getTypeId<T>();
}
