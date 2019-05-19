// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <functional>
#include <optional>

namespace ecs::detail
{
	template <class T>
	class Reference
	{
	public:
		using Type = T;

		Reference(T &reference) noexcept;
		~Reference() = default;

		Reference(Reference const &) noexcept = default;
		Reference(Reference &&) noexcept = default;

		Reference &operator=(Reference const &) noexcept = default;
		Reference &operator=(Reference &&) noexcept = default;

		// Call the function of the stored reference
		T *operator->() const noexcept;

		// Get the address of the stored reference
		T *operator&() const noexcept;

		// Access the stored reference
		operator T&() const noexcept;

		// Access the stored reference
		T &get() const noexcept;

		// Call the stored function
		template <class... Args>
		std::invoke_result_t<T&, Args...> operator()(Args &&...args) const;

	private:
		// Address of the referenced object
		T *m_reference{ nullptr };
	};

	template <class T>
	using OptionalReference = std::optional<Reference<T>>;
}

#include <ECS/Detail/Reference.inl>
