// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/EventDispatcher.hpp>

void ecs::EventDispatcher::clearAll()
{
	m_listeners.clear();
}

void ecs::EventDispatcher::clear(Event::Id id)
{
	for (auto it{ m_listeners.begin() }; it != m_listeners.end();) {
		if (it->second.id == id) {
			it = m_listeners.erase(it);
		}
		else {
			++it;
		}
	}
}
