// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Detail/SystemHolder.hpp>

ecs::detail::SystemHolder::~SystemHolder()
{
	removeAllSystems();
}

void ecs::detail::SystemHolder::removeAllSystems()
{
	for (auto &system : m_systems) 
	{
		if (system.second != nullptr)
		{
			system.second->shutdownEvent();
			system.second->detachAll();
		}
	}

	m_systems.clear();
	m_priorities.clear();
}

void ecs::detail::SystemHolder::removeSystemPriority(detail::TypeId id)
{
	for (auto it{ m_priorities.begin() }; it != m_priorities.end();) 
	{
		if (it->second == id) 
		{
			it = m_priorities.erase(it);
		}
		else 
		{
			++it;
		}
	}
}
