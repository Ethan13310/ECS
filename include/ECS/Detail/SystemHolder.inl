// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <algorithm>
#include <stdexcept>

#include <ECS/Exceptions/Exception.hpp>
#include <ECS/Log.hpp>

template <class T>
void ecs::detail::SystemHolder::addSystem(std::size_t priority, std::unique_ptr<T> &&system)
{
	// Remove previous System, if exists
	removeSystem<T>();

	auto const typeId{ getSystemTypeId<T>() };

	// Insert the priority value
	m_priorities.insert({ priority, typeId });

	// Then, add the System
	m_systems[typeId] = std::move(system);
}

template <class T>
T &ecs::detail::SystemHolder::getSystem()
{
	auto it{ m_systems.find(getSystemTypeId<T>()) };

	if (it == m_systems.end() || it->second == nullptr) {
		throw Exception{ "World does not have this System.", "ecs::World::getSystem()" };
	}

	return *static_cast<T*>(it->second.get());
}

template <class T>
T const &ecs::detail::SystemHolder::getSystem() const
{
	auto it{ m_systems.find(getSystemTypeId<T>()) };

	if (it == m_systems.end() || it->second == nullptr) {
		throw Exception{ "World does not have this System.", "ecs::World::getSystem()" };
	}

	return *static_cast<T*>(it->second.get());
}

template <class T>
bool ecs::detail::SystemHolder::hasSystem() const
{
	auto const it{ m_systems.find(getSystemTypeId<T>()) };

	return it != m_systems.end() && it->second != nullptr;
}

template <class T>
void ecs::detail::SystemHolder::removeSystem()
{
	auto const typeId{ getSystemTypeId<T>() };

	auto system{ m_systems.find(typeId) };

	if (system != m_systems.end() && system->second != nullptr) {
		system->second->onShutdown();
		system->second->detachAll();
	}

	// Remove the priority value for this System
	removeSystemPriority(typeId);

	// Then, remove the System
	m_systems.erase(typeId);
}

template <class Func>
void ecs::detail::SystemHolder::forEach(Func &&func)
{
	for (auto const &typeId : m_priorities) {
		auto &system{ m_systems[typeId.second] };

		if (system != nullptr) {
			try {
				func(*system, typeId.second);
			}
			catch (std::exception const &e) {
				Log::error(e.what());
			}
		}
	}
}
