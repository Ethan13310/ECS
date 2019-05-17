// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS.hpp>
#include <lest/lest.hpp>

lest::test const specification[] =
{
	CASE("Increment Entity IDs")
	{
		ecs::detail::EntityPool pool;

		EXPECT(pool.create() == 0); // First ID starts at 0
		EXPECT(pool.create() == 1);
		EXPECT(pool.create() == 2);
		EXPECT(pool.create() == 3);
		EXPECT(pool.create() == 4);
	},

	CASE("Get stored IDs first")
	{
		ecs::detail::EntityPool pool;

		pool.create(); // 0
		pool.create(); // 1
		pool.create(); // 2
		pool.create(); // 3

		pool.store(2);
		pool.store(0);
		pool.store(1);

		EXPECT(pool.create() == 1);
		EXPECT(pool.create() == 0);
		EXPECT(pool.create() == 2);
		EXPECT(pool.create() == 4); // Empty Pool
	},

	CASE("Pool reset")
	{
		ecs::detail::EntityPool pool;

		pool.create();
		pool.create();
		pool.create();

		pool.store(0);
		pool.store(1);
		pool.store(2);

		pool.reset();

		EXPECT(pool.create() == 0);

		pool.create();
		pool.create();
		pool.create();
		pool.create();

		pool.reset();

		EXPECT(pool.create() == 0);
	}
};

int main(int argc, char **argv)
{
	return lest::run(specification, argc, argv);
}
