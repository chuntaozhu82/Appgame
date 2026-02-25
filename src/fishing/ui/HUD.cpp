#include "fishing/ui/HUD.h"
#include "fishing/ui/UIManager.h"
#include "fishing/systems/FishingSystem.h"
#include "fishing/systems/WeatherSystem.h"
#include "fishing/systems/TimeSystem.h"
#include <iostream>

namespace FishingGame {

HUD::HUD()
    : m_uiManager(nullptr),
      m_fishingSystem(nullptr),
      m_weatherSystem(nullptr),
      m_timeSystem(nullptr),
      m_playerData(nullptr),
      m_visible(true),
      m_width(1920),
      m_height(1080),
      m_uiScaleFactor(1.0f),
      m_messageDuration(3.0f),
      m_messageRemainingTime(0.0f),
      m_fishingStatusPanel(nullptr),
      m_fishingStateLabel(nullptr),
      m_reelingProgressBar(nullptr),
      m_lineTensionLabel(nullptr),
      m_fishInfoLabel(nullptr),
      m_playerStatusPanel(nullptr),
      m_playerLevelLabel(nullptr),
      m_playerMoneyLabel(nullptr),
      m_playerExperienceLabel(nullptr),
      m_playerStatsLabel(nullptr),
      m_weatherTimePanel(nullptr),
      m_weatherLabel(nullptr),
      m_timeLabel(nullptr),
      m_messagePanel(nullptr),
      m_messageLabel(nullptr)
{
}

HUD::~HUD() {
    cleanup();
}

bool HUD::init() {
    // 初始化UI元素
    initUIElements();
    
    std::cout << "HUD initialized" << std::endl;
    return true;
}

void HUD::cleanup() {
    // 清理UI元素
    for (auto& element : m_uiElements) {
        element->cleanup();
        delete element;
    }
    m_uiElements.clear();
    
    // 重置指针
    m_fishingStatusPanel = nullptr;
    m_fishingStateLabel = nullptr;
    m_reelingProgressBar = nullptr;
    m_lineTensionLabel = nullptr;
    m_fishInfoLabel = nullptr;
    m_playerStatusPanel = nullptr;
    m_playerLevelLabel = nullptr;
    m_playerMoneyLabel = nullptr;
    m_playerExperienceLabel = nullptr;
    m_playerStatsLabel = nullptr;
    m_weatherTimePanel = nullptr;
    m_weatherLabel = nullptr;
    m_timeLabel = nullptr;
    m_messagePanel = nullptr;
    m_messageLabel = nullptr;
    
    std::cout << "HUD cleaned up" << std::endl;
}

void HUD::update(float32 deltaTime) {
    if (!m_visible) {
        return;
    }
    
    // 更新钓鱼状态UI
    updateFishingStatusUI();
    
    // 更新玩家状态UI
    updatePlayerStatusUI();
    
    // 更新天气和时间UI
    updateWeatherAndTimeUI();
    
    // 更新消息UI
    updateMessageUI(deltaTime);
    
    // 更新UI元素
    for (auto& element : m_uiElements) {
        element->update(deltaTime);
    }
}

void HUD::render() {
    if (!m_visible) {
        return;
    }
    
    // 渲染UI元素
    for (auto& element : m_uiElements) {
        element->render();
    }
}

bool HUD::handleInput(int32 inputType, int32 inputValue, float32 x, float32 y) {
    if (!m_visible) {
        return false;
    }
    
    // 处理UI元素输入
    for (auto& element : m_uiElements) {
        if (element->handleInput(inputType, inputValue, x, y)) {
            return true;
        }
    }
    
    return false;
}

void HUD::setUIManager(UIManager* uiManager) {
    m_uiManager = uiManager;
}

UIManager* HUD::getUIManager() const {
    return m_uiManager;
}

void HUD::setFishingSystem(FishingSystem* fishingSystem) {
    m_fishingSystem = fishingSystem;
}

FishingSystem* HUD::getFishingSystem() const {
    return m_fishingSystem;
}

void HUD::setWeatherSystem(WeatherSystem* weatherSystem) {
    m_weatherSystem = weatherSystem;
}

WeatherSystem* HUD::getWeatherSystem() const {
    return m_weatherSystem;
}

void HUD::setTimeSystem(TimeSystem* timeSystem) {
    m_timeSystem = timeSystem;
}

TimeSystem* HUD::getTimeSystem() const {
    return m_timeSystem;
}

void HUD::setPlayerData(PlayerData* playerData) {
    m_playerData = playerData;
}

PlayerData* HUD::getPlayerData() const {
    return m_playerData;
}

void HUD::show() {
    m_visible = true;
}

void HUD::hide() {
    m_visible = false;
}

bool HUD::isVisible() const {
    return m_visible;
}

void HUD::setVisible(bool visible) {
    m_visible = visible;
}

void HUD::showFishingStatus() {
    if (m_fishingStatusPanel) {
        m_fishingStatusPanel->setVisible(true);
    }
}

void HUD::hideFishingStatus() {
    if (m_fishingStatusPanel) {
        m_fishingStatusPanel->setVisible(false);
    }
}

void HUD::showPlayerStatus() {
    if (m_playerStatusPanel) {
        m_playerStatusPanel->setVisible(true);
    }
}

void HUD::hidePlayerStatus() {
    if (m_playerStatusPanel) {
        m_playerStatusPanel->setVisible(false);
    }
}

void HUD::showWeatherAndTime() {
    if (m_weatherTimePanel) {
        m_weatherTimePanel->setVisible(true);
    }
}

void HUD::hideWeatherAndTime() {
    if (m_weatherTimePanel) {
        m_weatherTimePanel->setVisible(false);
    }
}

void HUD::showMessage(const std::string& message, float32 duration) {
    m_currentMessage = message;
    m_messageDuration = duration;
    m_messageRemainingTime = duration;
    
    if (m_messagePanel) {
        m_messagePanel->setVisible(true);
    }
    if (m_messageLabel) {
        // TODO: 更新消息标签文本
    }
}

void HUD::clearMessage() {
    m_currentMessage.clear();
    m_messageRemainingTime = 0.0f;
    
    if (m_messagePanel) {
        m_messagePanel->setVisible(false);
    }
}

const std::string& HUD::getCurrentMessage() const {
    return m_currentMessage;
}

bool HUD::hasMessage() const {
    return !m_currentMessage.empty() && m_messageRemainingTime > 0.0f;
}

void HUD::resize(int32 width, int32 height) {
    // 设置新大小
    m_width = width;
    m_height = height;
    
    // 调整UI元素大小
    resizeUIElements(width, height);
    
    // 重新计算UI元素位置
    calculateUIElementPositions();
}

int32 HUD::getWidth() const {
    return m_width;
}

int32 HUD::getHeight() const {
    return m_height;
}

void HUD::setSize(int32 width, int32 height) {
    m_width = width;
    m_height = height;
}

float32 HUD::getUIScaleFactor() const {
    return m_uiScaleFactor;
}

void HUD::setUIScaleFactor(float32 scaleFactor) {
    m_uiScaleFactor = scaleFactor;
    
    // 调整UI元素缩放
    for (auto& element : m_uiElements) {
        element->setScale(scaleFactor);
    }
}

void HUD::adjustForDeviceType(DeviceType deviceType) {
    // 根据设备类型调整UI元素
    adjustUIElementsForDeviceType(deviceType);
}

void HUD::adjustForInputType(InputType inputType) {
    // 根据输入类型调整UI元素
    adjustUIElementsForInputType(inputType);
}

void HUD::initUIElements() {
    // 初始化钓鱼状态UI
    initFishingStatusUI();
    
    // 初始化玩家状态UI
    initPlayerStatusUI();
    
    // 初始化天气和时间UI
    initWeatherAndTimeUI();
    
    // 初始化消息UI
    initMessageUI();
    
    // 计算UI元素位置
    calculateUIElementPositions();
}

void HUD::initFishingStatusUI() {
    if (!m_uiManager) {
        return;
    }
    
    // 创建钓鱼状态面板
    m_fishingStatusPanel = m_uiManager->createPanel("fishingStatusPanel", 10, 10, 300, 200);
    m_uiElements.push_back(m_fishingStatusPanel);
    
    // 创建钓鱼状态标签
    m_fishingStateLabel = m_uiManager->createLabel("fishingStateLabel", "钓鱼状态: 准备", 20, 30, 16);
    m_uiElements.push_back(m_fishingStateLabel);
    
    // 创建收杆进度条
    m_reelingProgressBar = m_uiManager->createProgressBar("reelingProgressBar", 20, 60, 260, 20, 0.0f);
    m_uiElements.push_back(m_reelingProgressBar);
    
    // 创建鱼线张力标签
    m_lineTensionLabel = m_uiManager->createLabel("lineTensionLabel", "鱼线张力: 0%", 20, 90, 14);
    m_uiElements.push_back(m_lineTensionLabel);
    
    // 创建鱼信息标签
    m_fishInfoLabel = m_uiManager->createLabel("fishInfoLabel", "鱼: 未知", 20, 120, 14);
    m_uiElements.push_back(m_fishInfoLabel);
}

void HUD::initPlayerStatusUI() {
    if (!m_uiManager) {
        return;
    }
    
    // 创建玩家状态面板
    m_playerStatusPanel = m_uiManager->createPanel("playerStatusPanel", 10, m_height - 110, 300, 100);
    m_uiElements.push_back(m_playerStatusPanel);
    
    // 创建玩家等级标签
    m_playerLevelLabel = m_uiManager->createLabel("playerLevelLabel", "等级: 1", 20, m_height - 90, 16);
    m_uiElements.push_back(m_playerLevelLabel);
    
    // 创建玩家金钱标签
    m_playerMoneyLabel = m_uiManager->createLabel("playerMoneyLabel", "金钱: 0", 20, m_height - 65, 14);
    m_uiElements.push_back(m_playerMoneyLabel);
    
    // 创建玩家经验标签
    m_playerExperienceLabel = m_uiManager->createLabel("playerExperienceLabel", "经验: 0/100", 150, m_height - 90, 14);
    m_uiElements.push_back(m_playerExperienceLabel);
    
    // 创建玩家状态标签
    m_playerStatsLabel = m_uiManager->createLabel("playerStatsLabel", "状态: 正常", 150, m_height - 65, 14);
    m_uiElements.push_back(m_playerStatsLabel);
}

void HUD::initWeatherAndTimeUI() {
    if (!m_uiManager) {
        return;
    }
    
    // 创建天气和时间面板
    m_weatherTimePanel = m_uiManager->createPanel("weatherTimePanel", m_width - 210, 10, 200, 80);
    m_uiElements.push_back(m_weatherTimePanel);
    
    // 创建天气标签
    m_weatherLabel = m_uiManager->createLabel("weatherLabel", "天气: 晴天", m_width - 200, 30, 14);
    m_uiElements.push_back(m_weatherLabel);
    
    // 创建时间标签
    m_timeLabel = m_uiManager->createLabel("timeLabel", "时间: 08:00", m_width - 200, 60, 14);
    m_uiElements.push_back(m_timeLabel);
}

void HUD::initMessageUI() {
    if (!m_uiManager) {
        return;
    }
    
    // 创建消息面板
    m_messagePanel = m_uiManager->createPanel("messagePanel", m_width / 2 - 200, m_height / 2 - 50, 400, 100);
    m_uiElements.push_back(m_messagePanel);
    m_messagePanel->setVisible(false);
    
    // 创建消息标签
    m_messageLabel = m_uiManager->createLabel("messageLabel", "", m_width / 2 - 190, m_height / 2 - 30, 18);
    m_uiElements.push_back(m_messageLabel);
    m_messageLabel->setVisible(false);
}

void HUD::updateFishingStatusUI() {
    if (!m_fishingSystem || !m_fishingStateLabel || !m_reelingProgressBar || !m_lineTensionLabel || !m_fishInfoLabel) {
        return;
    }
    
    // 更新钓鱼状态标签
    FishingState state = m_fishingSystem->getState();
    std::string stateString = getFishingStateString(state);
    // TODO: 更新钓鱼状态标签文本
    
    // 更新收杆进度条
    float32 reelingProgress = m_fishingSystem->getReelingProgress();
    // TODO: 更新收杆进度条值
    
    // 更新鱼线张力标签
    float32 lineTension = m_fishingSystem->getLineTension();
    // TODO: 更新鱼线张力标签文本
    
    // 更新鱼信息标签
    if (m_fishingSystem->hasCaughtFish()) {
        std::string fishInfo = getFishInfoString();
        // TODO: 更新鱼信息标签文本
    }
}

void HUD::updatePlayerStatusUI() {
    if (!m_playerData || !m_playerLevelLabel || !m_playerMoneyLabel || !m_playerExperienceLabel || !m_playerStatsLabel) {
        return;
    }
    
    // 更新玩家等级标签
    // TODO: 更新玩家等级标签文本
    
    // 更新玩家金钱标签
    // TODO: 更新玩家金钱标签文本
    
    // 更新玩家经验标签
    // TODO: 更新玩家经验标签文本
    
    // 更新玩家状态标签
    // TODO: 更新玩家状态标签文本
}

void HUD::updateWeatherAndTimeUI() {
    if (!m_weatherSystem || !m_timeSystem || !m_weatherLabel || !m_timeLabel) {
        return;
    }
    
    // 更新天气标签
    std::string weatherInfo = getWeatherInfoString();
    // TODO: 更新天气标签文本
    
    // 更新时间标签
    std::string timeInfo = getTimeInfoString();
    // TODO: 更新时间标签文本
}

void HUD::updateMessageUI(float32 deltaTime) {
    if (m_messageRemainingTime > 0.0f) {
        m_messageRemainingTime -= deltaTime;
        if (m_messageRemainingTime <= 0.0f) {
            clearMessage();
        }
    }
}

void HUD::resizeUIElements(int32 width, int32 height) {
    // 调整钓鱼状态面板
    if (m_fishingStatusPanel) {
        m_fishingStatusPanel->setPosition(10, 10);
    }
    
    // 调整钓鱼状态标签
    if (m_fishingStateLabel) {
        m_fishingStateLabel->setPosition(20, 30);
    }
    
    // 调整收杆进度条
    if (m_reelingProgressBar) {
        m_reelingProgressBar->setPosition(20, 60);
    }
    
    // 调整鱼线张力标签
    if (m_lineTensionLabel) {
        m_lineTensionLabel->setPosition(20, 90);
    }
    
    // 调整鱼信息标签
    if (m_fishInfoLabel) {
        m_fishInfoLabel->setPosition(20, 120);
    }
    
    // 调整玩家状态面板
    if (m_playerStatusPanel) {
        m_playerStatusPanel->setPosition(10, height - 110);
    }
    
    // 调整玩家等级标签
    if (m_playerLevelLabel) {
        m_playerLevelLabel->setPosition(20, height - 90);
    }
    
    // 调整玩家金钱标签
    if (m_playerMoneyLabel) {
        m_playerMoneyLabel->setPosition(20, height - 65);
    }
    
    // 调整玩家经验标签
    if (m_playerExperienceLabel) {
        m_playerExperienceLabel->setPosition(150, height - 90);
    }
    
    // 调整玩家状态标签
    if (m_playerStatsLabel) {
        m_playerStatsLabel->setPosition(150, height - 65);
    }
    
    // 调整天气和时间面板
    if (m_weatherTimePanel) {
        m_weatherTimePanel->setPosition(width - 210, 10);
    }
    
    // 调整天气标签
    if (m_weatherLabel) {
        m_weatherLabel->setPosition(width - 200, 30);
    }
    
    // 调整时间标签
    if (m_timeLabel) {
        m_timeLabel->setPosition(width - 200, 60);
    }
    
    // 调整消息面板
    if (m_messagePanel) {
        m_messagePanel->setPosition(width / 2 - 200, height / 2 - 50);
    }
    
    // 调整消息标签
    if (m_messageLabel) {
        m_messageLabel->setPosition(width / 2 - 190, height / 2 - 30);
    }
}

void HUD::adjustUIElementsForDeviceType(DeviceType deviceType) {
    // 根据设备类型调整UI元素大小和位置
    switch (deviceType) {
        case DeviceType::SMARTPHONE:
            // 智能手机：增大UI元素，简化布局
            for (auto& element : m_uiElements) {
                element->setScale(m_uiScaleFactor * 1.5f);
            }
            break;
        case DeviceType::TABLET:
            // 平板电脑：中等UI元素大小
            for (auto& element : m_uiElements) {
                element->setScale(m_uiScaleFactor * 1.25f);
            }
            break;
        case DeviceType::LAPTOP:
        case DeviceType::DESKTOP:
        default:
            // 桌面设备：标准UI元素大小
            for (auto& element : m_uiElements) {
                element->setScale(m_uiScaleFactor);
            }
            break;
    }
}

void HUD::adjustUIElementsForInputType(InputType inputType) {
    // 根据输入类型调整UI元素
    switch (inputType) {
        case InputType::TOUCH:
            // 触摸输入：增大点击区域
            // TODO: 实现触摸输入调整
            break;
        case InputType::MOUSE:
        case InputType::BOTH:
        default:
            // 鼠标输入：标准点击区域
            // TODO: 实现鼠标输入调整
            break;
    }
}

void HUD::calculateUIElementPositions() {
    // 计算UI元素位置
    resizeUIElements(m_width, m_height);
}

std::string HUD::getFishingStateString(FishingState state) const {
    switch (state) {
        case FishingState::IDLE:
            return "准备";
        case FishingState::CASTING:
            return "抛竿";
        case FishingState::WAITING:
            return "等待";
        case FishingState::HOOKING:
            return "上钩";
        case FishingState::REELING:
            return "收杆";
        case FishingState::CAUGHT:
            return "捕获";
        case FishingState::FAILED:
            return "失败";
        default:
            return "未知";
    }
}

std::string HUD::getFishInfoString() const {
    if (!m_fishingSystem || !m_fishingSystem->hasCaughtFish()) {
        return "鱼: 未知";
    }
    
    // TODO: 实现鱼信息字符串获取
    return "鱼: 未知";
}

std::string HUD::getPlayerStatusString() const {
    if (!m_playerData) {
        return "状态: 正常";
    }
    
    // TODO: 实现玩家状态字符串获取
    return "状态: 正常";
}

std::string HUD::getWeatherInfoString() const {
    if (!m_weatherSystem) {
        return "天气: 晴天";
    }
    
    // TODO: 实现天气信息字符串获取
    return "天气: 晴天";
}

std::string HUD::getTimeInfoString() const {
    if (!m_timeSystem) {
        return "时间: 08:00";
    }
    
    // TODO: 实现时间信息字符串获取
    return "时间: 08:00";
}

} // namespace FishingGame
