// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS.hpp>
#include <lest/lest.hpp>

using namespace ecs;
using namespace detail;

struct A : Component {};
struct B : Component {};
struct C : Component {};
struct D : Component {};
struct E : Component {};
struct F : Component {};

lest::test const specification[] =
{
	CASE("Require list only")
	{
		ComponentFilter filter;

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
		ComponentFilter filter;

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
	}
};

int main(int argc, char **argv)
{
	// Init
	getComponentTypeId<A>();
	getComponentTypeId<B>();
	getComponentTypeId<C>();
	getComponentTypeId<D>();
	getComponentTypeId<E>();
	getComponentTypeId<F>();

	return lest::run(specification, argc, argv);
}
