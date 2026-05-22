#include <iostream>
#include "GLFW/glfw3.h"
#include "Log.h"
#include <assert.h>
#include "Window.h"

namespace HotelEngine
{
    namespace ClientCpp
    {
        void Window::Init(const char *Name, int Width, int Height)
        {
            HE_ASSERT(glfwInit() == true, "glfwInit Failed!");

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

            _Window = glfwCreateWindow(Width, Height, Name, NULL, NULL);
            HE_ASSERT(_Window != nullptr, "Window Creation Failed: " << Name << ", " << Width << "x" << Height);
            HE_PRINTLN("Window Created: " << Name << ", " << Width << "x" << Height );

            glfwMakeContextCurrent(_Window);
            glfwSwapInterval(1); // Enable vsync
        }

        void Window::Update()
        {
            glfwSwapBuffers(_Window);
        }

        void Window::PollEvents()
        {
            glfwPollEvents();
        }

        void Window::Destroy()
        {
            if (_Window == nullptr)
                return;

            glfwDestroyWindow(_Window);
            glfwTerminate();
            _Window = nullptr;
        }

        bool Window::ShouldWindowClose() const
        {
            return glfwWindowShouldClose(_Window);
        }
    } // namespace ClientCpp

} // namespace HotelEngine
