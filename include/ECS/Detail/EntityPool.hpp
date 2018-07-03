// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <vector>

#include <ECS/Entity.hpp>

namespace ecs
{
	namespace detail
	{
		class EntityPool
		{
		public:
			EntityPool() = default;
			~EntityPool() = default;

			EntityPool(EntityPool const &) = delete;
			EntityPool(EntityPool &&) noexcept = default;

			EntityPool &operator=(EntityPool const &) = delete;
			EntityPool &operator=(EntityPool &&) noexcept = default;

			// Create an Entity ID
			Entity::Id create();

			// Store an Entity ID
			void store(Entity::Id id);

			// Remove all Entity IDs stored within the pool and
			// reset the next Entity ID value
			void reset() noexcept;

		private:
			// List of stored Entities IDs
			std::vector<Entity::Id> m_storedIds;

			// Next Entity ID
			Entity::Id m_nextId{ 0 };
		};
	}
}
