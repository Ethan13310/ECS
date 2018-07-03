// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <bitset>

#include <ECS/Component.hpp>

namespace ecs
{
	namespace detail
	{
		class ComponentFilter
		{
		public:
			using Mask = std::bitset<MAX_COMPONENTS>;

			ComponentFilter() = default;
			~ComponentFilter() = default;

			ComponentFilter(ComponentFilter const &) = default;
			ComponentFilter(ComponentFilter &&) noexcept = default;

			ComponentFilter &operator=(ComponentFilter const &) = default;
			ComponentFilter &operator=(ComponentFilter &&) noexcept = default;

			// Check if an Entity matches the requirements
			bool check(Mask const &mask) const;

			// Make a Component required
			template <class T>
			void require();

			// Make a Component excluded
			template <class T>
			void exclude();

			// Exclude all Components that are not required
			void excludeNotRequired();

			// Exclude all Components
			void excludeAll();

			// Remove a Component from both lists
			template <class T>
			void ignore();

		private:
			// List of required components
			Mask m_required;

			// List of excluded components
			Mask m_excluded;
		};
	}
}

#include <ECS/Detail/ComponentFilter.inl>
