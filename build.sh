#!/bin/bash

# 设置MSYS2环境
export PATH="/mingw64/bin:$PATH"

# 清理构建目录
rm -rf build
mkdir -p build

# 进入项目目录
cd "$(dirname "$0")"

# 运行CMake和构建
echo "正在配置项目..."
cmake -B build -G "Ninja"

if [ $? -eq 0 ]; then
    echo "配置成功，正在构建..."
    cmake --build build --config Release
    
    if [ $? -eq 0 ]; then
        echo "构建成功！"
        ls -la build/bin/
    else
        echo "构建失败！"
    fi
else
    echo "配置失败！"
fi
