// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Detail/ComponentFilter.hpp>

void ecs::detail::ComponentFilter::excludeAll()
{
	m_required.reset();
	m_excluded.set();
}

void ecs::detail::ComponentFilter::excludeNotRequired()
{
	m_excluded = ~m_required;
}

bool ecs::detail::ComponentFilter::check(Mask const &mask) const
{
	auto const excludeMask{ m_excluded & mask };
	
	// Check if there is an excluded component
	if (excludeMask.any()) {
		return false;
	}

	for (std::size_t i{ 0 }; i < m_required.size(); ++i) {
		if (m_required[i] && !mask[i]) {
			// A required component is missing
			return false;
		}
	}

	return true;
}
