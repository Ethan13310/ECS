// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <ECS/Detail/ComponentFilter.hpp>
#include <ECS/Detail/ReferenceWrapper.hpp>
#include <ECS/Detail/TypeInfo.hpp>
#include <ECS/Entity.hpp>
#include <ECS/EventDispatcher.hpp>

namespace ecs
{
	class System
	{
	public:
		virtual ~System();

		System(System const &) = delete;
		System(System &&) = default;

		System &operator=(System const &) = delete;
		System &operator=(System &&) = default;

		// Get Entities attached to this System
		std::vector<Entity> getEntities() const;

		// Get the World that the System belongs to
		World &getWorld();
		World const &getWorld() const;

		// Get Entity count
		std::size_t getEntityCount() const noexcept;

		// Detach all entities
		void detachAll();

		// Iterate through all enabled Entities
		template <class Func>
		void forEach(Func &&func);

		// Triggered on System start up
		virtual void onStart();

		// Triggered on System shut down
		virtual void onShutdown();
		
		// Triggered before each refresh, before the Entities are attached,
		// detached, enabled or disabled inside the System
		virtual void onPreUpdate(float elapsed);

		// Triggered for each refresh
		virtual void onUpdate(float elapsed);

		// Triggered when an Entity has been attached to the System
		virtual void onEntityAttached(Entity entity);

		// Triggered when an Entity has been detached from the System
		virtual void onEntityDetached(Entity entity);

		// Triggered when an Entity has been enabled
		virtual void onEntityEnabled(Entity entity);
		
		// Triggered when an Entity has been disabled
		virtual void onEntityDisabled(Entity entity);

	protected:
		// This class must be inherited
		System() = default;

		// Access to the filter
		detail::ComponentFilter &getFilter();

		// Emit Event T
		template <class T>
		void emitEvent(T const &evt) const;

		// Connect function Func to Event T
		template <class T, class Func>
		std::size_t connectEvent(Func &&func);

		// Clear connected function ID
		void disconnectEvent(std::size_t id);

		// Clear all Events
		void disconnectAllEvents();

	private:
		enum class EntityStatus
		{
			NotAttached,
			Enabled,
			Disabled
		};

		// Attach an Entity to the System
		void attachEntity(Entity const &entity);

		// Detach an Entity from the System
		void detachEntity(Entity const &entity);

		// Enable Entity
		void enableEntity(Entity const &entity);

		// Disable Entity
		void disableEntity(Entity const &entity);

		// Attach event
		void attachEvent(Entity const &entity);
		
		// Detach event
		void detachEvent(Entity const &entity);
		
		// Enable event
		void enableEvent(Entity const &entity);
		
		// Disable event
		void disableEvent(Entity const &entity);

		// Get Entity status
		EntityStatus getEntityStatus(Entity::Id id) const;

		// Set Entity status
		void setEntityStatus(Entity::Id id, EntityStatus status);

		// Enabled Entities attached to this System
		std::vector<Entity> m_enabledEntities;

		// Disabled Entities attached to this System
		std::vector<Entity> m_disabledEntities;

		// Entities status (enabled/disabled)
		std::unordered_map<Entity::Id, EntityStatus> m_status;

		// The World that this System belongs to
		std::optional<detail::ReferenceWrapper<World>> m_world;

		// The mask that the Entities must matched to be
		// attached to this System
		detail::ComponentFilter m_filter;

		// List of the Events this System is listening to
		std::unordered_set<std::size_t> m_events;

		// Only World can access detail::ComponentFilter
		friend World;
	};

	// Get the Type ID for the System T
	template <class T>
	detail::TypeId getSystemTypeId() noexcept;
}
