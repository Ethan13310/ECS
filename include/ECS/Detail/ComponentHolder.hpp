// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include <ECS/Component.hpp>
#include <ECS/Detail/ComponentFilter.hpp>
#include <ECS/Detail/ReferenceWrapper.hpp>
#include <ECS/Detail/TypeInfo.hpp>
#include <ECS/Entity.hpp>

namespace ecs
{
	namespace detail
	{
		class ComponentHolder
		{
		public:
			ComponentHolder() = default;
			~ComponentHolder() = default;

			ComponentHolder(ComponentHolder const &) = delete;
			ComponentHolder(ComponentHolder &&) = default;

			ComponentHolder &operator=(ComponentHolder const &) = delete;
			ComponentHolder &operator=(ComponentHolder &&) = default;

			// Add the component T to the Entity
			template <class T>
			void addComponent(Entity::Id id, std::unique_ptr<T> &&component);

			// Get the Component T from the Entity
			template <class T>
			T &getComponent(Entity::Id id);

			// Check whether the Entity has the Component T or not
			template <class T>
			bool hasComponent(Entity::Id id) const;

			// Remove the Component T from the Entity
			template <class T>
			void removeComponent(Entity::Id id);

			// Remove all components from the Entity
			void removeAllComponents(Entity::Id id);

			// Get the Component mask for the given Entity
			ComponentFilter::Mask getComponentsMask(Entity::Id id) const;

			// Resize the Component array
			void resize(std::size_t size);

			// Clear all Components
			void clear() noexcept;

		private:
			template <class T>
			std::optional<ReferenceWrapper<std::unique_ptr<Component>>> getComponentPtr(Entity::Id id);

			// The index of this array matches the Component type ID
			using ComponentArray = std::array<std::unique_ptr<Component>, MAX_COMPONENTS>;

			// List of all Components of all Entities
			// The index of this array matches the Entity ID
			std::vector<ComponentArray> m_components;

			// List of all masks of all Composents of all Entities
			// The index of this array matches the Entity ID
			std::vector<ComponentFilter::Mask> m_componentsMasks;
		};
	}
}

#include <ECS/Detail/ComponentHolder.inl>
