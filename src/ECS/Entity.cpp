// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Entity.inl>
#include <ECS/World.inl>

ecs::Entity::Entity(Id id, World &world) :
	m_id{ id },
	m_world{ world }
{}

ecs::Entity::operator Id() const noexcept
{
	return m_id;
}

ecs::Entity::Id ecs::Entity::getId() const noexcept
{
	return m_id;
}

void ecs::Entity::removeAllComponents()
{
	m_world.value()->m_components.removeAllComponents(m_id);
	m_world.value()->refreshEntity(m_id);
}

void ecs::Entity::enable()
{
	m_world.value()->enableEntity(m_id);
}

void ecs::Entity::disable()
{
	m_world.value()->disableEntity(m_id);
}

bool ecs::Entity::isEnabled() const
{
	return m_world.value()->isEntityEnabled(m_id);
}

bool ecs::Entity::isValid() const
{
	return m_world.has_value() && m_world.value()->isEntityValid(m_id);
}

std::string ecs::Entity::getName() const
{
	return m_world.value()->getEntityName(m_id);
}

void ecs::Entity::remove()
{
	m_world.value()->removeEntity(m_id);
}

bool ecs::Entity::operator==(Entity const &rhs) const
{
	return m_id == rhs.m_id && &m_world.value() == &rhs.m_world.value();
}

bool ecs::Entity::operator!=(Entity const &rhs) const
{
	return !operator==(rhs);
}

std::size_t ecs::Entity::Hash::operator()(Entity const &entity) const
{
	return std::hash<Entity::Id>()(entity.m_id);
}
