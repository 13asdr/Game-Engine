#pragma once

#include <string>

// 前向声明，避免把 GLFW 头文件泄漏给所有包含 Window.h 的翻译单元。
struct GLFWwindow;

// 单个窗口的 RAII 封装：负责 GLFW 初始化、窗口创建/销毁，以及最基本的清屏。
//
// 已知局限（刻意不抽象）：glfwInit/glfwTerminate 放在这里，意味着「全进程只有一个窗口」。
// 等真的出现多窗口需求时，再把 init/terminate 上提为独立的 GlfwContext，Window 只保留窗口本身。
// 现在就拆是过度设计 —— 没有第二个窗口来验证拆得对不对。
class Window {
public:
    // 构造时按 OpenGL 4.5 Core Profile 创建窗口；失败不抛异常，用 valid() 查询。
    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // 构造是否成功。为 false 时其余成员函数不得调用（shouldClose 除外）。
    bool valid() const { return m_window != nullptr; }

    bool shouldClose() const;
    void pollEvents() const;
    void swapBuffers() const;

    void setClearColor(float r, float g, float b, float a) const;
    void clear() const;

    // 暴露原始句柄，供输入轮询等暂时还没被封装的操作用。
    GLFWwindow* handle() const { return m_window; }

private:
    void destroy();

    GLFWwindow* m_window = nullptr;
};
