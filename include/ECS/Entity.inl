// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <memory>
#include <utility>

#include <ECS/Entity.hpp>
#include <ECS/World.hpp>

template <class T, class... Args>
T &ecs::Entity::addComponent(Args &&...args)
{
	m_world.value()->m_components.addComponent<T>(m_id, std::make_unique<T>(std::forward<Args>(args)...));
	m_world.value()->refreshEntity(m_id);

	return getComponent<T>();
}

template <class T>
T &ecs::Entity::getComponent()
{
	return m_world.value()->m_components.getComponent<T>(m_id);
}

template <class T>
T const &ecs::Entity::getComponent() const
{
	return m_world.value()->m_components.getComponent<T>(m_id);
}

template <class T>
bool ecs::Entity::hasComponent() const
{
	return m_world.value()->m_components.hasComponent<T>(m_id);
}

template <class T>
void ecs::Entity::removeComponent()
{
	m_world.value()->m_components.removeComponent<T>(m_id);
	m_world.value()->refreshEntity(m_id);
}
