// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <type_traits>
#include <utility>

template <class T>
void ecs::EventDispatcher::emit(T const &evt) const
{
	static_assert(std::is_base_of<Event, T>::value, "T must be an Event");

	auto const range{ m_listeners.equal_range(getEventTypeId<T>()) };

	for (auto it{ range.first }; it != range.second; ++it) {
		it->second.func(&evt);
	}
}

template <class T, class Func>
std::size_t ecs::EventDispatcher::connect(Func &&func)
{
	static_assert(std::is_base_of<Event, T>::value, "T must be an Event");

	auto slot = [func](void const *evt) {
		func(*static_cast<T const *>(evt));
	};

	// Event handler ID
	auto const id{ m_nextId };
	++m_nextId;

	m_listeners.insert({ getEventTypeId<T>(), EventReceiverAttributes{ id, slot } });

	return id;
}

template <class T>
void ecs::EventDispatcher::clear()
{
	static_assert(std::is_base_of<Event, T>::value, "T must be an Event");

	m_listeners.erase(getEventTypeId<T>());
}
