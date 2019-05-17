// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS.hpp>
#include <lest/lest.hpp>

class Base {};
class Type {};

class A : public Base {};
class B : public Base {};
class C : public B {};

lest::test const specification[] =
{
	CASE("Increment Type IDs")
	{
		// New types
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<Base>() == 0);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<Type>() == 0);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<A>() == 1);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<B>() == 2);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<C>() == 3);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<A>() == 1);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<B>() == 2);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<C>() == 3);

		// Known types
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<Base>() == 0);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<Type>() == 0);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<Type>() == 0);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<Base>() == 0);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<C>() == 3);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<A>() == 1);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<B>() == 2);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<A>() == 1);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<B>() == 2);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<A>() == 1);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<C>() == 3);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<B>() == 2);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<C>() == 3);
		EXPECT(ecs::detail::TypeInfo<Base>::getTypeId<C>() == 3);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<B>() == 2);
		EXPECT(ecs::detail::TypeInfo<Type>::getTypeId<A>() == 1);
	}
};

int main(int argc, char **argv)
{
	return lest::run(specification, argc, argv);
}
