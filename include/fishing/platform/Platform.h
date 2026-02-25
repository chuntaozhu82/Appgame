#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>
#include <vector>
#include <functional>

namespace FishingGame {

// 设备类型
enum class DeviceType {
    DESKTOP,    // 桌面设备
    LAPTOP,     // 笔记本电脑
    TABLET,     // 平板电脑
    SMARTPHONE  // 智能手机
};

// 输入类型
enum class InputType {
    MOUSE,      // 鼠标输入
    TOUCH,      // 触摸输入
    BOTH        // 两种输入都支持
};

// 屏幕信息结构体
struct ScreenInfo {
    int width;          // 屏幕宽度
    int height;         // 屏幕高度
    float aspectRatio;  // 屏幕宽高比
    float dpi;          // 屏幕DPI
    DeviceType deviceType; // 设备类型
    InputType inputType;   // 输入类型
};

// 平台抽象接口
class Platform {
public:
    virtual ~Platform() = default;

    // 初始化平台
    virtual bool init() = 0;

    // 清理平台
    virtual void cleanup() = 0;

    // 获取屏幕信息
    virtual ScreenInfo getScreenInfo() = 0;

    // 获取设备类型
    virtual DeviceType getDeviceType() = 0;

    // 获取输入类型
    virtual InputType getInputType() = 0;

    // 检查是否支持触摸
    virtual bool hasTouchSupport() = 0;

    // 检查是否支持鼠标
    virtual bool hasMouseSupport() = 0;

    // 获取缩放因子
    virtual float getScaleFactor() = 0;

    // 获取UI缩放因子
    virtual float getUIScaleFactor() = 0;

    // 设置全屏模式
    virtual bool setFullscreen(bool fullscreen) = 0;

    // 获取全屏状态
    virtual bool isFullscreen() = 0;

    // 获取平台名称
    virtual std::string getPlatformName() = 0;

    // 获取平台版本
    virtual std::string getPlatformVersion() = 0;

    // 运行平台消息循环
    virtual bool runMessageLoop() = 0;

    // 平台特定的睡眠
    virtual void sleep(int milliseconds) = 0;

    // 平台特定的获取时间
    virtual unsigned long getTime() = 0;
};

// 平台工厂类
class PlatformFactory {
public:
    static Platform* createPlatform();
};

// 全局平台实例
extern Platform* g_platform;

} // namespace FishingGame

#endif // PLATFORM_H
