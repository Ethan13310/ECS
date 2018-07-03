// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <ECS/Detail/TypeInfo.hpp>

namespace ecs
{
	// The maximum number of Components an Entity can holds
	constexpr std::size_t MAX_COMPONENTS = 32;

	struct Component
	{
	};

	// Get the Type ID for the Component T
	template <class T>
	detail::TypeId getComponentTypeId() noexcept;
}

#include <ECS/Component.inl>
