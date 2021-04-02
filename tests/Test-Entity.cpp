// Copyright (c) 2021 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT License - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <memory>

#include <ECS.hpp>
#include <lest/lest.hpp>

#ifdef _MSC_VER
	#pragma warning(disable: 4702)
#endif

struct A : public ecs::Component {};
struct B : public ecs::Component {};
struct C : public ecs::Component {};

template <class T>
void const *addressOf(T const& instance)
{
	return static_cast<void const*>(std::addressof(instance));
}

lest::test const specification[] =
{
	CASE("Entity creation")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		EXPECT(entity.isValid());
		EXPECT(entity.isEnabled());
	},

	CASE("Entity deactivation and activation")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		entity.disable();
		world.update(0);

		EXPECT(entity.isValid());
		EXPECT_NOT(entity.isEnabled());

		entity.enable();
		world.update(0);

		EXPECT(entity.isValid());
		EXPECT_NOT(entity.isEnabled());
	},

	CASE("Entity destruction")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		entity.remove();
		world.update(0);

		EXPECT_NOT(entity.isValid());
		EXPECT_NOT(entity.isEnabled());

		EXPECT_THROWS(entity.enable());
		EXPECT_THROWS(entity.disable());
		EXPECT_THROWS(entity.remove());
	},

	CASE("Entities comparison")
	{
		ecs::World world1;
		ecs::World world2;

		auto entity1{ world1.createEntity() };
		auto entity2{ world2.createEntity() };
		auto entity3{ world2.createEntity() };

		auto entity1Ref{ entity1 };
		auto entity2Ref{ entity2 };
		auto entity3Ref{ entity3 };

		EXPECT(entity1 == entity1Ref);
		EXPECT(entity2 == entity2Ref);
		EXPECT(entity3 == entity3Ref);

		EXPECT_NOT(entity1 == entity2);
		EXPECT_NOT(entity1 == entity3);
		EXPECT_NOT(entity2 == entity3);
		EXPECT_NOT(entity1 == entity2Ref);
		EXPECT_NOT(entity2 == entity3Ref);
		EXPECT_NOT(entity1Ref == entity2);
		EXPECT_NOT(entity1Ref == entity2Ref);
		EXPECT_NOT(entity2Ref == entity3Ref);
		EXPECT_NOT(entity1Ref == entity3Ref);
	},

	CASE("Named entities")
	{
		ecs::World world;

		auto anonEntity{ world.createEntity() };
		auto namedEntity{ world.createEntity("MyEntity") };

		EXPECT(anonEntity.getName() == std::string{});
		EXPECT(namedEntity.getName() == "MyEntity");

		// Already exists
		EXPECT_THROWS(world.createEntity("MyEntity"));

		EXPECT_NO_THROW(world.createEntity("MyOtherEntity"));
	},

	CASE("Add components")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		entity.addComponent<C>();
		entity.addComponent<A>();

		EXPECT(entity.hasComponent<A>());
		EXPECT(entity.hasComponent<C>());
		
		EXPECT_NOT(entity.hasComponent<B>());
	},

	CASE("Remove components")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		entity.addComponent<A>();
		entity.addComponent<B>();
		entity.addComponent<C>();

		EXPECT(entity.hasComponent<A>());
		EXPECT(entity.hasComponent<B>());
		EXPECT(entity.hasComponent<C>());

		entity.removeComponent<B>();

		EXPECT(entity.hasComponent<A>());
		EXPECT_NOT(entity.hasComponent<B>());
		EXPECT(entity.hasComponent<C>());

		entity.removeAllComponents();

		EXPECT_NOT(entity.hasComponent<A>());
		EXPECT_NOT(entity.hasComponent<B>());
		EXPECT_NOT(entity.hasComponent<C>());
	},

	CASE("Get components")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		entity.addComponent<A>();
		entity.addComponent<C>();

		EXPECT_NO_THROW(entity.getComponent<A>());
		EXPECT_NO_THROW(entity.getComponent<C>());
		
		EXPECT_THROWS(entity.getComponent<B>());

		entity.removeAllComponents();

		EXPECT_THROWS(entity.getComponent<A>());
		EXPECT_THROWS(entity.getComponent<B>());
		EXPECT_THROWS(entity.getComponent<C>());
	},

	CASE("GetComponent() returns a valid reference")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		entity.addComponent<A>();
		entity.addComponent<C>();

		EXPECT(addressOf(entity.getComponent<A>()) == addressOf(entity.getComponent<A>()));
		EXPECT(addressOf(entity.getComponent<C>()) == addressOf(entity.getComponent<C>()));
		
		EXPECT_NOT(addressOf(entity.getComponent<A>()) == addressOf(entity.getComponent<C>()));
	},

	CASE("AddComponent() and GetComponent() return a valid reference")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		auto const address{ addressOf(entity.addComponent<A>()) };

		EXPECT(address == addressOf(entity.getComponent<A>()));
	},

	CASE("Replace existing components")
	{
		ecs::World world;
		auto entity{ world.createEntity() };

		entity.addComponent<A>();

		auto const oldAddress{ addressOf(entity.getComponent<A>()) };
		auto const newAddress{ addressOf(entity.addComponent<A>()) };

		EXPECT_NOT(oldAddress == newAddress);
		EXPECT_NOT(oldAddress == addressOf(entity.getComponent<A>()));
	},

	CASE("Entity ID")
	{
		ecs::World world;

		auto entity1{ world.createEntity() };
		auto entity2{ world.createEntity() };

		EXPECT(entity1.getId() == static_cast<ecs::Entity::Id>(entity1));
		EXPECT(entity2.getId() == static_cast<ecs::Entity::Id>(entity2));

		EXPECT_NOT(entity1.getId() == entity2.getId());
	}
};

int main(int argc, char** argv)
{
	return lest::run(specification, argc, argv);
}
