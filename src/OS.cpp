#include "OS.h"

#include <iostream>
#include "TrillekGame.h"

#ifdef __APPLE__
// Needed so we can disable retina support for our window.
#define GLFW_EXPOSE_NATIVE_COCOA 1
#define GLFW_EXPOSE_NATIVE_NSGL 1
#include <GLFW/glfw3native.h>
// We can't just include objc/runtime.h and objc/message.h because glfw is too forward thinking for its own good.
typedef void* SEL;
extern "C" id objc_msgSend(id self, SEL op, ...);
extern "C" SEL sel_getUid(const char *str);
#endif

namespace trillek {
    // Error helper function used by GLFW for error messaging.
    // Currently outputs to std::cout.
    static void ErrorCallback(int error, const char* description) {
        std::cout << description << std::endl;
    }

    bool OS::InitializeWindow(const int width, const int height, const std::string title,
        const unsigned int glMajor /*= 3*/, const unsigned int glMinor /*= 2*/) {
        glfwSetErrorCallback(ErrorCallback);

        // Initialize the library.
        if (glfwInit() != GL_TRUE) {
            return false;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

        // Create a windowed mode window and its OpenGL context.
        this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (!this->window) {
            glfwTerminate();
            return false;
        }

        this->width = width;
        this->height = height;

#ifdef __APPLE__
        // Force retina displays to create a 1x framebuffer so we don't choke our fillrate.
        id cocoaWindow = glfwGetCocoaWindow(this->window);
        id cocoaGLView = ((id (*)(id, SEL)) objc_msgSend)(cocoaWindow, sel_getUid("contentView"));
        ((void (*)(id, SEL, bool)) objc_msgSend)(cocoaGLView, sel_getUid("setWantsBestResolutionOpenGLSurface:"), false);
#endif

        // Make the window's context current.
        glfwMakeContextCurrent(this->window);

#ifndef __APPLE__
        // setting glewExperimental fixes a glfw context problem
        // (tested on Ubuntu 13.04)
        glewExperimental = GL_TRUE;

        // Init GLEW.
        GLuint error = glewInit();
        if (error != GLEW_OK) {
            return false;
        }
#endif

        // Associate a pointer for this instance with this window.
        glfwSetWindowUserPointer(this->window, this);

        // Set up some callbacks.
        glfwSetWindowSizeCallback(this->window, &OS::windowResized);
        glfwSetKeyCallback(this->window, &OS::keyboardEvent);
        glfwSetCursorPosCallback(this->window, &OS::mouseMoveEvent);
        glfwSetCharCallback(this->window, &OS::characterEvent);
        glfwSetMouseButtonCallback(this->window, &OS::mouseButtonEvent);
        glfwSetWindowFocusCallback(this->window, &OS::windowFocusChange);

        glfwGetCursorPos(this->window, &this->oldMouseX, &this->oldMouseY);

        glfwSetWindowCloseCallback(this->window, &OS::TrillekCloseWindowCallback);

        return true;
    }

    void OS::Terminate() {
        glfwTerminate();
    }

    bool OS::Closing() {
        return glfwWindowShouldClose(this->window) > 0;
    }

    void OS::SwapBuffers() {
        glfwSwapBuffers(this->window);
    }

    int OS::OSMessageLoop() {
        glfwPollEvents();
        return REQUEUE;
    }

    int OS::GetWindowWidth() {
        return this->width;
    }

    int OS::GetWindowHeight() {
        return this->height;
    }

    std::chrono::duration<double,std::ratio<1>> OS::GetTime() {
        return std::chrono::duration<double,std::ratio<1>>(glfwGetTime());
    }

    void OS::windowResized(GLFWwindow* window, int width, int height) {
        // Get the user pointer and cast it.
        OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

        if (os) {
            os->UpdateWindowSize(width, height);
        }
    }

    void OS::keyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Get the user pointer and cast it.
        OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

        if (os) {
            os->DispatchKeyboardEvent(key, scancode, action, mods);
        }
    }

    void OS::characterEvent(GLFWwindow* window, unsigned int uchar) {
        // Get the user pointer and cast it.
        OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

        if (os) {
            os->DispatchCharacterEvent(uchar);
        }
    }

    void OS::mouseMoveEvent(GLFWwindow* window, double x, double y) {
        // Get the user pointer and cast it.
        OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

        if (os) {
            os->DispatchMouseMoveEvent(x, y);
        }
    }

    void OS::mouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {
        // Get the user pointer and cast it.
        OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

        if (os) {
            os->DispatchMouseButtonEvent(button, action, mods);
        }
    }

    void OS::windowFocusChange(GLFWwindow* window, int focused) {
        if (focused == GL_FALSE) {
            // Get the user pointer and cast it.
            OS* os = static_cast<OS*>(glfwGetWindowUserPointer(window));

            if (os) {
                // TODO: Implement a DispatchWindowFocusEvent() method in OS
                // TODO: Dispatch a focus changed event.
            }
        }
    }

    void OS::UpdateWindowSize(const int width, const int height) {
        this->width = width;
        this->height = height;
    }

    void OS::DispatchKeyboardEvent(const int key, const int scancode, const int action, const int mods) {
        if (action == GLFW_PRESS) {
            // TODO: Dispatch a key down event.
        }
        else if (action == GLFW_REPEAT) {
            // TODO: Dispatch a key repeat event.
        }
        else if (action == GLFW_RELEASE) {
            // TODO: Dispatch a key up event.
        }
    }

    void OS::DispatchCharacterEvent(const unsigned int uchar) {
        // TODO: Dispatch a character event.
    }

    void OS::DispatchMouseMoveEvent(const double x, const double y) {
        // TODO: Dispatch a mouse move event.

        // If we are in mouse lock we will snap the mouse to the middle of the screen.
        if (this->mouseLock) {
            this->oldMouseX = this->width / 2;
            this->oldMouseY = this->height / 2;
            glfwSetCursorPos(this->window, this->oldMouseX, this->oldMouseY);
        }
        else {
            this->oldMouseX = x;
            this->oldMouseY = y;
        }
    }

    void OS::DispatchMouseButtonEvent(const int button, const int action, const int mods) {
        if (action == GLFW_PRESS) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                // TODO: Dispatch a left mouse button down event.
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                // TODO: Dispatch a right mouse button down event.
            }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                // TODO: Dispatch a middle mouse button down event.
            }
        }
        else if (action == GLFW_RELEASE) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                // TODO: Dispatch a left mouse button up event.
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                // TODO: Dispatch a right mouse button up event.
            }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                // TODO: Dispatch a middle mouse button up event.
            }
        }
    }

    void OS::ToggleMouseLock() {
        this->mouseLock = !this->mouseLock;
        if (this->mouseLock) {
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    void OS::SetMousePosition(double x, double y) {
        glfwSetCursorPos(this->window, x, y);
    }

    void OS::TrillekCloseWindowCallback(GLFWwindow* w) {
        TrillekGame::NotifyCloseWindow();
    }
}
