// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <ECS/Detail/TypeInfo.hpp>

namespace ecs
{
	struct Event
	{
		using Id = std::size_t;
	};

	// Get the Type ID for the Event T
	template <class T>
	detail::TypeId getEventTypeId() noexcept;
}

#include <ECS/Event.inl>
