// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

template <class T>
void ecs::detail::ComponentFilter::require()
{
	m_required.set(getComponentTypeId<T>());
	m_excluded.reset(getComponentTypeId<T>());
}

template <class T>
void ecs::detail::ComponentFilter::exclude()
{
	m_required.reset(getComponentTypeId<T>());
	m_excluded.set(getComponentTypeId<T>());
}

template <class T>
void ecs::detail::ComponentFilter::ignore()
{
	m_required.reset(getComponentTypeId<T>());
	m_excluded.reset(getComponentTypeId<T>());
}
