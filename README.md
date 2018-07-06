# ECS Library
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](./LICENSE)

An open-source **[Entity Component System](https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system)** library written in C++17.

# Getting started

## Requirements

- A compiler that supports C++17 :
  - [GNU C++ Compiler](https://gcc.gnu.org/) >= 7.1
  - [Microsoft Visual C++](https://visualstudio.microsoft.com/) >= 15.0 (2017)
  - [Clang++](https://clang.llvm.org/) >= 5.0
- [CMake](https://cmake.org/) >= 3.9

## Installation

```bash
cmake .
make install
```

## How to Use

This section describes how to use this library.

### The World

The World is what contains all the Entities, Systems and Components. You can use multiple Worlds at the same time.

```cpp
ecs::World world;
```

You can remove everything that contains the World by calling `clear()` :

```cpp
world.clear();
// Every Entities and Systems have been removed from the World
```

If you want to keep the Systems in place, use `removeAllEntities()` instead :

```cpp
world.removeAllEntities();
// Every Entities and their Components have been removed, but the Systems are still running
```

### The Entities

An Entity is what describes an object (the player, a tree, etc.) or anything else which may not have a visual representation (like a music) in your game.

To create an Entity, you have to call the member function `createEntity()` of your World object :

```cpp
auto entity{ world.createEntity() };

// You can also create a named Entity
// A name must be unique
auto other{ world.createEntity("MyEntity") };
```

An Entity is basically an ID and a reference to the World that it belongs to. Thus, `Entity` objects can be copied so they will refer to the same Entity :

```cpp
auto entity1{ world.createEntity() };

// entity1 and entity2 refer to the same Entity
auto entity2{ entity1 };
```

Entities can be disabled, enabled and removed. By default, they're enabled.

You can use two methods :

- Method 1 :

```cpp
entity.disable();
entity.enable();
entity.remove();
```

- Method 2 :

```cpp
world.disableEntity(entity);
world.enableEntity(entity);
world.removeEntity(entity);
```

After an Entity has been removed, it becomes invalid and you should not use it anymore. Otherwise, an exception will be raised.

You can also get some informations about the Entity :

```cpp
// Get the Entity ID
entity.getId();

// Get the Entity name
entity.getName(); // or world.getEntityName(entity);

// Check whether the Entity is enabled or not
entity.isEnabled(); // or world.isEntityEnabled(entity);

// Check whether the Entity is valid or not
// An entity is considered invalid if it does not belong to any World or if its ID does
// not correspond to any Entity within the World
entity.isValid(); // or world.isEntityValid(entity);

// Compare two Entities
if (entity1 == entity2) {
    // entity1 and entity2 refer to the same Entity
}
```

The World allows you to retreive Entities by their ID or their name :

```cpp
auto entity1{ world.getEntity(2) };          // By ID
auto entity2{ world.getEntity("MyEntity") }; // By name

// Both functions return a std::optional
if (entity1.has_value() && entity2.has_value()) {
    // entity1 and entity2 exist ...
}
```

### The Components

In order to describe what an Entity is, you need to add some Components to it.

First of all, you have to create your own Components. Your Components must inherit from `ecs::Component` :

```cpp
struct Health : public ecs::Component
{
    float healthPoints;
    float maxHealthPoints;
    float shield;
    // ...
};
```

To add, get or remove a Component to an Entity, use :

```cpp
// Add a Component
entity.addComponent<Health>(/* optional parameters */);

// Get a Component
entity.getComponent<Health>();

// Remove a Component
entity.removeComponent<Health>();

// Remove every Components
entity.removeAllComponents();
```

You can pass some parameters to `addComponent<>()` which will be used to construct the Component.

Both `addComponent()` and `getComponent()` return a reference to the Component.

`getComponent<>()` will raise an exception if the Entity does not have this Component. You can use `hasComponent<>()` to determine if an Entity has a Component or not :

```cpp
if (entity.hasComponent<Health>()) {
    // The Component exists ...
}
```

### The Systems

A System is used to manage a group of Entities which meet some requirements.

To create a System, you must create a class which inherits from `ecs::System` :

```cpp
class HealthSystem : public ecs::System
{
    // ...
};
```

You can manage your Systems via your World object, exactly the same way you do for your Entities and their Components :

```cpp
// Instantiate a System
world.addSystem<HealthSystem>(/* optional parameters */);

// Get a System
world.getSystem<HealthSystem>(); // Will throw if the System does not exist

// Remove a System
world.removeSystem<HealthSystem>();

// Remove every Systems
world.removeAllSystems();

// Check whether a System exists or not
if (world.hasSystem<HealthSystem>()) {
    // The System exists ...
}
```

#### System Filter

By default, a System accept any Entity. This behaviour can be modified by editing the Filter within the System's constructor :

```cpp
class HealthSystem : public ecs::System
{
public:
    HealthSystem()
    {
        getFilter().require<Health>();
        getFilter().require<Character>();
        getFilter().exclude<Immortal>();
        // ...
    }
};
```

Here's the list of what you can do with Filters :

```cpp
// Only Entities which have the Component 'MyComponent' will be attached to the System.
getFilter().require<MyComponent>();

// Entities which have the Component 'MyComponent' won't be attached to the System.
getFilter().exclude<MyComponent>();

// Entities which have other Components than the required ones won't be attached to
// the System. So you must use 'require()' before calling this function.
getFilter().excludeNotRequired();

// No Entity will be attached to the System.
getFilter().excludeAll();

// 'MyComponent' is no longer required or excluded.
getFilter().ignore<MyComponent>();
```

#### System Events

Systems are also subject to various events. You can overload the functions of the events you want your System to handle.

Here's the list of the events that a System can handle :

```cpp
// Fired after the System has been instantiated
virtual void onStart();

// Fired before the System is destroyed. This is the last event to be fired
virtual void onShutdown();

// Fired each frame, before each Entities have been updated (and then before onUpdate())
virtual void onPreUpdate(float elapsed);

// Fired each frame
virtual void onUpdate(float elapsed);

// Fired when an Entity has been attached to the System
// You can use this event to allocate resources for this Entity
virtual void onEntityAttached(Entity entity);

// Fired when an Entity has been detached from the System
// You can use this event to free the resources of this Entity
virtual void onEntityDetached(Entity entity);

// Fired when an Entity has been enabled
virtual void onEntityEnabled(Entity entity);

// Fired when an Entity has been disabled
virtual void onEntityDisabled(Entity entity);
```

**All Entities will be detached from the System through `onEntityDetached()` whatever happens.** So you don't have to worry about releasing resources within the destructor or anything else.

#### Manage Entities

You may want to be able to manipulate each Entities held by System (within `onUpdate()`, for example). You have two ways to proceed.

- The first, by using a lambda :

```cpp
void HealthSystem::onUpdate(float elapsed)
{
    forEach([&](Entity entity) {
        // Manipulate your Entity here
    });
}
```

- And the second, by using a range-based for loop :

```cpp
void HealthSystem::onUpdate(float elapsed)
{
    for (auto entity : getEntities()) {
        // Manipulate your Entity here
    }
}
```

These two methods iterate only through enabled Entities. There's no way to iterate through the disabled ones.

You can query the number of enabled Entities attached to the System by calling `getEntityCount()`.

#### My World

If you need to access the World that your System belongs to, you can use `getWorld()` :

```cpp
// Access a named Entity
auto entity{ getWorld().getEntity("MainCharacter").value() };
```

**Do not use this to make your System remove itself ! You're warned.**

### The Event Dispatcher

Systems can also communicate between them thanks to the Event Dispatcher. The Event Dispatcher allows Systems to emit and receive Events.

An Event is a `struct` which inherits from `ecs::Event` :

```cpp
struct ButtonClickedEvent : public ecs::Event
{
    std::string buttonName;
    // ...
};
```

#### Emit an Event

Use `emit()` :

```cpp
ButtonClickedEvent evt;
evt.buttonName = "MainMenu";
emit(evt);
```

Each Systems which are waiting for a `ButtonClickedEvent` will be notified.

#### Receive an Event

To receive an Event, you need to register it first :

```cpp
void MySystem::buttonClickedHandler(ButtonClickedEvent const &evt)
{
    // ...
}

void MySystem::registerEvents()
{
    auto eventId{ connectEvent<ButtonClickedEvent>(&MySystem::buttonClickedHandler) };
    // ...
}
```

You can register as many handlers as you want for the same Event.

If, for any reason, you want to disconnect an handler from an Event, you can use `disconnectEvent()`. However, you will need the Event ID which is return by `connectEvent()` :

```cpp
auto eventId{ connectEvent<ButtonClickedEvent>(&MySystem::buttonClickedHandler) };
// ...
disconnectEvent(eventId);
```

**A System can only disconnect his own handlers !**

You can also disconnect all handlers your System has registered at once by calling `disconnectAllEvents()`.

### That's all !

Have fun !
