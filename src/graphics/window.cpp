#include "graphics/window.hpp"

#include "core/events/event.hpp"
#include "core/events/window_event.hpp"
#include "core/events/keyboard_event.hpp"
#include "core/events/mouse_event.hpp"
#include "core/events/mapper/glfw_event_mapper.hpp"

#include <stdexcept>

namespace CGEngine {

    Window::Window(const WindowData& props): m_data(props) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        
        m_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(m_window, &m_data);

        registerCallbacks();
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }


    void Window::registerCallbacks() {
  
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.eventCallback(event);
		});

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
            
            data.width = static_cast<uint32_t>(width);
            data.height = static_cast<uint32_t>(height);
            data.frameBufferResized = true;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int glfwKey, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyCode key = mapGLFWKey(glfwKey);

			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});


    }
}