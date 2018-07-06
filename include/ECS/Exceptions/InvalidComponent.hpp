// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <ECS/Exceptions/Exception.hpp>

namespace ecs
{
	class InvalidComponent : public Exception
	{
	public:
		InvalidComponent(std::string const &function);
		~InvalidComponent() = default;
	};
}
