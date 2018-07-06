// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <memory>
#include <utility>

#include <ECS/World.hpp>

template <class T, class... Args>
T &ecs::World::addSystem(std::size_t priority, Args &&...args)
{
	m_systems.addSystem<T>(priority, std::make_unique<T>(std::forward<Args>(args)...));

	m_newSystems.emplace_back(getSystem<T>());

	// Set System's World
	getSystem<T>().m_world = *this;

	return getSystem<T>();
}

template <class T>
T &ecs::World::getSystem()
{
	return m_systems.getSystem<T>();
}

template <class T>
bool ecs::World::hasSystem() const
{
	return m_systems.hasSystem<T>();
}

template <class T>
void ecs::World::removeSystem()
{
	m_systems.removeSystem<T>();
}

template <class Func>
void ecs::World::updateSystems(Func &&func)
{
	updateEntities();

	m_systems.forEach(std::forward<Func>(func));
}
