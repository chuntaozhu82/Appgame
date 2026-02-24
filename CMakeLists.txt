cmake_minimum_required(VERSION 3.16)
project(AppgameFramework VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/SDL2/include
    ${CMAKE_SOURCE_DIR}/external/Box2D/include
    ${CMAKE_SOURCE_DIR}/external/OpenAL/include
)

# 链接目录
link_directories(
    ${CMAKE_SOURCE_DIR}/external/SDL2/lib
    ${CMAKE_SOURCE_DIR}/external/Box2D/lib
    ${CMAKE_SOURCE_DIR}/external/OpenAL/lib
)

# 平台特定设置
if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(APPLE)
    add_definitions(-DPLATFORM_IOS)
elseif(ANDROID)
    add_definitions(-DPLATFORM_ANDROID)
endif()

# 核心引擎源文件
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
file(GLOB_RECURSE PLATFORM_SOURCES "src/platform/*.cpp")
file(GLOB_RECURSE UTILS_SOURCES "src/utils/*.cpp")

# 创建核心引擎库
add_library(AppgameCore STATIC
    ${CORE_SOURCES}
    ${PLATFORM_SOURCES}
    ${UTILS_SOURCES}
)

# 链接依赖
target_link_libraries(AppgameCore PRIVATE
    SDL2
    Box2D
    OpenAL
)

# 示例项目
add_subdirectory(examples)
