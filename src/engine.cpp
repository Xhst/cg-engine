#include "engine.hpp"
#include "core/memory.hpp"
#include "scene/ecs/entity.hpp"
#include "graphics/render_systems/simple_render_system.hpp"


#include <iostream>
#include <array>
#include <stdexcept>
#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace CGEngine {

    Engine::Engine() {
        loadScene();
    }

    Engine::~Engine() {}

    void Engine::run() {
        SimpleRenderSystem simpleRenderSystem(m_device, m_renderer.getSwapChainRenderPass());

        while (isRunning()) {
            glfwPollEvents();
            
            if (auto commandBuffer = m_renderer.beginFrame()) {
                m_renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderScene(commandBuffer, m_scene);
                m_renderer.endSwapChainRenderPass(commandBuffer);
                m_renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(m_device.device());
    }

    bool Engine::isRunning() {
        return !m_window.shouldClose();
    }

    void Engine::loadScene() {
        std::vector<Model::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        auto model = createShared<Model>(m_device, vertices);

        auto rotation = .25f * glm::two_pi<float>();
        m_scene.createEntity("triangle")
            .add<Transform2dComponent>(glm::vec2{0.5f, 0.0f}, glm::vec2{0.5f, 0.5f}, rotation)
            .add<ColorComponent>(glm::vec3{0.6f, 0.0f, 0.5f})
            .add<ModelComponent>(model);
    }
}