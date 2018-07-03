// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <type_traits>

template <class Func>
void ecs::System::forEach(Func &&func)
{
	for (auto const &entity : m_enabledEntities) {
		if (entity.isValid()) {
			func(entity);
		}
	}
}

template <class T>
void ecs::System::emitEvent(T const &evt) const
{
	getWorld().m_evtDispatcher.emit(evt);
}

template <class T, class Func>
std::size_t ecs::System::connectEvent(Func &&func)
{
	auto const id{ getWorld().m_evtDispatcher.connect<T>(std::forward<Func>(func)) };

	// Save connection ID
	m_events.insert(id);

	return id;
}

template <class T>
ecs::detail::TypeId ecs::getSystemTypeId() noexcept
{
	static_assert(std::is_base_of<System, T>::value, "T must be a System");

	return detail::TypeInfo<System>::getTypeId<T>();
}
