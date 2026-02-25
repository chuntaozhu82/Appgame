#include "fishing/platform/default/DefaultPlatform.h"
#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>

namespace FishingGame {

DefaultPlatform::DefaultPlatform()
    : m_deviceType(DeviceType::DESKTOP),
      m_inputType(InputType::MOUSE),
      m_scaleFactor(1.0f),
      m_uiScaleFactor(1.0f),
      m_fullscreen(false),
      m_platformName("Default"),
      m_platformVersion("1.0.0")
{
    // 初始化设备类型
    initDeviceType();
    
    // 初始化输入类型
    initInputType();
    
    // 初始化屏幕信息
    initScreenInfo();
}

DefaultPlatform::~DefaultPlatform()
{
    cleanup();
}

bool DefaultPlatform::init()
{
    // 初始化平台
    std::cout << "DefaultPlatform initialized" << std::endl;
    return true;
}

void DefaultPlatform::cleanup()
{
    // 清理平台资源
    std::cout << "DefaultPlatform cleaned up" << std::endl;
}

ScreenInfo DefaultPlatform::getScreenInfo()
{
    return m_screenInfo;
}

DeviceType DefaultPlatform::getDeviceType()
{
    return m_deviceType;
}

InputType DefaultPlatform::getInputType()
{
    return m_inputType;
}

bool DefaultPlatform::hasTouchSupport()
{
    return m_inputType == InputType::TOUCH || m_inputType == InputType::BOTH;
}

bool DefaultPlatform::hasMouseSupport()
{
    return m_inputType == InputType::MOUSE || m_inputType == InputType::BOTH;
}

float DefaultPlatform::getScaleFactor()
{
    return m_scaleFactor;
}

float DefaultPlatform::getUIScaleFactor()
{
    return m_uiScaleFactor;
}

bool DefaultPlatform::setFullscreen(bool fullscreen)
{
    m_fullscreen = fullscreen;
    std::cout << "Fullscreen set to: " << (fullscreen ? "true" : "false") << std::endl;
    return true;
}

bool DefaultPlatform::isFullscreen()
{
    return m_fullscreen;
}

std::string DefaultPlatform::getPlatformName()
{
    return m_platformName;
}

std::string DefaultPlatform::getPlatformVersion()
{
    return m_platformVersion;
}

bool DefaultPlatform::runMessageLoop()
{
    // 默认平台消息循环实现
    return true;
}

void DefaultPlatform::sleep(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

unsigned long DefaultPlatform::getTime()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void DefaultPlatform::initDeviceType()
{
    // 基于屏幕尺寸和DPI推断设备类型
    // 在实际实现中，应该根据具体平台API获取更准确的设备信息
    int32 width = m_screenInfo.width;
    int32 height = m_screenInfo.height;
    float32 dpi = m_screenInfo.dpi;
    
    // 计算屏幕对角线长度（英寸）
    float32 diagonalInches = std::sqrt(width * width + height * height) / dpi;
    
    // 根据屏幕尺寸和DPI推断设备类型
    if (diagonalInches < 6.0f) {
        m_deviceType = DeviceType::SMARTPHONE;
    } else if (diagonalInches < 12.0f) {
        m_deviceType = DeviceType::TABLET;
    } else if (diagonalInches < 17.0f) {
        m_deviceType = DeviceType::LAPTOP;
    } else {
        m_deviceType = DeviceType::DESKTOP;
    }
}

void DefaultPlatform::initInputType()
{
    // 初始化输入类型
    // 默认支持鼠标输入
    // 在实际实现中，应该根据具体平台API检测是否支持触摸
    m_inputType = InputType::MOUSE;
    
    // 模拟触摸支持检测
    // 在实际实现中，应该使用平台特定API检测
    bool hasTouch = false;
    // 这里可以添加平台特定的触摸检测代码
    
    if (hasTouch) {
        m_inputType = InputType::BOTH;
    }
}

void DefaultPlatform::initScreenInfo()
{
    // 初始化屏幕信息
    // 默认值，实际实现中应该根据具体平台API获取
    m_screenInfo.width = 1920;
    m_screenInfo.height = 1080;
    m_screenInfo.aspectRatio = static_cast<float>(m_screenInfo.width) / m_screenInfo.height;
    m_screenInfo.dpi = 96.0f;
    
    // 推断设备类型
    initDeviceType();
    
    // 推断输入类型
    initInputType();
    
    // 更新屏幕信息
    m_screenInfo.deviceType = m_deviceType;
    m_screenInfo.inputType = m_inputType;
    
    // 计算缩放因子
    // 基于屏幕尺寸、设备类型和DPI计算
    calculateScaleFactors();
}

void DefaultPlatform::calculateScaleFactors()
{
    // 基于屏幕尺寸、设备类型和DPI计算缩放因子
    if (m_deviceType == DeviceType::SMARTPHONE) {
        // 智能手机：较小的游戏世界缩放，较大的UI缩放
        m_scaleFactor = 0.5f;
        m_uiScaleFactor = 1.5f;
    } else if (m_deviceType == DeviceType::TABLET) {
        // 平板电脑：中等缩放
        m_scaleFactor = 0.75f;
        m_uiScaleFactor = 1.25f;
    } else if (m_deviceType == DeviceType::LAPTOP) {
        // 笔记本电脑：标准缩放
        m_scaleFactor = 1.0f;
        m_uiScaleFactor = 1.0f;
    } else {
        // 桌面设备：标准缩放
        m_scaleFactor = 1.0f;
        m_uiScaleFactor = 1.0f;
    }
    
    // 根据屏幕分辨率调整缩放因子
    int32 resolution = m_screenInfo.width * m_screenInfo.height;
    if (resolution > 3840 * 2160) { // 4K及以上
        m_scaleFactor *= 0.75f;
    } else if (resolution > 1920 * 1080) { // 2K
        m_scaleFactor *= 0.85f;
    }
    
    // 根据DPI调整UI缩放因子
    if (m_screenInfo.dpi > 150.0f) {
        m_uiScaleFactor *= 1.2f;
    } else if (m_screenInfo.dpi > 120.0f) {
        m_uiScaleFactor *= 1.1f;
    }
}

} // namespace FishingGame
