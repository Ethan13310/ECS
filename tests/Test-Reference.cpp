// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <utility>

#include <ECS.hpp>
#include <lest/lest.hpp>

int square(int x)
{
	return x * x;
}

class Foo
{
public:
	int multiplty(int x, int y)
	{
		return x * y;
	}
};

lest::test const specification[] =
{
	CASE("Simple reference")
	{
		int x{ 5 };
		ecs::detail::Reference<int> y{ x };

		y.get() = 10;

		EXPECT(x == 10);
		EXPECT(y == x);
		EXPECT(y.get() == x);
	},

	CASE("Function reference")
	{
		ecs::detail::Reference<int(int)> funcRef{ std::ref(square) };

		EXPECT(funcRef(8) == square(8));
	},

	CASE("Moving reference")
	{
		int x{ 2 };
		ecs::detail::Reference<int> y{ x };
		ecs::detail::Reference<int> z{ std::move(y) };

		z.get() = 42;

		EXPECT(x == 42);
		EXPECT(z == x);
		EXPECT(z.get() == x);
	},

	CASE("Operator ->")
	{
		Foo foo;
		ecs::detail::Reference<Foo> refFoo{ foo };

		EXPECT(refFoo->multiplty(5, 5) == foo.multiplty(5, 5));
		EXPECT(refFoo->multiplty(5, 5) == refFoo.get().multiplty(5, 5));
	}
};

int main(int argc, char **argv)
{
	return lest::run(specification, argc, argv);
}
