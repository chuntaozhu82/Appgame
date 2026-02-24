# Appgame 跨平台游戏框架

Appgame 是一个专为移动平台设计的跨平台游戏框架，支持 iOS 和 Android 系统。该框架提供了完整的游戏开发工具链，包括游戏循环管理、图形渲染、输入处理、资源管理、物理碰撞检测、音频播放、数据存储和调试工具等核心功能。

## 核心特性

- **跨平台兼容性**：支持 iOS 和 Android 系统
- **高性能渲染**：基于 OpenGL ES 的图形渲染引擎
- **灵活的游戏循环**：支持固定和可变时间步长模式
- **全面的输入处理**：支持触摸、键盘和鼠标输入
- **高效的资源管理**：支持异步加载和缓存机制
- **集成物理引擎**：基于 Box2D 的碰撞检测系统
- **跨平台音频**：支持音效和音乐播放
- **安全的数据存储**：提供统一的数据存储接口
- **强大的调试工具**：内置日志记录和性能分析

## 目录结构

```
Appgame/
├── CMakeLists.txt          # 主CMake配置文件
├── README.md               # 项目说明文档
├── ARCHITECTURE.md         # 架构设计文档
├── .gitignore              # Git忽略文件
├── include/                # 公共头文件
│   ├── core/               # 核心引擎头文件
│   ├── platform/           # 平台抽象层头文件
│   └── utils/              # 工具类头文件
├── src/                    # 源代码
│   ├── core/               # 核心引擎实现
│   ├── platform/           # 平台特定实现
│   │   ├── ios/            # iOS平台实现
│   │   ├── android/        # Android平台实现
│   │   └── common/         # 跨平台通用实现
│   └── utils/              # 工具类实现
├── external/               # 第三方依赖
│   ├── SDL2/               # SDL2库
│   ├── Box2D/              # Box2D物理引擎
│   └── OpenAL/             # OpenAL音频库
└── examples/               # 示例代码
    └── hello_world/        # Hello World示例
```

## 技术栈

- **核心语言**：C++17
- **跨平台库**：SDL2
- **图形API**：OpenGL ES 3.0+
- **物理引擎**：Box2D
- **音频API**：OpenAL
- **构建系统**：CMake

## 快速开始

### 环境要求

- CMake 3.16+
- C++17 兼容编译器
- iOS: Xcode 12+
- Android: Android Studio 4.0+, NDK r21+

### 构建步骤

1. **克隆仓库**
   ```bash
   git clone https://github.com/yourusername/Appgame.git
   cd Appgame
   ```

2. **配置依赖**
   - 下载并配置 SDL2、Box2D 和 OpenAL 库到 `external/` 目录

3. **构建项目**
   - **iOS**：
     ```bash
     mkdir -p build/ios
     cd build/ios
     cmake -G Xcode ../..
     # 然后使用 Xcode 打开生成的项目
     ```

   - **Android**：
     ```bash
     mkdir -p build/android
     cd build/android
     cmake -G "Android Gradle - Ninja" ../..
     # 然后使用 Android Studio 打开生成的项目
     ```

## 示例代码

### Hello World

```cpp
#include "core/GameLoop.h"
#include "core/Graphics.h"
#include "core/Input.h"

using namespace Appgame;

int main() {
    // 初始化图形系统
    GraphicsManager::getInstance().init();
    
    // 创建渲染器
    auto renderer = GraphicsManager::getInstance().createRenderer();
    renderer->init();
    
    // 初始化输入系统
    InputManager::getInstance().init();
    auto inputHandler = InputManager::getInstance().createInputHandler();
    inputHandler->init();
    
    // 创建游戏循环
    GameLoop gameLoop;
    gameLoop.setTargetFPS(60);
    
    // 注册回调函数
    gameLoop.setUpdateCallback([&](float deltaTime) {
        // 更新游戏逻辑
        inputHandler->update();
    });
    
    gameLoop.setRenderCallback([&](float alpha) {
        // 渲染游戏
        renderer->beginRender();
        
        // 绘制背景
        renderer->drawRect(Rect(0, 0, 800, 600), Color(0.2f, 0.3f, 0.4f, 1.0f));
        
        // 绘制文本或精灵
        
        renderer->endRender();
    });
    
    // 启动游戏循环
    gameLoop.start();
    
    return 0;
}
```

## 文档

- **架构设计**：[ARCHITECTURE.md](ARCHITECTURE.md)
- **API参考**：即将推出
- **教程**：即将推出

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！

## 联系方式

- 项目主页：https://github.com/yourusername/Appgame
- 问题反馈：https://github.com/yourusername/Appgame/issues
