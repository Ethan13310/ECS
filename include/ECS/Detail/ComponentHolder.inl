// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <ECS/Exceptions/Exception.hpp>
#include <ECS/Exceptions/InvalidComponent.hpp>
#include <ECS/Exceptions/InvalidEntity.hpp>

template <class T>
T &ecs::detail::ComponentHolder::addComponent(Entity::Id id, std::unique_ptr<T> &&component)
{
	if (id >= m_components.size()) {
		// The Entity ID is out of range
		throw InvalidEntity{ "ecs::Entity::addComponent()" };
	}

	auto const typeId{ getComponentTypeId<T>() };

	if (typeId >= m_components[id].size()) {
		// The Component type ID is out of range
		throw InvalidComponent{ "ecs::Entity::addComponent()" };
	}

	m_components[id][typeId] = std::move(component);
	m_componentsMasks[id].set(typeId);

	return *static_cast<T*>(m_components[id][typeId].get());
}

template <class T>
T &ecs::detail::ComponentHolder::getComponent(Entity::Id id)
{
	auto component{ getComponentPtr<T>(id) };

	if (!component.has_value() || component.value().get() == nullptr) {
		// The Component does not exist
		throw Exception{ "Entity does not have this Component.", "ecs::Entity::getComponent()" };
	}

	return *static_cast<T*>(component.value()->get());
}

template <class T>
bool ecs::detail::ComponentHolder::hasComponent(Entity::Id id) const
{
	// Is the Entity ID and the Component type ID known
	if (id < m_components.size()) {
		auto const typeId{ getComponentTypeId<T>() };

		// Is the Component type ID known
		if (typeId < m_components[id].size()) {
			return m_components[id][typeId] != nullptr;
		}
	}

	return false;
}

template <class T>
void ecs::detail::ComponentHolder::removeComponent(Entity::Id id)
{
	auto component{ getComponentPtr<T>(id) };

	if (component.has_value()) {
		// The Component exists, we remove it
		component.value()->reset();
		m_componentsMasks[id].reset(getComponentTypeId<T>());
	}
}

template <class T>
ecs::detail::OptionalReference<std::unique_ptr<ecs::Component>> ecs::detail::ComponentHolder::getComponentPtr(Entity::Id id)
{
	if (!hasComponent<T>(id)) {
		// The Component does not exist
		return std::nullopt;
	}

	return m_components[id][getComponentTypeId<T>()];
}
