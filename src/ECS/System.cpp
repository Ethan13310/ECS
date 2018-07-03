// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <algorithm>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

ecs::System::~System()
{
	disconnectAllEvents();
}

void ecs::System::detachAll()
{
	// Enabled Entities
	for (auto &entity : m_enabledEntities) {
		disableEvent(entity);
		detachEvent(entity);
	}
	
	// Disabled Entities
	for (auto &entity : m_disabledEntities) {
		detachEvent(entity);
	}

	m_enabledEntities.clear();
	m_disabledEntities.clear();

	m_status.clear();
}

void ecs::System::attachEntity(Entity const &entity)
{
	if (getEntityStatus(entity) == EntityStatus::NotAttached) {
		// Add Entity to the Disabled list
		// The Entity is not enabled by default
		m_disabledEntities.push_back(entity);

		attachEvent(entity);
		setEntityStatus(entity, EntityStatus::Disabled);
	}
}

void ecs::System::detachEntity(Entity const &entity)
{
	auto const status{ getEntityStatus(entity) };

	if (status != EntityStatus::NotAttached) {
		if (status == EntityStatus::Enabled) {
			// Remove Entity from Enabled list
			m_enabledEntities.erase(std::remove(m_enabledEntities.begin(), m_enabledEntities.end(), entity), m_enabledEntities.end());
			disableEvent(entity);
		}
		else {
			// Remove Entity from Disabled list
			m_disabledEntities.erase(std::remove(m_disabledEntities.begin(), m_disabledEntities.end(), entity), m_disabledEntities.end());
		}

		detachEvent(entity);
		setEntityStatus(entity, EntityStatus::NotAttached);
	}
}

void ecs::System::enableEntity(Entity const &entity)
{
	if (getEntityStatus(entity) == EntityStatus::Disabled) {
		// Remove Entity from Disabled list
		m_disabledEntities.erase(std::remove(m_disabledEntities.begin(), m_disabledEntities.end(), entity), m_disabledEntities.end());

		// Then, add it to the Enabled list
		m_enabledEntities.push_back(entity);

		enableEvent(entity);
		setEntityStatus(entity, EntityStatus::Enabled);
	}
}

void ecs::System::disableEntity(Entity const &entity)
{
	if (getEntityStatus(entity) == EntityStatus::Enabled) {
		// Remove Entity from Enabled list
		m_enabledEntities.erase(std::remove(m_enabledEntities.begin(), m_enabledEntities.end(), entity), m_enabledEntities.end());

		// Then, add it to the Disabled list
		m_disabledEntities.push_back(entity);

		disableEvent(entity);
		setEntityStatus(entity, EntityStatus::Disabled);
	}
}

void ecs::System::attachEvent(Entity const &entity)
{
	onEntityAttached(entity);

	printDebug("System: Entity ", entity, " attached");
}

void ecs::System::detachEvent(Entity const &entity)
{
	onEntityDetached(entity);

	printDebug("System: Entity ", entity, " detached");
}

void ecs::System::enableEvent(Entity const &entity)
{
	onEntityEnabled(entity);

	printDebug("System: Entity ", entity, " enabled");
}

void ecs::System::disableEvent(Entity const &entity)
{
	onEntityDisabled(entity);

	printDebug("System: Entity ", entity, " disabled");
}

std::vector<ecs::Entity> ecs::System::getEntities() const
{
	return m_enabledEntities;
}

std::size_t ecs::System::getEntityCount() const noexcept
{
	return m_enabledEntities.size();
}

ecs::World &ecs::System::getWorld()
{
	if (!m_world.has_value()) {
		throw std::runtime_error{ "System is not attached to any World" };
	}

	return m_world.value();
}

ecs::World const &ecs::System::getWorld() const
{
	if (!m_world.has_value()) {
		throw std::runtime_error{ "System is not attached to any World" };
	}

	return m_world.value();
}

void ecs::System::onStart()
{}

void ecs::System::onShutdown()
{}

void ecs::System::onUpdate(float)
{}

void ecs::System::onEntityAttached(Entity)
{}

void ecs::System::onEntityDetached(Entity)
{}

void ecs::System::onEntityEnabled(Entity)
{}

void ecs::System::onEntityDisabled(Entity)
{}

ecs::detail::ComponentFilter &ecs::System::getFilter()
{
	return m_filter;
}

void ecs::System::disconnectEvent(std::size_t id)
{
	if (m_events.find(id) != m_events.end()) {
		getWorld().m_evtDispatcher.clear(id);
		m_events.erase(id);
	}
}

void ecs::System::disconnectAllEvents()
{
	auto &evtDispatcher{ getWorld().m_evtDispatcher };

	for (auto id : m_events) {
		evtDispatcher.clear(id);
	}

	m_events.clear();
}

ecs::System::EntityStatus ecs::System::getEntityStatus(Entity::Id id) const
{
	auto const it{ m_status.find(id) };

	if (it != m_status.end()) {
		return it->second;
	}

	return EntityStatus::NotAttached;
}

void ecs::System::setEntityStatus(Entity::Id id, EntityStatus status)
{
	if (status == EntityStatus::NotAttached) {
		m_status.erase(id);
	}
	else {
		m_status[id] = status;
	}
}
