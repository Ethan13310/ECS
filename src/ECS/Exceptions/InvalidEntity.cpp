// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Exceptions/InvalidEntity.hpp>

ecs::InvalidEntity::InvalidEntity(std::string const &function) :
	Exception{ "Invalid Entity.", function }
{}
