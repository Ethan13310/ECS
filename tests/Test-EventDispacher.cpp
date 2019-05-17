// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#include <ECS.hpp>
#include <lest/lest.hpp>

struct MyEvent : public ecs::Event
{
	int value{ 10 };
};

struct OtherEvent : public ecs::Event
{
	char letter{ 'A' };
};

struct UnusedEvent : public ecs::Event
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
		// The last Event that has been emitted
		auto lastCall{ LastCall::Unknown };

		// Number of times an event handler has been called
		std::size_t callCount{ 0 };

		// Reset current state between two events
		auto reset = [&]() {
			lastCall = LastCall::Unknown;
			callCount = 0;
		};

		// 'MyEvent' handler
		auto receiveMyEvent = [&](MyEvent const &evt) {
			EXPECT(evt.value == 10);
			lastCall = LastCall::MyEvent;
			++callCount;
		};

		// 'OtherEvent' handler
		auto receiveOtherEvent = [&](OtherEvent const &evt) {
			EXPECT(evt.letter == 'A');
			lastCall = LastCall::OtherEvent;
			++callCount;
		};

		ecs::EventDispatcher evt;

		// Single handler
		auto const myEventId{ evt.connect<MyEvent>(receiveMyEvent) };

		// Multiple handlers
		evt.connect<OtherEvent>(receiveOtherEvent);
		evt.connect<OtherEvent>(receiveOtherEvent);

		evt.emit<OtherEvent>();
		EXPECT(lastCall == LastCall::OtherEvent);
		EXPECT(callCount == 2);

		reset();
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);
		EXPECT(callCount == 1);

		reset();
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);
		EXPECT(callCount == 1);

		reset();
		evt.emit<OtherEvent>();
		EXPECT(lastCall == LastCall::OtherEvent);
		EXPECT(callCount == 2);

		reset();
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);
		EXPECT(callCount == 1);

		reset();
		evt.emit<UnusedEvent>();
		EXPECT(lastCall == LastCall::Unknown);
		EXPECT(callCount == 0);

		reset();
		// No more hanlder for 'OtherEvent'
		evt.clear<OtherEvent>();
		evt.emit<OtherEvent>();
		EXPECT(lastCall == LastCall::Unknown);
		EXPECT(callCount == 0);

		reset();
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::MyEvent);
		EXPECT(callCount == 1);

		reset();
		// No more handler for 'MyEvent'
		evt.clear(myEventId);
		evt.emit<MyEvent>();
		EXPECT(lastCall == LastCall::Unknown);
		EXPECT(callCount == 0);
	}
};

int main(int argc, char **argv)
{
	return lest::run(specification, argc, argv);
}
