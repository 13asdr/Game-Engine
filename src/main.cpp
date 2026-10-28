#include "core/Window.h"

#include <cstdio>

#include <GLFW/glfw3.h>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

// 打印真实的 GL 信息。
// 这不是调试废话：它同时验证了「拿到了 4.5 上下文」和「有没有掉到软件渲染」，
// 是里程碑 1 的验收证据之一。
void printRendererInfo() {
    // glGetString 返回 const GLubyte*，必须 reinterpret_cast 成 const char* 才能给 printf 用。
    std::printf("GL_VERSION  = %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    std::printf("GL_RENDERER = %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    std::printf("GL_VENDOR   = %s\n", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    std::fflush(stdout);
}

}  // namespace

int main() {
    Window window(kWindowWidth, kWindowHeight, "C++ Game Engine - milestone 1");
    if (!window.valid()) {
        std::fprintf(stderr, "[main] window creation failed, abort\n");
        return 1;
    }

    window.setClearColor(0.12f, 0.14f, 0.18f, 1.0f);
    printRendererInfo();

    while (!window.shouldClose()) {
        window.pollEvents();

        // ESC 退出。里程碑 1 直接用 GLFW 轮询键位；
        // 等有输入系统时这段要移走，别让它长在 main 里。
        if (glfwGetKey(window.handle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }

        window.clear();
        window.swapBuffers();
    }

    return 0;
}
