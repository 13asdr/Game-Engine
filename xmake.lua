-- 里程碑 1：窗口 + 清屏
-- 构建：xmake
-- 依赖：xmake 内置包管理器 xrepo（不是 vcpkg，vcpkg 仅作逃生通道）

add_rules("mode.debug", "mode.release")
set_languages("c++17")
set_warnings("all", "extra")

-- 生成 xmake-requires.lock，把依赖版本钉死(包依赖锁定)（类似 Cargo.lock）。
-- 该文件必须提交进 git，否则依赖会随时间漂移。
set_policy("package.requires_lock", true)  

-- 从 xmake-repo 拉 GLFW 源码，按当前工具链现场编译。
-- glfw_include = "system" 很关键：xmake 的 glfw 包默认 glfw_include="none"，
-- 会给使用方定义 GLFW_INCLUDE_NONE，导致 glfw3.h 不包含 <GL/gl.h>，
-- glClear/glClearColor 这些 OpenGL 1.1 函数就找不到声明。改成 system 才走默认包含。
-- 逃生通道：xrepo 拉不到时改成 add_requires("vcpkg::glfw3", {alias = "glfw"})
add_requires("glfw 3.4", {configs = {glfw_include = "system"}})

target("engine")
    set_kind("binary")

    -- 中文 Windows 上 MSVC 默认按代码页 936(GBK) 读源码，UTF-8 中文注释会吞掉行尾换行，
    -- 导致整份文件解析错位（C4819 + 一堆莫名语法错误）。这条让 MSVC 生成 /utf-8。
    set_encodings("utf-8")

    add_files("src/**.cpp")
    add_includedirs("src")
    add_packages("glfw")

    -- 里程碑 1 只用到 OpenGL 1.1 函数（glClear/glClearColor/glViewport/glGetString），
    -- 它们在系统 GL 库里就有，所以现在不需要 GLAD 之类的加载器。
    if is_plat("windows") then
        add_syslinks("opengl32")
    elseif is_plat("linux") then
        add_syslinks("GL")
    end
