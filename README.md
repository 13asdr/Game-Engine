# C++ 游戏引擎

从零实现的简单 C++ 游戏引擎。当前进度：**里程碑 1 —— 窗口 + 清屏**。

项目协作规则见 [AI_PROJECT_RULES.md](AI_PROJECT_RULES.md)。

## 技术栈

| 项 | 选择 | 为什么 |
|---|---|---|
| 语言 | C++17 | — |
| 构建 | xmake | 构建与包管理同一工具，不双 manifest |
| 包管理 | xmake 内置 xrepo | 从源码按当前工具链编译，避免 ABI triplet 分裂 |
| 窗口/输入 | GLFW 3.4 | 最小：只做窗口/上下文/输入，Win/Linux 同源 |
| 图形 API | OpenGL 4.5 Core | 状态机简单、教程多、调试直观 |
| 工具链 | MSVC 首选，MinGW 备选 | xmake 自动探测；**两者不可混用** |

## 环境准备

1. 安装 xmake（当前最新 v3.1.2）：

   ```powershell
   winget install xmake
   # 或：irm https://xmake.io/psget.text | iex
   ```

2. 重开一个终端，验证：

   ```powershell
   xmake --version
   xmake show -l toolchains   # 应能看到 msvc（首选）或 mingw（备选）
   ```

3. 首次编译会自动从 xmake-repo 拉 GLFW 源码并编译，**需要联网**。

4. Linux 侧额外需要 X11 开发头（GLFW 的 X11 后端依赖）：

   ```bash
   sudo apt install xorg-dev libgl1-mesa-dev
   ```

   本里程碑暂未在 Linux 上实际验证。

## 编译与运行

```powershell
xmake f -m debug    # 首次配置
xmake               # 编译
xmake run           # 运行
```

切工具链（配置项，不改代码）：

```powershell
xmake f --toolchain=mingw
```

## 验收清单（里程碑 1）

1. `xmake` 编译零警告。
2. `xmake run` 弹出 1280×720 窗口，背景色为 `(0.12, 0.14, 0.18)`。
3. 控制台 `GL_VERSION` 以 `4.5` 开头。
4. 关闭按钮 / `ESC` 都能退出；`xmake run` 退出码 0。
5. 拖动、缩放窗口不崩溃，清屏铺满整个客户区。
6. CPU 占用平稳，无单核 100% 空转。

## 本机实测结果

> 由你运行后填写，作为「这台机器能跑 OpenGL 4.5」的凭据。

```
GL_VERSION  = 待填
GL_RENDERER = 待填
GL_VENDOR   = 待填
```

## 目录结构

```
xmake.lua              # 构建与依赖描述
xmake-requires.lock    # 依赖锁文件，首次编译后生成，必须提交
src/
  main.cpp             # 入口：主循环
  core/
    Window.h           # 单窗口 RAII 封装
    Window.cpp
```

## 当前刻意不做

GLAD 等函数加载器（里程碑 2 画三角形时才需要）、`Application`/`Engine` 基类、渲染器抽象层、ECS、场景/资源管理、物理、脚本、编辑器 GUI、音频、网络。

## 常见坑

- **不要自己 include `<GL/gl.h>`**：Windows 上它依赖 `<windows.h>` 的类型定义，只 include `<GLFW/glfw3.h>` 即可。
- **`glfwSwapInterval(1)` 不是可选项**：不设就是满核空转。
- **Core Profile 下固定管线永久不可用**：`glBegin/glEnd` 从里程碑 1 起就是禁区。
- **工具链不要混用 MSVC 与 MinGW**：ABI 不兼容，问题会拖到链接期才暴露。
