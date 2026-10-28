#include "core/Window.h"

#include <cstdio>

// 这里只 include GLFW 头，不要自己单独 include <GL/gl.h>：
// Windows 上 <GL/gl.h> 依赖 <windows.h> 里的类型定义，GLFW 头已经替你处理好了，
// 而且它默认会带上 <GL/gl.h>，所以 glClear/glClearColor/glViewport 可以直接用。
#include <GLFW/glfw3.h>

namespace {

void onGlfwError(int code, const char* description) {
    std::fprintf(stderr, "[glfw] error %d: %s\n", code, description ? description : "(no description)");
}

}  // namespace

Window::Window(int width, int height, const std::string& title) {
    // 错误回调必须在 glfwInit 之前装上，否则初始化失败只会得到一个 false，没有原因。
    glfwSetErrorCallback(onGlfwError);

    if (glfwInit() != GLFW_TRUE) {
        std::fprintf(stderr, "[window] glfwInit failed\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        // 最常见原因：驱动/远程桌面不支持 4.5 core，或者是纯软件渲染环境。
        std::fprintf(stderr, "[window] glfwCreateWindow failed: OpenGL 4.5 core profile unavailable\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    // 垂直同步开 1：主循环靠它节流。不设的话就是满核空转，最常见的「能跑但不对劲」。
    glfwSwapInterval(1);
}

Window::~Window() { destroy(); }

void Window::destroy() {
    // m_window 为空有两种来源：glfwInit 失败、或 glfwCreateWindow 失败。
    // 后者的构造路径里已经自行 glfwTerminate 过了，所以这里直接返回，避免重复 terminate。
    if (!m_window) {
        return;
    }

    glfwDestroyWindow(m_window);
    m_window = nullptr;
    glfwTerminate();
}

bool Window::shouldClose() const {
    if (!m_window) {
        return true;
    }
    return glfwWindowShouldClose(m_window) != 0;
}

void Window::pollEvents() const { glfwPollEvents(); }

void Window::swapBuffers() const {
    if (m_window) {
        glfwSwapBuffers(m_window);
    }
}

void Window::setClearColor(float r, float g, float b, float a) const {
    glClearColor(r, g, b, a);
}

void Window::clear() const { glClear(GL_COLOR_BUFFER_BIT); }
