// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <functional>

namespace ecs::detail
{
	template <class T>
	class ReferenceWrapper
	{
	public:
		using Type = T;

		ReferenceWrapper(T &reference) noexcept;
		~ReferenceWrapper() = default;

		ReferenceWrapper(ReferenceWrapper const &) noexcept = default;
		ReferenceWrapper(ReferenceWrapper &&) noexcept = default;

		ReferenceWrapper &operator=(ReferenceWrapper const &) noexcept = default;
		ReferenceWrapper &operator=(ReferenceWrapper &&) noexcept = default;

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
}

#include <ECS/Detail/ReferenceWrapper.inl>
