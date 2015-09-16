#include "world.hpp"
#include <cassert>

namespace entity
{

    World::World()
    {
        entity_manager = std::make_unique<EntityManager>(*this);
    }

    EntityManager& World::get_entity_manager() const
    {
        assert(entity_manager != nullptr);
        return *entity_manager;
    }

    void World::update()
    {
        for (auto e : created_entities) {
            update_systems(e);
        }
        created_entities.clear();

        for (auto e : killed_entities) {
            get_entity_manager().destroy_entity(e);
        }
        killed_entities.clear();
    }

    void World::update_systems(Entity e)
    {
        const auto &entity_component_mask = get_entity_manager().get_component_mask(e);

        for (auto &it : systems) {
            auto system = it.second;
            const auto &system_component_mask = system->get_component_mask();
            auto interest = (entity_component_mask & system_component_mask) == system_component_mask;

            if (interest) {
                system->add_entity(e);
            }
        }
    }

    Entity World::create_entity()
    {
        auto e = get_entity_manager().create_entity();
        created_entities.push_back(e);
        return e;
    }

    void World::kill_entity(Entity e)
    {
        killed_entities.push_back(e);
    }

    Entity World::get_entity(std::string tag_name) const
    {
        return get_entity_manager().get_entity_by_tag(tag_name);
    }

    std::vector<Entity> World::get_entity_group(std::string group_name) const
    {
        return get_entity_manager().get_entity_group(group_name);
    }

}
