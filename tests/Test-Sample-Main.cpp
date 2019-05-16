// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <functional>
#include <vector>

#include <ECS.hpp>
#include <lest/lest.hpp>

using namespace ecs;

struct BasicComponent : public Component
{
	BasicComponent(int val = 0) : value{ val } {}

	int value;
};

struct MissingComponent : public Component
{};

class BasicSystem : public System
{
public:
	BasicSystem()
	{
		getFilter().require<BasicComponent>();
	}

	void onUpdate(float)
	{
		auto entities{ getEntities() };

		for (auto &entity : entities) {
			entity.getComponent<BasicComponent>().value += 1;
		}
	}
};

class MissingSystem : public System
{
public:
	MissingSystem() = default;
};

lest::test const specification[] =
{
	CASE("Basic ECS usage")
	{
		World world;

		world.addSystem<BasicSystem>(0);

		EXPECT(world.hasSystem<BasicSystem>());
		EXPECT_NOT(world.hasSystem<MissingSystem>());

		std::vector<Entity> entities{
			world.createEntity(),
			world.createEntity()
		};

		EXPECT(entities[0].getId() == 0);
		EXPECT(entities[1].getId() == 1);

		EXPECT(entities[0].isValid());
		EXPECT(entities[1].isValid());

		entities[0].addComponent<BasicComponent>(0);
		entities[1].addComponent<BasicComponent>(10);

		EXPECT(entities[0].hasComponent<BasicComponent>());
		EXPECT(entities[1].hasComponent<BasicComponent>());

		EXPECT_NOT(entities[0].hasComponent<MissingComponent>());
		EXPECT_NOT(entities[1].hasComponent<MissingComponent>());

		std::vector<std::reference_wrapper<BasicComponent>> components{
			entities[0].getComponent<BasicComponent>(),
			entities[1].getComponent<BasicComponent>()
		};

		EXPECT(components[0].get().value == 0);
		EXPECT(components[1].get().value == 10);

		for (std::size_t i{ 0 }; i < 5; ++i) {
			world.update(0);
		}

		EXPECT(components[0].get().value == 5);
		EXPECT(components[1].get().value == 15);
	}
};

int main(int argc, char **argv)
{
	return lest::run(specification, argc, argv);
}
