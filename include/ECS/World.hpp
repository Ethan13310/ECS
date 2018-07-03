// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <ECS/Component.hpp>
#include <ECS/Detail/ComponentFilter.hpp>
#include <ECS/Detail/ComponentHolder.hpp>
#include <ECS/Detail/Debug.hpp>
#include <ECS/Detail/EntityPool.hpp>
#include <ECS/Detail/ReferenceWrapper.hpp>
#include <ECS/Detail/SystemHolder.hpp>
#include <ECS/Detail/TypeInfo.hpp>
#include <ECS/Entity.hpp>
#include <ECS/EventDispatcher.hpp>
#include <ECS/System.hpp>

namespace ecs
{
	class World
	{
	public:
		World();
		~World();

		World(World const &) = delete;
		World(World &&) = delete;

		World &operator=(World const &) = delete;
		World &operator=(World &&) = delete;

		// Add a System
		template <class T, class... Args>
		T &addSystem(std::size_t priority = 0, Args &&...args);

		// Get a System
		template <class T>
		T &getSystem();

		// Check whether a System exists or not
		template <class T>
		bool hasSystem() const;

		// Remove a System
		template <class T>
		void removeSystem();

		// Remove all Systems
		void removeAllSystems();

		// Create a new Entity
		Entity createEntity();

		// Create a new named Entity
		Entity createEntity(std::string const &name);

		// Get Entity by ID
		std::optional<ecs::Entity> getEntity(Entity::Id id) const;

		// Get Entity by name
		std::optional<ecs::Entity> getEntity(std::string const &name) const;

		// Enable the Entity
		void enableEntity(Entity::Id id);

		// Disable the Entity
		void disableEntity(Entity::Id id);

		// Check whether the Entity is enabled or not
		bool isEntityEnabled(Entity::Id id) const;

		// Remove the Entity
		void removeEntity(Entity::Id id);

		// Remove all Entities
		void removeAllEntities();

		// Check whether an Entity is valid or not
		bool isEntityValid(Entity::Id id) const;

		// Get Entity name
		std::string getEntityName(Entity::Id id) const;

		// Update the World
		void update(float elapsed);

		// Clear the World by removing all Systems and Entities
		void clear();

	private:
		struct EntityAttributes
		{
			// Entity
			Entity entity;

			// Is this Entity valid (hasn't been removed)
			bool isValid{ false };

			// Is this Entity enabled
			bool isEnabled{ false };

			// Entity name
			std::optional<std::string> name;

			// The Systems this Entity is attached
			std::vector<detail::TypeId> systems;
		};

		struct EntityAction
		{
			enum class Action
			{
				Enable,
				Disable,
				Refresh,
				Remove
			};

			// Entity ID
			Entity::Id id;

			// Action to perform on this Entity
			Action action;
		};

		enum AttachStatus
		{
			Attached,
			AlreadyAttached,
			Detached,
			NotAttached
		};

		// Refresh the Entity Systems list
		void refreshEntity(Entity::Id id);

		// Update the Entities within the World (enable, disable, remove)
		void updateEntities();

		// Add Entity to the Systems it meets the requirements
		void actionEnable(Entity::Id id);

		// Remove Entity from the Systems it meets the requirements
		void actionDisable(Entity::Id id);

		// Attach the Entity to the Systems it meets the requirements or detach
		// it from the Systems it does not meet the requirements anymore
		// Used after addComponent and removeComponent
		void actionRefresh(Entity::Id id);

		// Remove Entity data from the World
		void actionRemove(Entity::Id id);

		// Checks the requirements the Entity meets for each Systems
		// Used by actionEnable and actionRefresh
		AttachStatus tryAttach(System &system, detail::TypeId systemId, Entity::Id id);

		// Extend the Entity and Component arrays
		void extend(std::size_t size);

		// List of all Entities
		std::vector<EntityAttributes> m_entities;

		// List of Entities that have been modified
		std::vector<EntityAction> m_actions;

		// List of all Entity names, associated to their Entities, for
		// faster search
		std::unordered_map<std::string, Entity::Id> m_names;

		// List of all Components of all Entities of the World
		detail::ComponentHolder m_components;

		// List of all Systems of the World
		detail::SystemHolder m_systems;

		// List of all System waiting to be started
		std::vector<detail::ReferenceWrapper<System>> m_newSystems;

		// ID Pool
		detail::EntityPool m_pool;

		// Event Dispacher
		EventDispatcher m_evtDispatcher;

		// Only Entity is able to use the detail::ComponentHolder
		friend class Entity;

		// Only System is able to use the EventDispatcher
		friend class System;
	};
}
