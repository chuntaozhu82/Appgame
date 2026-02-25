# 项目配置与环境要求

## 项目概述

本项目是一个跨平台钓鱼游戏应用程序，使用C++17开发，基于Appgame框架构建。项目支持PC端和移动端，实现了完整的跨设备兼容性和响应式设计。

## 环境检查结果

### 当前系统环境状态

| 工具/依赖 | 状态 | 说明 |
|----------|------|------|
| CMake | ❌ 未安装 | 需要3.16+版本 |
| Visual Studio | ❌ 未安装 | 需要2019或更高版本 |
| GCC/g++ | ❌ 未安装 | 需要支持C++17的版本 |
| SDL2 | ❌ 未安装 | 项目依赖库缺失 |
| Box2D | ❌ 未安装 | 项目依赖库缺失 |
| OpenAL | ❌ 未安装 | 项目依赖库缺失 |

### 系统PATH检查

当前系统PATH中包含以下相关路径：
- Node.js: ✅ 已安装
- Git: ✅ 已安装
- PowerShell 7: ✅ 已安装
- MySQL: ✅ 已安装

**结论：当前环境缺少C++编译器和项目依赖库，无法直接编译和运行项目。**

## 项目依赖详情

### 必需依赖

| 依赖名称 | 版本要求 | 用途 | 安装方式 |
|---------|---------|------|---------|
| CMake | 3.16+ | 构建系统 | [官网下载](https://cmake.org/download/) |
| C++编译器 | C++17支持 | 代码编译 | 见下方说明 |
| SDL2 | 2.0+ | 窗口管理和输入处理 | [官网下载](https://www.libsdl.org/) |
| Box2D | 2.4+ | 物理引擎 | [GitHub](https://github.com/erincatto/box2d) |
| OpenAL | 1.1+ | 音频处理 | [官网下载](https://www.openal.org/) |

### 编译器选择

#### Windows平台
推荐使用以下编译器之一：
1. **Visual Studio 2019/2022** (推荐)
   - 包含MSVC编译器
   - 集成开发环境
   - 调试工具完善

2. **MinGW-w64**
   - GCC编译器的Windows移植版
   - 轻量级
   - 命令行工具

#### macOS平台
- **Xcode Command Line Tools**
- **Clang** (系统自带)

#### Linux平台
- **GCC** 7+
- **Clang** 5+

## 安装指南

### Windows平台安装步骤

#### 1. 安装CMake

```powershell
# 方式1: 使用winget (推荐)
winget install Kitware.CMake

# 方式2: 使用Chocolatey
choco install cmake

# 方式3: 手动下载
# 访问 https://cmake.org/download/ 下载安装包
```

#### 2. 安装Visual Studio

1. 访问 [Visual Studio官网](https://visualstudio.microsoft.com/)
2. 下载Visual Studio Community 2022 (免费版)
3. 安装时选择以下工作负载：
   - "使用C++的桌面开发"
   - "CMake工具"
   - "Windows 10/11 SDK"

#### 3. 安装项目依赖

**方式1: 使用vcpkg (推荐)**

```powershell
# 安装vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# 安装依赖
.\vcpkg install sdl2:x64-windows
.\vcpkg install box2d:x64-windows
.\vcpkg install openal-soft:x64-windows

# 集成到Visual Studio
.\vcpkg integrate install
```

**方式2: 手动安装**

1. **SDL2**
   - 访问 https://www.libsdl.org/download-2.0.php
   - 下载开发库 (SDL2-devel-2.x.x-VC.zip)
   - 解压到 `external/SDL2` 目录

2. **Box2D**
   - 访问 https://github.com/erincatto/box2d/releases
   - 下载最新版本
   - 编译并安装到 `external/Box2D` 目录

3. **OpenAL**
   - 访问 https://www.openal.org/downloads/
   - 下载OpenAL SDK
   - 安装到 `external/OpenAL` 目录

### 目录结构要求

安装完成后，项目目录应包含以下结构：

```
Appgame/
├── external/
│   ├── SDL2/
│   │   ├── include/
│   │   │   └── SDL2/
│   │   │       ├── SDL.h
│   │   │       └── ...
│   │   └── lib/
│   │       ├── x64/
│   │       │   ├── SDL2.lib
│   │       │   └── SDL2main.lib
│   │       └── SDL2.dll
│   ├── Box2D/
│   │   ├── include/
│   │   │   └── box2d/
│   │   └── lib/
│   │       └── box2d.lib
│   └── OpenAL/
│       ├── include/
│       │   └── AL/
│       └── lib/
│           └── OpenAL32.lib
├── include/
├── src/
└── CMakeLists.txt
```

## 构建步骤

### 使用Visual Studio (推荐)

```powershell
# 1. 创建构建目录
mkdir build
cd build

# 2. 生成Visual Studio项目
cmake .. -G "Visual Studio 17 2022" -A x64

# 3. 构建项目
cmake --build . --config Release

# 4. 运行游戏
.\bin\Release\FishingGame.exe
```

### 使用命令行

```powershell
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# 3. 构建项目
cmake --build .

# 4. 运行游戏
.\bin\FishingGame.exe
```

### 运行测试

```powershell
# 在build目录中运行测试
ctest -C Release

# 或直接运行测试可执行文件
.\bin\Release\FishingGameTests.exe
```

## 预期结果

### 成功构建后的输出

构建成功后，将在 `build/bin` 目录下生成以下文件：

1. **FishingGame.exe** - 游戏主程序
2. **FishingGameTests.exe** - 测试程序
3. **AppgameCore.lib** - 核心引擎库
4. **SDL2.dll** - SDL2动态链接库
5. **OpenAL32.dll** - OpenAL动态链接库

### 运行游戏预期行为

1. 控制台输出：
```
Starting Fishing Game...
DefaultPlatform initialized
Platform: Default 1.0.0
Screen: 1920x1080 (1.77778)
DPI: 96
Device Type: Desktop
Input Type: Mouse
Scale Factor: 1
UI Scale Factor: 1
Initializing UI Manager...
Initialized HUD
Initialized InventoryUI
Initialized ShopUI
UIManager initialized successfully
Running game loop...
```

2. 游戏窗口：
   - 显示游戏界面
   - HUD显示在左上角和右上角
   - 支持鼠标交互

### 测试运行预期结果

```
========================================
Running All Tests
========================================

--- Test Suite: Platform ---
Running test: CreatePlatform... PASSED
Running test: InitializePlatform... PASSED
...
--- Test Suite: Config ---
Running test: CreateConfig... PASSED
...
--- Test Suite: FishingSystem ---
Running test: CreateFishingSystem... PASSED
...
--- Test Suite: UIManager ---
Running test: CreateUIManager... PASSED
...

========================================
Test Results Summary
========================================
Total Tests: 58
Passed: 58
Failed: 0
Skipped: 0
========================================
All tests passed successfully!
```

## 故障排除

### 常见问题

#### 1. CMake找不到依赖库

**错误信息**: `Could not find SDL2`

**解决方案**:
- 确保SDL2已安装到 `external/SDL2` 目录
- 或设置环境变量 `SDL2_DIR` 指向SDL2安装目录

#### 2. 编译器不支持C++17

**错误信息**: `C++17 feature not supported`

**解决方案**:
- 更新编译器到支持C++17的版本
- Visual Studio 2019 16.1+
- GCC 7+
- Clang 5+

#### 3. 链接错误

**错误信息**: `unresolved external symbol`

**解决方案**:
- 检查库文件是否正确放置在 `external/*/lib` 目录
- 确保库文件架构匹配 (x64 vs x86)
- 检查CMakeLists.txt中的链接配置

#### 4. 运行时缺少DLL

**错误信息**: `无法找到SDL2.dll`

**解决方案**:
- 将SDL2.dll复制到可执行文件目录
- 或将DLL路径添加到系统PATH

## 最低系统要求

### 开发环境

| 项目 | 最低要求 | 推荐配置 |
|-----|---------|---------|
| 操作系统 | Windows 10 | Windows 11 |
| 内存 | 8GB | 16GB+ |
| 磁盘空间 | 10GB | 20GB+ |
| CPU | 4核心 | 8核心+ |

### 运行环境

| 项目 | 最低要求 | 推荐配置 |
|-----|---------|---------|
| 操作系统 | Windows 7 | Windows 10/11 |
| 内存 | 2GB | 4GB+ |
| 显卡 | OpenGL 3.0 | OpenGL 3.3+ |
| 磁盘空间 | 100MB | 500MB+ |

## 下一步行动

### 立即需要安装的工具

1. **CMake** - 构建系统必需
2. **Visual Studio 2022** - 编译器和IDE
3. **vcpkg** - 依赖管理工具

### 安装命令摘要

```powershell
# 安装CMake
winget install Kitware.CMake

# 安装Visual Studio (手动下载安装)

# 安装vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat

# 安装依赖
.\vcpkg install sdl2:x64-windows box2d:x64-windows openal-soft:x64-windows

# 构建项目
cd d:\TRAE Test\Appgame
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

## 总结

当前项目无法在本地环境直接运行，因为缺少以下关键组件：

1. ❌ CMake构建系统
2. ❌ C++编译器 (Visual Studio或GCC)
3. ❌ 项目依赖库 (SDL2, Box2D, OpenAL)

请按照上述安装指南安装所需工具和依赖后，项目即可正常构建和运行。
