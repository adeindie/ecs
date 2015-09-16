#include "system.hpp"
#include "world.hpp"
#include <algorithm>

namespace entity
{

    void System::add_entity(Entity e)
    {
        entities.push_back(e);
    }

    void System::remove_entity(Entity e)
    {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&e](Entity other) { return e == other; }
        ), entities.end());
    }

    World& System::get_world() const
    {
        assert(world != nullptr);
        return *world;
    }

}
