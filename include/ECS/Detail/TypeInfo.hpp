// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <cstddef>

namespace ecs::detail
{
	using TypeId = std::size_t;

	template <class BaseT>
	class TypeInfo
	{
	public:
		TypeInfo() = delete;

		// Get the type ID of T which is a base of BaseT
		template <class T>
		static TypeId getTypeId() noexcept;

	private:
		// Get the next type ID for BaseT
		static TypeId nextTypeId() noexcept;

		// Next type ID for BaseT
		static TypeId m_nextTypeId;
	};

	template <class BaseT>
	TypeId TypeInfo<BaseT>::m_nextTypeId{ 0 };
}

#include <ECS/Detail/TypeInfo.inl>
