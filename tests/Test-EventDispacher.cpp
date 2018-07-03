// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS.hpp>
#include <lest/lest.hpp>

using namespace ecs;
using namespace detail;

struct MyEvent : public Event
{
	int value{ 10 };
};

struct OtherEvent : public Event
{
	char letter{ 'A' };
};

struct UnusedEvent : public Event
{
	bool status{ true };
};

enum class LastCall
{
	Unknown,
	MyEvent,
	OtherEvent
};

lest::test const specification[] =
{
	CASE("Event Dispacher")
	{
		LastCall lastCall{ LastCall::Unknown };

		auto receiveMyEvent = [&](MyEvent const &evt) {
			EXPECT(evt.value == 10);
			lastCall = LastCall::MyEvent;
		};

		auto receiveOtherEvent = [&](OtherEvent const &evt) {
			EXPECT(evt.letter == 'A');
			lastCall = LastCall::OtherEvent;
		};

		EventDispatcher evt;

		evt.connect<MyEvent>(receiveMyEvent);
		evt.connect<OtherEvent>(receiveOtherEvent);
		evt.connect<OtherEvent>(receiveOtherEvent);

		evt.emit<OtherEvent>();
		EXPECT(lastCall == LastCall::OtherEvent);

		lastCall = LastCall::Unknown;
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);

		lastCall = LastCall::Unknown;
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);

		lastCall = LastCall::Unknown;
		evt.emit<OtherEvent>();
		EXPECT(lastCall == LastCall::OtherEvent);

		lastCall = LastCall::Unknown;
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);

		lastCall = LastCall::Unknown;
		evt.emit<UnusedEvent>();
		EXPECT(lastCall == LastCall::Unknown);

		lastCall = LastCall::Unknown;
		evt.clear<OtherEvent>();
		evt.emit<OtherEvent>();
		EXPECT(lastCall == LastCall::Unknown);

		lastCall = LastCall::Unknown;
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);
	}
};

int main(int argc, char **argv)
{
	return lest::run(specification, argc, argv);
}
