#pragma once

#include "scene/scene.hpp"
#include "scene/ecs/component.hpp"

#include <entt/entt.hpp>

namespace CGEngine {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity entity, Scene* scene) : m_enttEntity(entity), m_scene(scene) {}
		Entity(const Entity& other) = default;

        template <typename... T>
        bool has() {
            return m_scene->m_registry.all_of<T...>(m_enttEntity);
        }

        template <typename T>
        T& get() {
            assert(has<T>() && "Entity does not have component");

            return m_scene->m_registry.get<T>(m_enttEntity);
        }

        template <typename T, typename... Args>
        T& add(Args&&... args) {
            T& component = m_scene->m_registry.emplace<T>(m_enttEntity, std::forward<Args>(args)...);
            return component;
        }

        template <typename T>
        void remove() {
            assert(has<T>() && "Entity does not have component");
            assert((std::is_same<T, IDComponent>::value) && "Cannot remove ID component");

            m_scene->m_registry.remove<T>(m_enttEntity);
        }

        operator entt::entity() const { return m_enttEntity; }
        operator bool() const { return m_enttEntity != entt::null; }

        UUID getUUID() {
            return get<IDComponent>().uuid;
        }

    private:
        entt::entity m_enttEntity{entt::null};
        Scene* m_scene = nullptr;
    };

}