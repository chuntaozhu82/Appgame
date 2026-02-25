# Trae C++ 扩展配置指南

## 前提条件

在配置Trae之前，需要先安装以下工具：

| 工具 | 安装命令 | 说明 |
|-----|---------|------|
| CMake | `winget install Kitware.CMake` | 构建系统 |
| Ninja | `winget install Ninja-build.Ninja` | 构建工具 |
| MinGW-w64 | `winget install MSYS2.MSYS2` | GCC编译器 |

## 第一步：安装MinGW-w64编译器

### 1. 安装MSYS2
```powershell
winget install MSYS2.MSYS2
```

### 2. 打开MSYS2 UCRT64终端
从开始菜单找到 "MSYS2 UCRT64" 并打开

### 3. 安装GCC工具链
在MSYS2终端中运行：
```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-gdb
```

### 4. 添加到系统PATH
将以下路径添加到Windows系统环境变量：
```
C:\msys64\mingw64\bin
```

## 第二步：安装vcpkg依赖

```powershell
cd C:\vcpkg

# 使用MinGW triplet安装依赖
.\vcpkg install sdl2:x64-mingw-dynamic box2d:x64-mingw-dynamic openal-soft:x64-mingw-dynamic
```

## 第三步：配置Trae

### 方式1：使用工作区文件

我已经为您创建了工作区配置文件 `fishing-game.code-workspace`，直接在Trae中打开这个文件即可。

### 方式2：手动配置

#### 1. 打开Trae设置
- 按 `Ctrl + ,` 打开设置
- 或点击 文件 → 首选项 → 设置

#### 2. 搜索并配置以下设置

**C/C++ 扩展设置：**
```
C_Cpp > Default: Compiler Path
→ 填入: C:/msys64/mingw64/bin/g++.exe

C_Cpp > Default: C Standard
→ 选择: c17

C_Cpp > Default: C++ Standard
→ 选择: c++17

C_Cpp > Default: Intelli Sense Mode
→ 选择: windows-gcc-x64
```

**CMake 设置：**
```
Cmake: Configure On Open
→ 勾选

Cmake: Build Directory
→ 填入: ${workspaceFolder}/build

Cmake: Generator
→ 选择: Ninja

Cmake: Configure Args
→ 添加: -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

#### 3. 创建 c_cpp_properties.json

在项目根目录创建 `.vscode/c_cpp_properties.json`：

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/vcpkg/installed/x64-mingw-dynamic/include",
                "C:/msys64/mingw64/include"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.22621.0",
            "compilerPath": "C:/msys64/mingw64/bin/g++.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "windows-gcc-x64",
            "configurationProvider": "ms-vscode.cmake-tools"
        }
    ],
    "version": 4
}
```

#### 4. 创建 settings.json

在项目根目录创建 `.vscode/settings.json`：

```json
{
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.generator": "Ninja",
    "cmake.configureArgs": [
        "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    ],
    "cmake.environment": {
        "PATH": "C:/msys64/mingw64/bin;${env:PATH}"
    },
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "C_Cpp.default.cppStandard": "c++17",
    "C_Cpp.default.intelliSenseMode": "windows-gcc-x64",
    "files.associations": {
        "*.h": "cpp",
        "*.cpp": "cpp"
    }
}
```

## 第四步：构建项目

### 方式1：使用命令面板
1. 按 `Ctrl + Shift + P`
2. 输入 `CMake: Configure`
3. 等待配置完成
4. 按 `Ctrl + Shift + P`
5. 输入 `CMake: Build`

### 方式2：使用终端
```powershell
# 配置项目
cmake -B build -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# 构建项目
cmake --build build --config Release
```

## 第五步：运行和调试

### 运行游戏
```powershell
./build/bin/FishingGame.exe
```

### 运行测试
```powershell
./build/bin/FishingGameTests.exe
```

### 调试配置

创建 `.vscode/launch.json`：

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(Windows) Launch",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/bin/FishingGame.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "console": "externalTerminal",
            "preLaunchTask": "CMake: build"
        },
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/bin/FishingGame.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
            "miDebuggerPath": "C:/msys64/mingw64/bin/gdb.exe",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

## 常见问题

### 1. 找不到头文件
- 检查 `c_cpp_properties.json` 中的 `includePath`
- 确保vcpkg依赖已安装

### 2. CMake配置失败
- 检查CMake和Ninja是否在PATH中
- 检查vcpkg工具链文件路径是否正确

### 3. 链接错误
- 确保使用正确的triplet (x64-mingw-dynamic)
- 检查库文件是否存在于vcpkg安装目录

### 4. 中文乱码
- 在设置中搜索 `files.encoding`
- 设置为 `utf8`

## 验证配置

运行以下命令验证配置是否正确：

```powershell
# 检查编译器
g++ --version

# 检查CMake
cmake --version

# 检查Ninja
ninja --version

# 检查vcpkg
C:\vcpkg\vcpkg --version
```

## 总结

配置完成后，您的开发环境应该包含：

| 组件 | 路径/版本 |
|-----|----------|
| 编译器 | C:/msys64/mingw64/bin/g++.exe |
| 调试器 | C:/msys64/mingw64/bin/gdb.exe |
| CMake | 系统PATH |
| Ninja | 系统PATH |
| vcpkg | C:/vcpkg/ |
| SDL2 | C:/vcpkg/installed/x64-mingw-dynamic/ |
| Box2D | C:/vcpkg/installed/x64-mingw-dynamic/ |
| OpenAL | C:/vcpkg/installed/x64-mingw-dynamic/ |
