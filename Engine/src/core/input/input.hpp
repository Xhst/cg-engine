#pragma once

#include "application.hpp"
#include "core/input/key_code.hpp"
#include "core/input/mapper/glfw_input_mapper.hpp"

#include <glm/glm.hpp>

namespace CGEngine {

    class Input {
    public:
        static bool isKeyPressed(KeyCode key) {
            return glfwGetKey(getWindow(), mapToGLFWKey(key)) == GLFW_PRESS;
        }

        static bool isMouseButtonPressed(MouseButton button) {
            return glfwGetMouseButton(getWindow(), mapToGLFWMouseButton(button)) == GLFW_PRESS;
        }

        static glm::vec2 getMousePosition() {
            double x, y;
		    glfwGetCursorPos(getWindow(), &x, &y);
            return { x, y };
        }

    private:
        static GLFWwindow* getWindow() {
            return Application::get().getWindow().getBaseWindow();
        }
    };
}