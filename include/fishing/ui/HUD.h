#ifndef HUD_H
#define HUD_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include "fishing/ui/UIManager.h"
#include <string>
#include <vector>
#include <map>

namespace FishingGame {

// 前向声明
class UIManager;
class FishingSystem;
class WeatherSystem;
class TimeSystem;

// HUD类
class HUD {
public:
    HUD();
    ~HUD();

    // 初始化HUD
    bool init();

    // 清理HUD
    void cleanup();

    // 更新HUD
    void update(float32 deltaTime);

    // 渲染HUD
    void render();

    // 处理输入
    bool handleInput(int32 inputType, int32 inputValue, float32 x, float32 y);

    // 设置UI管理器
    void setUIManager(UIManager* uiManager);

    // 获取UI管理器
    UIManager* getUIManager() const;

    // 设置钓鱼系统
    void setFishingSystem(FishingSystem* fishingSystem);

    // 获取钓鱼系统
    FishingSystem* getFishingSystem() const;

    // 设置天气系统
    void setWeatherSystem(WeatherSystem* weatherSystem);

    // 获取天气系统
    WeatherSystem* getWeatherSystem() const;

    // 设置时间系统
    void setTimeSystem(TimeSystem* timeSystem);

    // 获取时间系统
    TimeSystem* getTimeSystem() const;

    // 设置玩家数据
    void setPlayerData(PlayerData* playerData);

    // 获取玩家数据
    PlayerData* getPlayerData() const;

    // 显示HUD
    void show();

    // 隐藏HUD
    void hide();

    // 检查HUD是否可见
    bool isVisible() const;

    // 设置HUD可见性
    void setVisible(bool visible);

    // 显示钓鱼状态
    void showFishingStatus();

    // 隐藏钓鱼状态
    void hideFishingStatus();

    // 显示玩家状态
    void showPlayerStatus();

    // 隐藏玩家状态
    void hidePlayerStatus();

    // 显示天气和时间
    void showWeatherAndTime();

    // 隐藏天气和时间
    void hideWeatherAndTime();

    // 显示消息
    void showMessage(const std::string& message, float32 duration = 3.0f);

    // 清除消息
    void clearMessage();

    // 获取当前消息
    const std::string& getCurrentMessage() const;

    // 检查是否有消息
    bool hasMessage() const;

    // 响应式调整大小
    void resize(int32 width, int32 height);

    // 获取HUD宽度
    int32 getWidth() const;

    // 获取HUD高度
    int32 getHeight() const;

    // 设置HUD大小
    void setSize(int32 width, int32 height);

    // 获取UI缩放因子
    float32 getUIScaleFactor() const;

    // 设置UI缩放因子
    void setUIScaleFactor(float32 scaleFactor);

    // 根据设备类型调整HUD
    void adjustForDeviceType(DeviceType deviceType);

    // 根据输入类型调整HUD
    void adjustForInputType(InputType inputType);

private:
    // UI管理器
    UIManager* m_uiManager;

    // 钓鱼系统
    FishingSystem* m_fishingSystem;

    // 天气系统
    WeatherSystem* m_weatherSystem;

    // 时间系统
    TimeSystem* m_timeSystem;

    // 玩家数据
    PlayerData* m_playerData;

    // HUD可见性
    bool m_visible;

    // HUD大小
    int32 m_width;
    int32 m_height;

    // UI缩放因子
    float32 m_uiScaleFactor;

    // 当前消息
    std::string m_currentMessage;

    // 消息持续时间
    float32 m_messageDuration;

    // 消息剩余时间
    float32 m_messageRemainingTime;

    // UI元素
    std::vector<UIElement*> m_uiElements;

    // 钓鱼状态UI元素
    UIElement* m_fishingStatusPanel;
    UIElement* m_fishingStateLabel;
    UIElement* m_reelingProgressBar;
    UIElement* m_lineTensionLabel;
    UIElement* m_fishInfoLabel;

    // 玩家状态UI元素
    UIElement* m_playerStatusPanel;
    UIElement* m_playerLevelLabel;
    UIElement* m_playerMoneyLabel;
    UIElement* m_playerExperienceLabel;
    UIElement* m_playerStatsLabel;

    // 天气和时间UI元素
    UIElement* m_weatherTimePanel;
    UIElement* m_weatherLabel;
    UIElement* m_timeLabel;

    // 消息UI元素
    UIElement* m_messagePanel;
    UIElement* m_messageLabel;

    // 初始化UI元素
    void initUIElements();

    // 初始化钓鱼状态UI
    void initFishingStatusUI();

    // 初始化玩家状态UI
    void initPlayerStatusUI();

    // 初始化天气和时间UI
    void initWeatherAndTimeUI();

    // 初始化消息UI
    void initMessageUI();

    // 更新钓鱼状态UI
    void updateFishingStatusUI();

    // 更新玩家状态UI
    void updatePlayerStatusUI();

    // 更新天气和时间UI
    void updateWeatherAndTimeUI();

    // 更新消息UI
    void updateMessageUI(float32 deltaTime);

    // 响应式调整UI元素大小
    void resizeUIElements(int32 width, int32 height);

    // 根据设备类型调整UI元素
    void adjustUIElementsForDeviceType(DeviceType deviceType);

    // 根据输入类型调整UI元素
    void adjustUIElementsForInputType(InputType inputType);

    // 计算UI元素位置
    void calculateUIElementPositions();

    // 获取钓鱼状态字符串
    std::string getFishingStateString(FishingState state) const;

    // 获取鱼的信息字符串
    std::string getFishInfoString() const;

    // 获取玩家状态字符串
    std::string getPlayerStatusString() const;

    // 获取天气信息字符串
    std::string getWeatherInfoString() const;

    // 获取时间信息字符串
    std::string getTimeInfoString() const;
};

} // namespace FishingGame

#endif // HUD_H
