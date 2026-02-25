#ifndef DEFAULT_PLATFORM_H
#define DEFAULT_PLATFORM_H

#include "fishing/platform/Platform.h"

namespace FishingGame {

// 默认平台实现
class DefaultPlatform : public Platform {
public:
    DefaultPlatform();
    ~DefaultPlatform() override;

    // 初始化平台
    bool init() override;

    // 清理平台
    void cleanup() override;

    // 获取屏幕信息
    ScreenInfo getScreenInfo() override;

    // 获取设备类型
    DeviceType getDeviceType() override;

    // 获取输入类型
    InputType getInputType() override;

    // 检查是否支持触摸
    bool hasTouchSupport() override;

    // 检查是否支持鼠标
    bool hasMouseSupport() override;

    // 获取缩放因子
    float getScaleFactor() override;

    // 获取UI缩放因子
    float getUIScaleFactor() override;

    // 设置全屏模式
    bool setFullscreen(bool fullscreen) override;

    // 获取全屏状态
    bool isFullscreen() override;

    // 获取平台名称
    std::string getPlatformName() override;

    // 获取平台版本
    std::string getPlatformVersion() override;

    // 运行平台消息循环
    bool runMessageLoop() override;

    // 平台特定的睡眠
    void sleep(int milliseconds) override;

    // 平台特定的获取时间
    unsigned long getTime() override;

private:
    // 屏幕信息
    ScreenInfo m_screenInfo;

    // 设备类型
    DeviceType m_deviceType;

    // 输入类型
    InputType m_inputType;

    // 缩放因子
    float m_scaleFactor;

    // UI缩放因子
    float m_uiScaleFactor;

    // 全屏状态
    bool m_fullscreen;

    // 平台信息
    std::string m_platformName;
    std::string m_platformVersion;
protected:
    // 初始化设备类型
    void initDeviceType();

    // 初始化输入类型
    void initInputType();

    // 初始化屏幕信息
    void initScreenInfo();

    // 计算缩放因子
    void calculateScaleFactors();
};

} // namespace FishingGame

#endif // DEFAULT_PLATFORM_H
