// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS/Exceptions/InvalidComponent.hpp>

ecs::InvalidComponent::InvalidComponent(std::string const &function) :
	Exception{ "Invalid Component.", function }
{}
