ecs
===

A minimal entity-component system

brief synopsis
--------------
Everything begins with Entities.

```c++
#include "ecs.hpp"
Entities entities;
```

Create entities:

```c++
auto e = entities.create();
```

An entity is basically just an id. You can add components to entities.

```c++
e.add<PositionComponent>(100, 100);
```

Before you add components, you must define them.

```c++
struct PositionComponent
{
    PositionComponent(int x = 0, int y = 0) : x(x), y(y) {}
    int x, y;
};
```

The entities are updated through systems, which are added like this:

```c++
entities.add_system<MoveSystem>();
```

Before you add systems, you must define them.

```c++
class MoveSystem : System
{
    MoveSystem()
    {
        require_component<PositionComponent>();
        require_component<VelocityComponent>()
    }

    virtual void update(float dt)
    {
        for (auto e : get_entities()) {
            if (e.is_alive()) {
                auto &pos = e.get<PositionComponent>();
                const auto vel = e.get<VelocityComponent>();
                pos.x += vel.x * delta;
                pos.y += vel.y * delta;
            }
            else {
                remove_entity(e);
            }
        }
    }
};
```

The system tells the world what components an entity must have (as a minimum) in order for the system to be interested in processing the entity.
Each frame, the system processes all the entities (of interest). If the entity is still alive, the system does its thing. If not, then the entity
is removed from the system's list of interest.

Now, we have all we need. Entities, components and systems. The only thing missing is the game loop, or rather, what needs to happen each frame in the game loop.

```c++
entities.update();
auto &move_system = entities.get_system<MoveSystem>();
move_system.update();
```

Each frame, the world needs to be updated. This means that the entities that were created and destroyed in the previous frame are *actually* created and destroyed 
and thus ready (and not ready) for processing by the systems. Then, as we don't know what exact order you want your systems to process entities, you need to get each system
from the system manager and call their update method manually.

That's it for now. Questions?
