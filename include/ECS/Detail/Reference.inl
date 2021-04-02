// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <memory>
#include <utility>

template <class T>
ecs::detail::Reference<T>::Reference(T &reference) noexcept :
	m_reference{ std::addressof(reference) }
{}

template <class T>
T *ecs::detail::Reference<T>::operator->() const noexcept
{
	return m_reference;
}

template <class T>
T *ecs::detail::Reference<T>::operator&() const noexcept
{
	return m_reference;
}

template <class T>
ecs::detail::Reference<T>::operator T&() const noexcept
{
	return *m_reference;
}

template <class T>
T &ecs::detail::Reference<T>::get() const noexcept
{
	return *m_reference;
}

template <class T>
template <class... Args>
std::invoke_result_t<T&, Args...> ecs::detail::Reference<T>::operator()(Args &&...args) const
{
	return std::invoke(get(), std::forward<Args>(args)...);
}
