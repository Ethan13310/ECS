// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Exceptions/InvalidEntity.hpp>

ecs::InvalidEntity::InvalidEntity(std::string const &function) :
	Exception{ "Invalid Entity.", function }
{}
