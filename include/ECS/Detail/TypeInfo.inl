// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

template <class BaseT>
template <class T>
ecs::detail::TypeId ecs::detail::TypeInfo<BaseT>::getTypeId() noexcept
{
	static auto const id{ nextTypeId() };

	return id;
}

template <class BaseT>
ecs::detail::TypeId ecs::detail::TypeInfo<BaseT>::nextTypeId() noexcept
{
	auto const id{ m_nextTypeId };

	++m_nextTypeId;

	return id;
}
