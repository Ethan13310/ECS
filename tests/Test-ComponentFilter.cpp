// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS.hpp>
#include <lest/lest.hpp>

struct A : ecs::Component {};
struct B : ecs::Component {};
struct C : ecs::Component {};
struct D : ecs::Component {};
struct E : ecs::Component {};
struct F : ecs::Component {};

lest::test const specification[] =
{
	CASE("Require list only")
	{
		ecs::detail::ComponentFilter filter;

		filter.require<A>();
		filter.require<F>();
		filter.require<B>();

		EXPECT(filter.check(0b100011));
		EXPECT(filter.check(0b101011));
		EXPECT(filter.check(0b110111));
		EXPECT(filter.check(0b111111));

		EXPECT_NOT(filter.check(0b111001));
		EXPECT_NOT(filter.check(0b100010));
		EXPECT_NOT(filter.check(0b101110));
		EXPECT_NOT(filter.check(0b000011));
		EXPECT_NOT(filter.check(0b001011));
		EXPECT_NOT(filter.check(0b000010));
		EXPECT_NOT(filter.check(0b110010));
		EXPECT_NOT(filter.check(0b000000));
	},

	CASE("Require and exclude list")
	{
		ecs::detail::ComponentFilter filter;

		filter.require<C>();
		filter.require<A>();
		filter.require<F>();

		filter.exclude<E>();
		filter.exclude<D>();

		EXPECT(filter.check(0b100101));
		EXPECT(filter.check(0b100111));

		EXPECT_NOT(filter.check(0b110101));
		EXPECT_NOT(filter.check(0b101101));
		EXPECT_NOT(filter.check(0b111101));
		EXPECT_NOT(filter.check(0b111111));
		EXPECT_NOT(filter.check(0b101110));
		EXPECT_NOT(filter.check(0b000000));
	},

	CASE("Exclude all")
	{
		ecs::detail::ComponentFilter filter;

		filter.excludeAll();

		EXPECT(filter.check(0b000000));

		EXPECT_NOT(filter.check(0b100000));
		EXPECT_NOT(filter.check(0b100100));
		EXPECT_NOT(filter.check(0b000001));
		EXPECT_NOT(filter.check(0b010100));
		EXPECT_NOT(filter.check(0b001101));
		EXPECT_NOT(filter.check(0b111111));
	},

	CASE("Exclude not required")
	{
		ecs::detail::ComponentFilter filter;

		filter.require<D>();
		filter.require<E>();
		filter.require<F>();

		filter.excludeNotRequired();

		EXPECT(filter.check(0b111000));

		EXPECT_NOT(filter.check(0b101000));
		EXPECT_NOT(filter.check(0b110000));
		EXPECT_NOT(filter.check(0b000000));
		EXPECT_NOT(filter.check(0b111010));
		EXPECT_NOT(filter.check(0b111001));
		EXPECT_NOT(filter.check(0b111101));
		EXPECT_NOT(filter.check(0b111111));
		EXPECT_NOT(filter.check(0b101111));
		EXPECT_NOT(filter.check(0b101011));
		EXPECT_NOT(filter.check(0b000000));
	},

	CASE("Require, exclude, then ignore")
	{
		ecs::detail::ComponentFilter filter;

		filter.require<C>();
		filter.require<F>();
		filter.require<D>();

		filter.exclude<A>();
		filter.exclude<C>();
		filter.exclude<E>();

		filter.ignore<A>();
		filter.ignore<F>();

		EXPECT(filter.check(0b001000));
		EXPECT(filter.check(0b101000));
		EXPECT(filter.check(0b101001));
		EXPECT(filter.check(0b101011));
		EXPECT(filter.check(0b001011));

		EXPECT_NOT(filter.check(0b000000));
		EXPECT_NOT(filter.check(0b001100));
		EXPECT_NOT(filter.check(0b011000));
		EXPECT_NOT(filter.check(0b011100));
		EXPECT_NOT(filter.check(0b111111));
	}
};

int main(int argc, char **argv)
{
	// Init
	ecs::getComponentTypeId<A>();
	ecs::getComponentTypeId<B>();
	ecs::getComponentTypeId<C>();
	ecs::getComponentTypeId<D>();
	ecs::getComponentTypeId<E>();
	ecs::getComponentTypeId<F>();

	return lest::run(specification, argc, argv);
}
