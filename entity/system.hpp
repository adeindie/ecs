#pragma once

#include "entity.hpp"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace entity
{
    class World;

    // The system processes entities that it's interested in each frame. Derive from this one!
    class System
    {
    public:
        virtual ~System() {}

        // override this!
        virtual void update(float dt = 0.0f) = 0;

        // what component types the system requires of entities (we can use this method in the constructor for example)
        template <typename T>
        void require_component();

        // returns a list of entities that the system should process each frame
        std::vector<Entity> get_entities() { return entities; }

        // adds an entity of interest
        void add_entity(Entity e);

        // if the entity is not alive anymore (during processing), the entity should be removed
        void remove_entity(Entity e);

        const ComponentMask& get_component_mask() const { return component_mask; }

    private:
        World& get_world() const;

        // which components an entity must have in order for the system to process the entity
        ComponentMask component_mask;

        // vector of all entities that the system is interested in
        std::vector<Entity> entities;

        World *world = nullptr;
        friend class World;
    };

    template <typename T>
    void System::require_component()
    {
        const auto component_id = Component<T>::get_id();
        component_mask.set(component_id);
    }

}
