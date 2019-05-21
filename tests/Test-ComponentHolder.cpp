// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <memory>

#include <ECS.hpp>
#include <lest/lest.hpp>

#ifdef _MSC_VER
	#pragma warning(disable: 4702)
#endif

struct A : public ecs::Component {};
struct B : public ecs::Component {};
struct C : public ecs::Component {};
struct D : public ecs::Component {};

template <class T>
void const* addressOf(T const& instance)
{
	return static_cast<void const*>(std::addressof(instance));
}

lest::test const specification[] =
{
	CASE("Add components")
	{
		ecs::detail::ComponentHolder holder;

		EXPECT_THROWS(holder.addComponent(0, std::make_unique<A>()));

		holder.resize(2);

		EXPECT_NO_THROW(holder.addComponent(0, std::make_unique<A>()));
		EXPECT_NO_THROW(holder.addComponent(1, std::make_unique<A>()));

		EXPECT_THROWS(holder.addComponent(2, std::make_unique<A>()));
	},

	CASE("Get components")
	{
		ecs::detail::ComponentHolder holder;
		holder.resize(1);

		holder.addComponent(0, std::make_unique<A>());
		holder.addComponent(0, std::make_unique<C>());

		EXPECT(holder.hasComponent<A>(0));
		EXPECT(holder.hasComponent<C>(0));
		
		EXPECT_NOT(holder.hasComponent<B>(0));
		EXPECT_NOT(holder.hasComponent<D>(0));

		EXPECT_NOT(holder.hasComponent<A>(1));
		EXPECT_NOT(holder.hasComponent<B>(1));
		EXPECT_NOT(holder.hasComponent<C>(1));
		EXPECT_NOT(holder.hasComponent<D>(1));
	},

	CASE("Remove components")
	{
		ecs::detail::ComponentHolder holder;
		holder.resize(1);

		holder.addComponent(0, std::make_unique<A>());
		holder.addComponent(0, std::make_unique<C>());

		holder.removeComponent<A>(0);

		EXPECT(holder.hasComponent<C>(0));
	
		EXPECT_NOT(holder.hasComponent<A>(0));
	},

	CASE("Remove all components")
	{
		ecs::detail::ComponentHolder holder;
		holder.resize(3);

		holder.addComponent(0, std::make_unique<A>());
		holder.addComponent(1, std::make_unique<D>());
		holder.addComponent(2, std::make_unique<B>());

		holder.removeAllComponents(1);

		EXPECT_NOT(holder.hasComponent<D>(1));

		holder.clear();

		EXPECT_NOT(holder.hasComponent<A>(0));
		EXPECT_NOT(holder.hasComponent<B>(2));
	},

	CASE("Get components")
	{
		ecs::detail::ComponentHolder holder;
		holder.resize(2);

		holder.addComponent(0, std::make_unique<B>());
		holder.addComponent(1, std::make_unique<B>());

		EXPECT_NO_THROW(holder.getComponent<B>(0));
		EXPECT_NO_THROW(holder.getComponent<B>(1));
		
		EXPECT_THROWS(holder.getComponent<A>(0));
		EXPECT_THROWS(holder.getComponent<C>(0));
		EXPECT_THROWS(holder.getComponent<D>(0));
		EXPECT_THROWS(holder.getComponent<A>(1));
		EXPECT_THROWS(holder.getComponent<B>(2));
		EXPECT_THROWS(holder.getComponent<C>(3));
	},

	CASE("GetComponent() returns a valid reference")
	{
		ecs::detail::ComponentHolder holder;
		holder.resize(2);

		holder.addComponent(0, std::make_unique<A>());
		holder.addComponent(0, std::make_unique<D>());
		holder.addComponent(1, std::make_unique<A>());
		holder.addComponent(1, std::make_unique<D>());

		EXPECT(addressOf(holder.getComponent<A>(0)) == addressOf(holder.getComponent<A>(0)));
		EXPECT(addressOf(holder.getComponent<D>(0)) == addressOf(holder.getComponent<D>(0)));
		EXPECT(addressOf(holder.getComponent<A>(1)) == addressOf(holder.getComponent<A>(1)));
		EXPECT(addressOf(holder.getComponent<D>(1)) == addressOf(holder.getComponent<D>(1)));
		
		EXPECT_NOT(addressOf(holder.getComponent<A>(0)) == addressOf(holder.getComponent<A>(1)));
		EXPECT_NOT(addressOf(holder.getComponent<D>(0)) == addressOf(holder.getComponent<D>(1)));
		EXPECT_NOT(addressOf(holder.getComponent<A>(0)) == addressOf(holder.getComponent<D>(0)));
		EXPECT_NOT(addressOf(holder.getComponent<D>(0)) == addressOf(holder.getComponent<A>(0)));
	}
};

int main(int argc, char** argv)
{
	// Init
	ecs::getComponentTypeId<A>();
	ecs::getComponentTypeId<B>();
	ecs::getComponentTypeId<C>();
	ecs::getComponentTypeId<D>();

	return lest::run(specification, argc, argv);
}
