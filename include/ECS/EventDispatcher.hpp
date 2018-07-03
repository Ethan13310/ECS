// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <functional>
#include <unordered_map>

#include <ECS/Detail/TypeInfo.hpp>
#include <ECS/Event.hpp>

namespace ecs
{
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;
		~EventDispatcher() = default;

		EventDispatcher(EventDispatcher const &) = delete;
		EventDispatcher(EventDispatcher &&) = default;

		EventDispatcher &operator=(EventDispatcher const &) = delete;
		EventDispatcher &operator=(EventDispatcher &&) = default;

		// Emit Event T
		template <class T>
		void emit(T const &evt = T{}) const;

		// Connect function Func to Event T
		template <class T, class Func>
		std::size_t connect(Func &&func);

		// Clear all connected functions to Event T
		template <class T>
		void clear();

		// Clear connected function ID
		void clear(std::size_t id);

		// Clear all Events
		void clearAll();

	private:
		using EventReceiver = std::function<void(void const *)>;

		struct EventReceiverAttributes
		{
			std::size_t id;
			EventReceiver func;
		};

		// Lister list
		std::unordered_multimap<detail::TypeId, EventReceiverAttributes> m_listeners;

		// Next Event handler ID
		std::size_t m_nextId{ 0 };
	};
}

#include <ECS/EventDispatcher.inl>
