// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Detail/EntityPool.hpp>

ecs::Entity::Id ecs::detail::EntityPool::create()
{
	Entity::Id id{};

	if (m_storedIds.empty()) {
		id = m_nextId;
		++m_nextId;
	}
	else {
		id = m_storedIds.back();
		m_storedIds.pop_back();
	}

	return id;
}

void ecs::detail::EntityPool::store(Entity::Id id)
{
	if (id < m_nextId) {
		// We cannot store an ID that haven't been generated before
		m_storedIds.push_back(id);
	}
}

void ecs::detail::EntityPool::reset() noexcept
{
	m_storedIds.clear();
	m_nextId = 0;
}
