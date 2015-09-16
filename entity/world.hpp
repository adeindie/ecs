#pragma once

#include "entity.hpp"
#include "system.hpp"
#include <vector>
#include <string>
#include <memory>

namespace entity
{
    /*
    The World manages the creation and destruction of entities so that entities don't show up/disappear mid-frame.
    It also has all the managers (entity, system, event), which can be accessed by the systems using the getters.
    */
    class World
    {
    public:
        /*
        Creates all the managers.
        */
        World();

        /*
        Getters to the managers.
        */
        EntityManager& get_entity_manager() const;

        /*
        Updates the systems so that created/deleted entities are removed from the systems' vectors of entities.
        Updates the entity manager so that the version of a destructed entity's index is incremented.
        Destroys all the events that were created during the last frame.
        */
        void update();

        /*
        Creates an entity.
        */
        Entity create_entity();

        /*
        Kills an entity.
        */
        void kill_entity(Entity e);

        /*
        Returns an entity by its tag.
        */
        Entity get_entity(std::string tag_name) const;

        /*
        Returns a vector of entities belonging to a certain group.
        */
        std::vector<Entity> get_entity_group(std::string group_name) const;

        /* System */
        template <typename T> void add_system();
        template <typename T> void remove_system();
        template <typename T> T& get_system();
        template <typename T> bool has_system() const;

        // adds an entity to each system that is interested of the entity
        void update_systems(Entity e);

    private:
        // vector of entities that are awaiting creation
        std::vector<Entity> created_entities;

        // vector of entities that are awaiting destruction
        std::vector<Entity> killed_entities;

        // the managers
        std::unique_ptr<EntityManager> entity_manager = nullptr;

        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
    };

    template <typename T>
    void World::add_system()
    {
        if (has_system<T>()) {
            return;
        }

        std::shared_ptr<T> system(new T);
        system->world = this;
        systems.insert(std::make_pair(std::type_index(typeid(T)), system));
    }

    template <typename T>
    void World::remove_system()
    {
        if (!has_system<T>()) {
            return;
        }

        auto it = systems.find(std::type_index(typeid(T)));
        systems.erase(it);
    }

    template <typename T>
    T& World::get_system()
    {
        if (!has_system<T>()) {
            throw std::runtime_error(std::string("Failed to get system: ") + typeid(T).name());
        }

        auto it = systems.find(std::type_index(typeid(T)));
        return *(std::static_pointer_cast<T>(it->second));
    }

    template <typename T>
    bool World::has_system() const
    {
        return systems.find(std::type_index(typeid(T))) != systems.end();
    }

}
