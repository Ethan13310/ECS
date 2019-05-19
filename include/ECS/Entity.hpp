// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <optional>

#include <ECS/Component.hpp>
#include <ECS/Detail/Reference.hpp>

namespace ecs
{
	class World;

	class Entity
	{
	public:
		// Entity ID type
		using Id = std::size_t;

		class Hash
		{
		public:
			// Compute Entity hash
			std::size_t operator()(Entity const &entity) const;
		};

		Entity() = default;
		~Entity() = default;

		Entity(Id id, World &world);

		Entity(Entity const &) = default;
		Entity(Entity &&) noexcept = default;

		Entity &operator=(Entity const &) = default;
		Entity &operator=(Entity &&) noexcept = default;

		// Cast Entity into its ID
		operator Id() const noexcept;

		// Get the Entity ID
		Id getId() const noexcept;

		// Add the Component T to the Entity
		template <class T, class... Args>
		T &addComponent(Args &&...args);

		// Get the Component T from the Entity
		template <class T>
		T &getComponent();

		// Get the Component T from the Entity
		template <class T>
		T const &getComponent() const;

		// Check whether the Entity has the Component T or not
		template <class T>
		bool hasComponent() const;

		// Remove the Component T from the Entity
		template <class T>
		void removeComponent();

		// Remove all components from the Entity
		void removeAllComponents();

		// Enable the Entity
		void enable();

		// Disable the Entity
		void disable();

		// Check whether the Entity is enabled or not
		bool isEnabled() const;

		// Check whether the Entity is valid or not
		bool isValid() const;

		// Get Entity name
		std::string getName() const;

		// Remove the Entity
		void remove();

		bool operator==(Entity const &rhs) const;
		bool operator!=(Entity const &rhs) const;

	private:
		// Entity ID
		Id m_id{ 0 };

		// The World that this Entity belongs to
		detail::OptionalReference<World> m_world;
	};
}
