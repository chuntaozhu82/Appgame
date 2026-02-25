cmake_minimum_required(VERSION 3.16)
project(AppgameFramework VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 检测MSYS2/MinGW环境
if(EXISTS "C:/msys64/mingw64")
    set(MSYS2_MINGW64_PATH "C:/msys64/mingw64")
    message(STATUS "Found MSYS2 MinGW64 at ${MSYS2_MINGW64_PATH}")
endif()

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# 添加MSYS2/MinGW包含目录
if(DEFINED MSYS2_MINGW64_PATH)
    include_directories(
        ${MSYS2_MINGW64_PATH}/include
        ${MSYS2_MINGW64_PATH}/include/SDL2
    )
    link_directories(
        ${MSYS2_MINGW64_PATH}/lib
    )
else()
    include_directories(
        ${CMAKE_SOURCE_DIR}/external/SDL2/include
        ${CMAKE_SOURCE_DIR}/external/Box2D/include
        ${CMAKE_SOURCE_DIR}/external/OpenAL/include
    )
    link_directories(
        ${CMAKE_SOURCE_DIR}/external/SDL2/lib
        ${CMAKE_SOURCE_DIR}/external/Box2D/lib
        ${CMAKE_SOURCE_DIR}/external/OpenAL/lib
    )
endif()

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

# 创建核心引擎库
add_library(AppgameCore STATIC
    ${CORE_SOURCES}
)

# 链接依赖
if(DEFINED MSYS2_MINGW64_PATH)
    target_link_libraries(AppgameCore PRIVATE
        SDL2
        SDL2main
        box2d
        openal
        mingw32
    )
else()
    target_link_libraries(AppgameCore PRIVATE
        SDL2
        SDL2main
        Box2D
        OpenAL
    )
endif()

# 示例项目
add_subdirectory(examples)

# 钓鱼游戏项目
file(GLOB_RECURSE FISHING_SOURCES "src/fishing/**/*.cpp")

# 创建钓鱼游戏可执行文件
add_executable(FishingGame
    ${FISHING_SOURCES}
)

# 包含目录
target_include_directories(FishingGame PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/fishing
)

# 链接依赖
target_link_libraries(FishingGame PRIVATE
    AppgameCore
)

# 复制资源文件
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    add_custom_command(TARGET FishingGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    )
endif()

# 复制DLL文件
if(DEFINED MSYS2_MINGW64_PATH)
    add_custom_command(TARGET FishingGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MSYS2_MINGW64_PATH}/bin/SDL2.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
    add_custom_command(TARGET FishingGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MSYS2_MINGW64_PATH}/bin/OpenAL32.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()

# 平台特定设置
if(WIN32)
    # Windows 特定设置
    set_target_properties(FishingGame PROPERTIES
        WIN32_EXECUTABLE ON
    )
elseif(APPLE)
    # iOS 特定设置
    set_target_properties(FishingGame PROPERTIES
        MACOSX_BUNDLE ON
    )
elseif(ANDROID)
    # Android 特定设置
    # 使用 Android Gradle 插件处理
endif()

# 测试项目
file(GLOB_RECURSE TEST_SOURCES "src/fishing/test/*.cpp")

# 排除main.cpp，因为测试有自己的main函数
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# 创建测试可执行文件
add_executable(FishingGameTests
    ${TEST_SOURCES}
    src/fishing/test/main.cpp
)

# 包含目录
target_include_directories(FishingGameTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/fishing
)

# 链接依赖
target_link_libraries(FishingGameTests PRIVATE
    AppgameCore
)

# 启用测试
enable_testing()
add_test(NAME FishingGameTests COMMAND FishingGameTests)
