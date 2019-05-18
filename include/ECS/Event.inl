// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <type_traits>

template <class T>
ecs::detail::TypeId ecs::getEventTypeId() noexcept
{
	static_assert(std::is_base_of<Event, T>::value, "T must be an Event.");

	return detail::TypeInfo<Event>::getTypeId<T>();
}
