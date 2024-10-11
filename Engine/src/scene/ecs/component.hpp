
#pragma once

#include "core/uuid.hpp"
#include "core/memory.hpp"
#include "graphics/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CGEngine
{
    struct IDComponent {
        UUID uuid;

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;

        operator UUID&() { return uuid; }
        operator const UUID&() const { return uuid; }
    };

    struct NameComponent {
        std::string name;

        NameComponent() = default;
        NameComponent(const NameComponent&) = default;
        NameComponent(const std::string& name) : name(name) {}

        operator std::string&() { return name; }
        operator const std::string&() const { return name; }
    };

    struct ColorComponent {
        glm::vec4 color;

        ColorComponent() = default;
        ColorComponent(const ColorComponent&) = default;
        ColorComponent(const glm::vec4& color) : color(color) {}
        ColorComponent(const glm::vec3& color) : color(glm::vec4{color, 1.f}) {}

        operator glm::vec4&() { return color; }
        operator const glm::vec4&() const { return color; }
        operator glm::vec3() const { return glm::vec3(color); }
    };

    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            const float sin = glm::sin(rotation);
            const float cos = glm::cos(rotation);
            glm::mat2 rotationMatrix{{cos, sin}, {-sin, cos}};

            glm::mat2 scaleMatrix{{scale.x, 0.f}, {0.f, scale.y}};
            
            return rotationMatrix * scaleMatrix;
        }

        Transform2dComponent() = default;
        Transform2dComponent(const Transform2dComponent&) = default;
        Transform2dComponent(const glm::vec2& translation) 
            : translation(translation) {}

        Transform2dComponent(const glm::vec2& translation, const glm::vec2& scale) 
            : translation(translation), scale(scale) {}

        Transform2dComponent(const glm::vec2& translation, const glm::vec2& scale, float rotation) 
            : translation(translation), scale(scale), rotation(rotation) {}
    
        operator glm::mat2() { return mat2(); }
    };

    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        /**
         * @brief Transforms the entity's position, scale, and rotation into a 4x4 matrix
         * 
         * Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
         * Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
         * 
         * To view the rotation as extrinsic, just read the operations from right to left
         * Otherwise, to view the rotation as intrinsic, read the operations from left to right
         * 
         * - Extrinsic: Z(world) -> X(world) -> Y(world)
         * 
         * - Intrinsic: Y(local) -> X(local) -> Z(local)
         * 
         * @note https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
         * 
         * @return glm::mat4
         */
        glm::mat4 mat4() {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            return glm::mat4{
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f,
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f,
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0f,
                },
                {translation.x, translation.y, translation.z, 1.0f}};
        }

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) 
            : translation(translation) {}

        TransformComponent(const glm::vec3& translation, const glm::vec3& scale) 
            : translation(translation), scale(scale) {}

        TransformComponent(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation) 
            : translation(translation), scale(scale), rotation(rotation) {}
    
        operator glm::mat4() { return mat4(); }
    };

    struct ModelComponent {
        Shared<Model> model;

        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;
        ModelComponent(const Shared<Model>& model) : model(model) {}
    };

    class Script;

    struct ScriptComponent {
        Script* script = nullptr;

        Script*(*create)();
		void (*destroy)(ScriptComponent*);

		template<typename T>
		void bind()
		{
			create  = []() { 
                return static_cast<Script*>(new T()); 
            };

			destroy = [](ScriptComponent* s) { 
                delete s->script; 
                s->script = nullptr; 
            };
		}

    };
}