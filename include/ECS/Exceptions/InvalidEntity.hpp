// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <ECS/Exceptions/Exception.hpp>

namespace ecs
{
	class InvalidEntity : public Exception
	{
	public:
		InvalidEntity(std::string const &function);
		~InvalidEntity() = default;
	};
}
