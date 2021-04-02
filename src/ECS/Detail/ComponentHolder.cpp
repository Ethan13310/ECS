// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Detail/ComponentHolder.hpp>

void ecs::detail::ComponentHolder::removeAllComponents(Entity::Id id)
{
	if (id < m_components.size())
	{
		for (auto &component : m_components[id])
		{
			component.reset();
		}
		
		m_componentsMasks[id].reset();
	}
}

ecs::detail::ComponentFilter::Mask ecs::detail::ComponentHolder::getComponentsMask(Entity::Id id) const
{
	if (id < m_componentsMasks.size())
	{
		return m_componentsMasks[id];
	}

	// Empty mask
	return {};
}

void ecs::detail::ComponentHolder::resize(std::size_t size)
{
	m_components.resize(size);
	m_componentsMasks.resize(size);
}

void ecs::detail::ComponentHolder::clear() noexcept
{
	m_components.clear();
	m_componentsMasks.clear();
}
