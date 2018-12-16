// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <functional>
#include <memory>
#include <map>
#include <unordered_map>

#include <ECS/Detail/ReferenceWrapper.hpp>
#include <ECS/Detail/TypeInfo.hpp>
#include <ECS/System.hpp>

namespace ecs::detail
{
	class SystemHolder
	{
	public:
		SystemHolder() = default;
		~SystemHolder();

		SystemHolder(SystemHolder const &) = delete;
		SystemHolder(SystemHolder &&) = default;

		SystemHolder &operator=(SystemHolder const &) = delete;
		SystemHolder &operator=(SystemHolder &&) = default;

		// Add a System
		template <class T>
		void addSystem(std::size_t priority, std::unique_ptr<T> &&system);

		// Get a System
		template <class T>
		T &getSystem();

		// Get a System
		template <class T>
		T const &getSystem() const;

		// Check whether a System exists or not
		template <class T>
		bool hasSystem() const;

		// Remove a System
		template <class T>
		void removeSystem();

		// Remove all Systems
		void removeAllSystems();

		// Iterate through all valid Systems
		template <class Func>
		void forEach(Func &&func);

	private:
		// Remove System from the priority list
		void removeSystemPriority(detail::TypeId id);

		// List of all Systems
		std::unordered_map<detail::TypeId, std::unique_ptr<System>> m_systems;

		// List of systems priorities
		std::multimap<std::size_t, detail::TypeId, std::greater<std::size_t>> m_priorities;
	};
}

#include <ECS/Detail/SystemHolder.inl>
