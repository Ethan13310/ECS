// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS.hpp>
#include <lest/lest.hpp>

using namespace ecs;
using namespace detail;

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
		EXPECT(TypeInfo<Base>::getTypeId<Base>() == 0);
		EXPECT(TypeInfo<Type>::getTypeId<Type>() == 0);
		EXPECT(TypeInfo<Base>::getTypeId<A>() == 1);
		EXPECT(TypeInfo<Base>::getTypeId<B>() == 2);
		EXPECT(TypeInfo<Base>::getTypeId<C>() == 3);
		EXPECT(TypeInfo<Type>::getTypeId<A>() == 1);
		EXPECT(TypeInfo<Type>::getTypeId<B>() == 2);
		EXPECT(TypeInfo<Type>::getTypeId<C>() == 3);

		// Known types
		EXPECT(TypeInfo<Base>::getTypeId<Base>() == 0);
		EXPECT(TypeInfo<Type>::getTypeId<Type>() == 0);
		EXPECT(TypeInfo<Type>::getTypeId<Type>() == 0);
		EXPECT(TypeInfo<Base>::getTypeId<Base>() == 0);
		EXPECT(TypeInfo<Base>::getTypeId<C>() == 3);
		EXPECT(TypeInfo<Base>::getTypeId<A>() == 1);
		EXPECT(TypeInfo<Type>::getTypeId<B>() == 2);
		EXPECT(TypeInfo<Base>::getTypeId<A>() == 1);
		EXPECT(TypeInfo<Base>::getTypeId<B>() == 2);
		EXPECT(TypeInfo<Type>::getTypeId<A>() == 1);
		EXPECT(TypeInfo<Type>::getTypeId<C>() == 3);
		EXPECT(TypeInfo<Base>::getTypeId<B>() == 2);
		EXPECT(TypeInfo<Type>::getTypeId<C>() == 3);
		EXPECT(TypeInfo<Base>::getTypeId<C>() == 3);
		EXPECT(TypeInfo<Type>::getTypeId<B>() == 2);
		EXPECT(TypeInfo<Type>::getTypeId<A>() == 1);
	}
};

int main(int argc, char **argv)
{
	return lest::run(specification, argc, argv);
}
