# 项目配置规则

## 构建命令

### 构建项目
```bash
cmake -B build -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

### 运行测试
```bash
cd build
ctest -C Release
```

### 运行游戏
```bash
./build/bin/Release/FishingGame.exe
```

## 依赖安装

### 安装vcpkg
```bash
git clone https://gitee.com/mirrors/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
```

### 安装依赖 (使用MinGW)
```bash
.\vcpkg install sdl2:x64-mingw-dynamic box2d:x64-mingw-dynamic openal-soft:x64-mingw-dynamic
```

### 安装依赖 (使用Visual Studio)
```bash
.\vcpkg install sdl2:x64-windows box2d:x64-windows openal-soft:x64-windows
```

## 代码风格

- 使用C++17标准
- 使用4空格缩进
- 类名使用PascalCase
- 函数名使用camelCase
- 常量使用UPPER_SNAKE_CASE
- 成员变量使用m_前缀

## 项目结构

```
Appgame/
├── include/          # 头文件
│   ├── core/        # 核心模块
│   └── fishing/     # 钓鱼游戏模块
├── src/             # 源文件
│   ├── core/
│   └── fishing/
├── external/        # 第三方库
├── build/           # 构建输出
└── CMakeLists.txt   # 构建配置
```

## 环境要求

- CMake 3.16+
- C++17编译器 (MSVC/GCC/Clang)
- vcpkg包管理器
- SDL2, Box2D, OpenAL库
