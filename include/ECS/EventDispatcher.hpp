// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

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
		Event::Id connect(Func &&func);

		// Clear all connected functions to Event T
		template <class T>
		void clear();

		// Clear connected function ID
		void clear(Event::Id id);

		// Clear all Events
		void clearAll();

	private:
		using EventReceiver = std::function<void(void const *)>;

		struct EventReceiverAttributes
		{
			Event::Id id;
			EventReceiver func;
		};

		// Lister list
		std::unordered_multimap<detail::TypeId, EventReceiverAttributes> m_listeners;

		// Next Event handler ID
		Event::Id m_nextId{ 0 };
	};
}

#include <ECS/EventDispatcher.inl>
