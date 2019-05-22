// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <stdexcept>

#include <ECS/Entity.hpp>
#include <ECS/Exceptions/Exception.hpp>
#include <ECS/Exceptions/InvalidEntity.hpp>
#include <ECS/Log.hpp>
#include <ECS/World.hpp>
#include <ECS/Entity.inl>
#include <ECS/World.inl>

ecs::World::~World()
{
	clear();
}

void ecs::World::removeAllSystems()
{
	m_systems.removeAllSystems();
}

ecs::Entity ecs::World::createEntity()
{
	auto const id{ m_pool.create() };

	// Resize containers if necessary
	extend(id + 1);

	// Entity
	m_entities[id].entity = Entity{ id, *this };

	// Attributes
	m_entities[id].isValid = true;
	m_entities[id].isEnabled = true;

	enableEntity(m_entities[id].entity);

	return m_entities[id].entity;
}

ecs::Entity ecs::World::createEntity(std::string const &name)
{
	if (m_names.find(name) != m_names.end()) {
		throw Exception{ "Entity name already used.", "ecs::World::createEntity()" };
	}

	auto const entity{ createEntity() };

	m_names[name] = entity.getId();
	m_entities[entity.getId()].name = name;

	return entity;
}

std::optional<ecs::Entity> ecs::World::getEntity(Entity::Id id) const
{
	if (!isEntityValid(id)) {
		return std::nullopt;
	}

	return m_entities[id].entity;
}

std::optional<ecs::Entity> ecs::World::getEntity(std::string const &name) const
{
	auto const it{ m_names.find(name) };

	if (it == m_names.end()) {
		return std::nullopt;
	}

	return getEntity(it->second);
}

std::string ecs::World::getEntityName(Entity::Id id) const
{
	if (!isEntityValid(id)) {
		throw InvalidEntity{ "ecs::World::getEntityName()" };
	}

	if (m_entities[id].name.has_value()) {
		return m_entities[id].name.value();
	}

	return {};
}

void ecs::World::enableEntity(Entity::Id id)
{
	if (!isEntityValid(id)) {
		throw InvalidEntity{ "ecs::World::enableEntity()" };
	}

	m_actions.push_back({ id, EntityAction::Action::Enable });
}

void ecs::World::disableEntity(Entity::Id id)
{
	if (!isEntityValid(id)) {
		throw InvalidEntity{ "ecs::World::disableEntity()" };
	}

	m_actions.push_back({ id, EntityAction::Action::Disable });
}

void ecs::World::refreshEntity(Entity::Id id)
{
	if (!isEntityValid(id)) {
		throw InvalidEntity{ "ecs::World::refreshEntity()" };
	}

	m_actions.push_back({ id, EntityAction::Action::Refresh });
}

bool ecs::World::isEntityEnabled(Entity::Id id) const
{
	return isEntityValid(id) && m_entities[id].isEnabled;
}

void ecs::World::removeEntity(Entity::Id id)
{
	if (!isEntityValid(id)) {
		throw InvalidEntity{ "ecs::World::removeEntity()" };
	}

	m_actions.push_back({ id, EntityAction::Action::Remove });
}

void ecs::World::removeAllEntities()
{
	for (auto const &entity : m_entities) {
		// We may iterate through invalid entities
		if (entity.isValid) {
			removeEntity(entity.entity);
		}
	}
}

bool ecs::World::isEntityValid(Entity::Id id) const
{
	return id < m_entities.size() && m_entities[id].isValid;
}

void ecs::World::update(float elapsed)
{
	// Start new Systems
	for (auto &system : m_newSystems) {
		system->startEvent();
	}

	m_newSystems.clear();

	updateSystems([elapsed](System &system, detail::TypeId) {
		system.updateEvent(elapsed);
	});

	updateSystems([elapsed](System &system, detail::TypeId) {
		system.postUpdateEvent(elapsed);
	});
}

void ecs::World::clear()
{
	removeAllSystems();

	m_entities.clear();
	m_actions.clear();
	m_names.clear();

	m_evtDispatcher.clearAll();
	m_components.clear();
	m_pool.reset();
}

void ecs::World::updateEntities()
{
	// Here, we move m_actions to another vector to make possible to create, enable, etc.
	// Entities within event handlers like system::onEntityAttached, etc.
	auto const actionsList{ std::move(m_actions) };
	m_actions = decltype(m_actions){};

	for (auto const &action : actionsList) {
		try {
			executeAction(action);
		}
		catch (std::exception const &e) {
			Log::error(e.what());
		}
	}
}

void ecs::World::executeAction(EntityAction const &action)
{
	if (!isEntityValid(action.id)) {
		throw InvalidEntity{ "ecs::World::executeAction()" };
	}

	switch (action.action) {
	case EntityAction::Action::Enable:
		actionEnable(action.id);
		break;

	case EntityAction::Action::Disable:
		actionDisable(action.id);
		break;

	case EntityAction::Action::Refresh:
		actionRefresh(action.id);
		break;

	case EntityAction::Action::Remove:
		actionRemove(action.id);
		break;
	}
}

void ecs::World::actionEnable(Entity::Id id)
{
	m_systems.forEach([&](System &system, detail::TypeId systemId) {
		auto const status{ tryAttach(system, systemId, id) };

		if (status == AttachStatus::AlreadyAttached || status == AttachStatus::Attached) {
			// The Entity is attached to the System, we enable it
			system.enableEntity(m_entities[id].entity);
		}
	});
}

void ecs::World::actionDisable(Entity::Id id)
{
	m_entities[id].isEnabled = false;

	m_systems.forEach([&](System &system, detail::TypeId systemId) {
		// Is the Entity attached to the System ?
		if (systemId < m_entities[id].systems.size() && m_entities[id].systems[systemId]) {
			system.disableEntity(m_entities[id].entity);
		}
	});
}

void ecs::World::actionRefresh(Entity::Id id)
{
	m_systems.forEach([&](System &system, detail::TypeId systemId) {
		auto const status{ tryAttach(system, systemId, id) };

		if (m_entities[id].isEnabled && status == AttachStatus::Attached) {
			// If the Entity has been attached and is enabled,
			// we enable it into the System
			system.enableEntity(m_entities[id].entity);
		}
	});
}

void ecs::World::actionRemove(Entity::Id id)
{
	m_systems.forEach([&](System &system, detail::TypeId systemId) {
		// Is the Entity attached to the System ?
		if (systemId < m_entities[id].systems.size() && m_entities[id].systems[systemId]) {
			system.detachEntity(m_entities[id].entity);
			m_entities[id].systems[systemId] = false;
		}
	});

	// Invalidate the Entity and reset its attributes
	m_entities[id].isValid = false;
	m_entities[id].systems.clear();

	// Remove its name from the list
	if (m_entities[id].name.has_value()) {
		m_names.erase(m_entities[id].name.value());
		m_entities[id].name.reset();
	}

	m_components.removeAllComponents(id);
	m_pool.store(id);
}

ecs::World::AttachStatus ecs::World::tryAttach(System &system, detail::TypeId systemId, Entity::Id id)
{
	// Does the Entity match the requirements to be part of the System ?
	if (system.getFilter().check(m_components.getComponentsMask(id))) {
		// Is the Entity not already attached to the System ?
		if (systemId >= m_entities[id].systems.size() || !m_entities[id].systems[systemId]) {
			if (systemId >= m_entities[id].systems.size()) {
				m_entities[id].systems.resize(systemId + 1, false);
			}

			m_entities[id].systems[systemId] = true;
			system.attachEntity(m_entities[id].entity);

			// The Entity has been attached to the System
			return AttachStatus::Attached;
		}

		// Otherwise, if the Entity is already attached to the System
		return AttachStatus::AlreadyAttached;
	}
	// If the Entity is already attached to the System but doest not
	// match the requirements anymore, we detach it from the System
	else if (systemId < m_entities[id].systems.size() && m_entities[id].systems[systemId]) {
		system.detachEntity(m_entities[id].entity);
		m_entities[id].systems[systemId] = false;

		// The Entity has been detached from the System
		return AttachStatus::Detached;
	}

	// Nothing happened because the Entity is not attached to the System
	// and does not match the requirements to be part of it
	return AttachStatus::NotAttached;
}

void ecs::World::extend(std::size_t size)
{
	if (size > m_entities.size()) {
		m_entities.resize(size);
		m_components.resize(size);
	}
}
