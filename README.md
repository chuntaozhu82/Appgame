# Appgame - 跨平台游戏应用框架

Appgame是一个功能完整的跨平台游戏应用框架，支持iOS和Android系统，提供了丰富的核心模块，便于游戏开发者快速构建高质量的游戏应用。

## 框架特性

### 核心模块
- **游戏循环管理系统**：支持固定和可变时间步长，提供帧率控制和性能统计
- **图形渲染引擎**：基于OpenGL ES 3.0+，支持2D精灵渲染和批量处理
- **用户输入处理模块**：支持触摸、键盘和鼠标输入，提供统一的输入接口
- **资源加载与管理系统**：支持异步资源加载，提供资源缓存和管理
- **物理碰撞检测系统**：集成Box2D物理引擎，支持刚体碰撞检测
- **音效与音乐播放模块**：基于OpenAL，支持多声道音频播放
- **游戏数据存储系统**：支持本地存储和加密存储，提供异步操作接口
- **调试与日志记录工具**：支持不同级别的日志记录，提供控制台和文件输出

### 技术选型
- **核心语言**：C++17
- **窗口管理**：SDL2
- **图形渲染**：OpenGL ES 3.0+
- **物理引擎**：Box2D
- **音频处理**：OpenAL
- **构建系统**：CMake
- **版本控制**：Git

## 目录结构

```
Appgame/
├── include/
│   └── core/          # 核心模块头文件
│       ├── GameLoop.h  # 游戏循环管理
│       ├── Graphics.h  # 图形渲染引擎
│       ├── Input.h     # 用户输入处理
│       ├── Resource.h  # 资源加载与管理
│       ├── Physics.h   # 物理碰撞检测
│       ├── Audio.h     # 音效与音乐播放
│       ├── Storage.h   # 游戏数据存储
│       └── Log.h       # 调试与日志记录
├── src/
│   └── core/          # 核心模块实现
│       ├── GameLoop.cpp
│       ├── Graphics.cpp
│       ├── Input.cpp
│       ├── Resource.cpp
│       ├── Physics.cpp
│       ├── Audio.cpp
│       ├── Storage.cpp
│       └── Log.cpp
├── examples/         # 示例代码
│   └── hello_world/   # Hello World示例
├── CMakeLists.txt    # CMake构建配置
├── ARCHITECTURE.md   # 架构设计文档
└── README.md         # 项目说明文档
```

## 安装与配置

### 依赖项
- CMake 3.16+
- C++17兼容编译器
- SDL2
- OpenGL ES 3.0+
- Box2D
- OpenAL

### 构建步骤

#### Windows
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A Win32
cmake --build . --config Release
```

#### iOS
```bash
mkdir build
cd build
cmake .. -G "Xcode" -DCMAKE_SYSTEM_NAME=iOS
cmake --build . --config Release
```

#### Android
```bash
# 使用Android Studio打开项目或使用NDK构建
```

## 使用示例

### Hello World示例

```cpp
#include "core/GameLoop.h"
#include "core/Graphics.h"
#include "core/Input.h"
#include "core/Log.h"

using namespace Appgame;

class HelloWorldGame : public GameLoop {
public:
    HelloWorldGame() {
        setName("Hello World Game");
        setFixedTimeStep(true);
        setTargetFPS(60);
    }

protected:
    bool onInit() override {
        // 初始化日志系统
        LogManager::getInstance().init(LogTarget::BOTH, "hello_world.log");
        
        // 初始化图形系统
        if (!Graphics::init()) {
            return false;
        }

        // 创建窗口
        if (!Graphics::createWindow(800, 600, "Hello World")) {
            return false;
        }

        // 初始化输入系统
        if (!Input::init()) {
            return false;
        }

        return true;
    }

    void onUpdate(float deltaTime) override {
        // 处理输入
        if (Input::isKeyPressed(KeyCode::ESCAPE)) {
            quit();
        }
    }

    void onRender() override {
        // 清屏
        Graphics::getRenderer()->clear(Color(0.1f, 0.1f, 0.2f, 1.0f));

        // 绘制矩形
        Graphics::getRenderer()->drawRect(
            Rect(100, 100, 50, 50),
            Color(1.0f, 0.5f, 0.0f, 1.0f),
            true
        );

        // 交换缓冲区
        Graphics::swapBuffers();
    }

    void onCleanup() override {
        Input::cleanup();
        Graphics::cleanup();
    }
};

int main(int argc, char* argv[]) {
    HelloWorldGame game;
    return game.run();
}
```

## API文档

### 游戏循环管理

#### GameLoop类
- `setName(const std::string& name)`：设置游戏名称
- `setFixedTimeStep(bool fixed)`：设置是否使用固定时间步长
- `setTargetFPS(int fps)`：设置目标帧率
- `run()`：运行游戏循环
- `quit()`：退出游戏循环
- `onInit()`：初始化回调
- `onUpdate(float deltaTime)`：更新回调
- `onRender()`：渲染回调
- `onCleanup()`：清理回调

### 图形渲染

#### Graphics类
- `init()`：初始化图形系统
- `cleanup()`：清理图形系统
- `createWindow(int width, int height, const std::string& title)`：创建窗口
- `getRenderer()`：获取渲染器实例
- `swapBuffers()`：交换缓冲区

#### Renderer类
- `drawRect(const Rect& rect, const Color& color, bool filled)`：绘制矩形
- `drawLine(const Vec2& start, const Vec2& end, const Color& color, float width)`：绘制线条
- `drawSprite(const Texture* texture, const Rect& srcRect, const Rect& destRect, float rotation, const Vec2& origin, const Color& color)`：绘制精灵

### 用户输入

#### Input类
- `init()`：初始化输入系统
- `cleanup()`：清理输入系统
- `isKeyPressed(KeyCode key)`：检查按键是否按下
- `isMouseButtonPressed(MouseButton button)`：检查鼠标按钮是否按下
- `getMousePosition()`：获取鼠标位置
- `getTouches()`：获取触摸点列表

### 资源管理

#### ResourceManager类
- `init()`：初始化资源管理器
- `cleanup()`：清理资源管理器
- `loadTexture(const std::string& path)`：加载纹理
- `loadSound(const std::string& path)`：加载音效
- `loadMusic(const std::string& path)`：加载音乐
- `unload(const std::string& path)`：卸载资源
- `get(const std::string& path)`：获取资源

### 物理引擎

#### PhysicsManager类
- `init()`：初始化物理管理器
- `cleanup()`：清理物理管理器
- `getWorld()`：获取物理世界

#### PhysicsWorld类
- `createBody(const BodyDef& def)`：创建刚体
- `destroyBody(RigidBody* body)`：销毁刚体
- `step(float deltaTime)`：物理世界步进
- `raycast(const Vec2& start, const Vec2& end, RaycastCallback* callback)`：射线检测

### 音频系统

#### AudioManager类
- `init()`：初始化音频管理器
- `cleanup()`：清理音频管理器
- `loadSound(const std::string& path)`：加载音效
- `loadMusic(const std::string& path)`：加载音乐
- `playSound(Sound* sound, bool loop = false)`：播放音效
- `playMusic(Music* music, bool loop = true)`：播放音乐
- `stopSound(Sound* sound)`：停止音效
- `stopMusic()`：停止音乐
- `setVolume(float volume)`：设置音量

### 数据存储

#### Storage类
- `save(const std::string& key, const T& value)`：保存数据
- `load(const std::string& key, T& value)`：加载数据
- `saveAsync(const std::string& key, const T& value)`：异步保存数据
- `loadAsync(const std::string& key, T& value)`：异步加载数据
- `remove(const std::string& key)`：删除数据
- `exists(const std::string& key)`：检查数据是否存在
- `clear()`：清除所有数据
- `saveEncrypted(const std::string& key, const std::string& value)`：加密保存数据
- `loadEncrypted(const std::string& key, std::string& value)`：加密加载数据

### 调试与日志

#### LogManager类
- `init(LogTarget target, const std::string& logFile)`：初始化日志系统
- `cleanup()`：清理日志系统
- `setLogLevel(LogLevel level)`：设置日志级别
- `log(LogLevel level, const std::string& message)`：输出日志

#### 日志宏
- `LOG_DEBUG(message)`：输出调试级别日志
- `LOG_INFO(message)`：输出信息级别日志
- `LOG_WARN(message)`：输出警告级别日志
- `LOG_ERROR(message)`：输出错误级别日志
- `LOG_FATAL(message)`：输出致命级别日志

#### Debug类
- `assert(bool condition, const std::string& message)`：断言检查
- `breakpoint()`：断点
- `checkMemoryLeaks()`：检查内存泄漏
- `startProfiling(const std::string& name)`：开始性能分析
- `endProfiling(const std::string& name)`：结束性能分析
- `outputProfilingResults()`：输出性能分析结果

## 示例代码

### Hello World示例

详细的Hello World示例代码位于 `examples/hello_world/` 目录，展示了如何使用框架的各个核心模块。

## 构建与运行

### 构建步骤

1. 克隆仓库：`git clone https://github.com/yourusername/Appgame.git`
2. 进入目录：`cd Appgame`
3. 创建构建目录：`mkdir build && cd build`
4. 配置CMake：`cmake ..`
5. 构建项目：`cmake --build .`

### 运行示例

构建完成后，运行Hello World示例：

```bash
./examples/hello_world/hello_world
```

## 许可证

MIT License

## 贡献

欢迎提交Issue和Pull Request！

## 联系方式

- 项目地址：https://github.com/yourusername/Appgame
- 邮箱：your.email@example.com
