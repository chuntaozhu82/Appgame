#include "fishing/systems/FishingSystem.h"
#include "fishing/systems/FishManager.h"
#include "fishing/systems/PhysicsManager.h"
#include <iostream>

namespace FishingGame {

FishingSystem::FishingSystem()
    : m_fishingState(FishingState::IDLE),
      m_currentFishingSpot(0),
      m_currentFish(0),
      m_currentFishSize(0.0f),
      m_currentFishWeight(0.0f),
      m_reelingProgress(0.0f),
      m_fishStrength(0.0f),
      m_lineTension(0.0f),
      m_isFishing(false),
      m_fishManager(nullptr),
      m_physicsManager(nullptr),
      m_playerData(nullptr),
      m_castPower(0.0f),
      m_castAngle(0.0f),
      m_reelPower(0.0f),
      m_fishingTime(0.0f),
      m_hookTime(0.0f),
      m_maxFishingTime(60.0f),
      m_maxReelingTime(30.0f)
{
    // 初始化鱼管理器
    m_fishManager = std::make_unique<FishManager>();
    
    // 初始化物理管理器
    m_physicsManager = std::make_unique<PhysicsManager>();
}

FishingSystem::~FishingSystem() {
    cleanup();
}

bool FishingSystem::init() {
    // 初始化鱼管理器
    if (!m_fishManager->init()) {
        std::cerr << "Failed to initialize FishManager" << std::endl;
        return false;
    }
    
    // 初始化物理管理器
    if (!m_physicsManager->init()) {
        std::cerr << "Failed to initialize PhysicsManager" << std::endl;
        return false;
    }
    
    // 初始化钓鱼状态
    initFishingState();
    
    std::cout << "FishingSystem initialized successfully" << std::endl;
    return true;
}

void FishingSystem::cleanup() {
    // 停止钓鱼
    stopFishing();
    
    // 清理鱼管理器
    if (m_fishManager) {
        m_fishManager->cleanup();
    }
    
    // 清理物理管理器
    if (m_physicsManager) {
        m_physicsManager->cleanup();
    }
}

void FishingSystem::update(float32 deltaTime) {
    // 更新鱼管理器
    if (m_fishManager) {
        m_fishManager->update(deltaTime);
    }
    
    // 更新物理管理器
    if (m_physicsManager) {
        m_physicsManager->update(deltaTime);
    }
    
    // 处理钓鱼状态
    if (m_isFishing) {
        handleFishingState(deltaTime);
    }
}

bool FishingSystem::startFishing(FishingSpotID spotId) {
    if (m_isFishing) {
        std::cerr << "Already fishing" << std::endl;
        return false;
    }
    
    // 设置当前钓鱼点
    m_currentFishingSpot = spotId;
    
    // 设置鱼管理器的当前钓鱼点
    if (m_fishManager) {
        m_fishManager->setCurrentFishingSpot(spotId);
    }
    
    // 初始化钓鱼状态
    initFishingState();
    
    // 设置钓鱼标志
    m_isFishing = true;
    
    std::cout << "Started fishing at spot " << spotId << std::endl;
    return true;
}

void FishingSystem::stopFishing() {
    if (!m_isFishing) {
        return;
    }
    
    // 重置钓鱼状态
    initFishingState();
    
    // 清除钓鱼标志
    m_isFishing = false;
    
    std::cout << "Stopped fishing" << std::endl;
}

bool FishingSystem::castRod(float32 power, float32 angle) {
    if (m_fishingState != FishingState::IDLE && m_fishingState != FishingState::FAILED) {
        std::cerr << "Cannot cast rod in current state" << std::endl;
        return false;
    }
    
    // 验证参数
    if (power < 0.0f || power > 1.0f) {
        std::cerr << "Invalid cast power: " << power << std::endl;
        return false;
    }
    
    if (angle < 0.0f || angle > 180.0f) {
        std::cerr << "Invalid cast angle: " << angle << std::endl;
        return false;
    }
    
    // 设置投掷参数
    m_castPower = power;
    m_castAngle = angle;
    
    // 切换到投掷状态
    m_fishingState = FishingState::CASTING;
    
    // 重置钓鱼时间
    m_fishingTime = 0.0f;
    
    std::cout << "Cast rod with power " << power << " and angle " << angle << std::endl;
    return true;
}

bool FishingSystem::reelIn(float32 power) {
    if (m_fishingState != FishingState::WAITING && m_fishingState != FishingState::HOOKED && m_fishingState != FishingState::REELING) {
        std::cerr << "Cannot reel in in current state" << std::endl;
        return false;
    }
    
    // 验证参数
    if (power < 0.0f || power > 1.0f) {
        std::cerr << "Invalid reel power: " << power << std::endl;
        return false;
    }
    
    // 设置收杆参数
    m_reelPower = power;
    
    // 如果是等待状态，直接收杆
    if (m_fishingState == FishingState::WAITING) {
        m_fishingState = FishingState::FAILED;
        std::cout << "Reeled in without catching anything" << std::endl;
        return true;
    }
    
    // 如果是上钩状态，切换到收杆状态
    if (m_fishingState == FishingState::HOOKED) {
        m_fishingState = FishingState::REELING;
        std::cout << "Started reeling in" << std::endl;
    }
    
    return true;
}

void FishingSystem::abandonFishing() {
    if (m_isFishing) {
        stopFishing();
    }
}

FishingState FishingSystem::getFishingState() const {
    return m_fishingState;
}

FishingSpotID FishingSystem::getCurrentFishingSpot() const {
    return m_currentFishingSpot;
}

FishTypeID FishingSystem::getCurrentFish() const {
    return m_currentFish;
}

float32 FishingSystem::getCurrentFishSize() const {
    return m_currentFishSize;
}

float32 FishingSystem::getCurrentFishWeight() const {
    return m_currentFishWeight;
}

float32 FishingSystem::getReelingProgress() const {
    return m_reelingProgress;
}

float32 FishingSystem::getFishStrength() const {
    return m_fishStrength;
}

float32 FishingSystem::getLineTension() const {
    return m_lineTension;
}

bool FishingSystem::isFishing() const {
    return m_isFishing;
}

FishManager* FishingSystem::getFishManager() const {
    return m_fishManager.get();
}

PhysicsManager* FishingSystem::getPhysicsManager() const {
    return m_physicsManager.get();
}

void FishingSystem::setPlayerData(PlayerData* playerData) {
    m_playerData = playerData;
}

PlayerData* FishingSystem::getPlayerData() const {
    return m_playerData;
}

void FishingSystem::initFishingState() {
    m_fishingState = FishingState::IDLE;
    m_currentFish = 0;
    m_currentFishSize = 0.0f;
    m_currentFishWeight = 0.0f;
    m_reelingProgress = 0.0f;
    m_fishStrength = 0.0f;
    m_lineTension = 0.0f;
    m_castPower = 0.0f;
    m_castAngle = 0.0f;
    m_reelPower = 0.0f;
    m_fishingTime = 0.0f;
    m_hookTime = 0.0f;
}

void FishingSystem::handleFishingState(float32 deltaTime) {
    switch (m_fishingState) {
        case FishingState::CASTING:
            handleCastingState(deltaTime);
            break;
        case FishingState::WAITING:
            handleWaitingState(deltaTime);
            break;
        case FishingState::HOOKED:
            handleHookedState(deltaTime);
            break;
        case FishingState::REELING:
            handleReelingState(deltaTime);
            break;
        case FishingState::CAUGHT:
            handleCaughtState(deltaTime);
            break;
        case FishingState::FAILED:
            handleFailedState(deltaTime);
            break;
        case FishingState::IDLE:
        default:
            break;
    }
}

void FishingSystem::handleCastingState(float32 deltaTime) {
    // 模拟投掷过程
    m_fishingTime += deltaTime;
    
    // 投掷时间
    const float32 castDuration = 1.0f;
    
    if (m_fishingTime >= castDuration) {
        // 切换到等待状态
        m_fishingState = FishingState::WAITING;
        m_fishingTime = 0.0f;
        
        std::cout << "Cast completed, waiting for fish..." << std::endl;
    }
}

void FishingSystem::handleWaitingState(float32 deltaTime) {
    // 更新钓鱼时间
    m_fishingTime += deltaTime;
    
    // 检查是否超时
    if (m_fishingTime >= m_maxFishingTime) {
        // 切换到失败状态
        m_fishingState = FishingState::FAILED;
        std::cout << "Fishing timed out" << std::endl;
        return;
    }
    
    // 尝试生成鱼
    if (spawnFish()) {
        // 切换到上钩状态
        m_fishingState = FishingState::HOOKED;
        m_hookTime = 0.0f;
        
        std::cout << "Fish hooked!" << std::endl;
    }
}

void FishingSystem::handleHookedState(float32 deltaTime) {
    // 更新上钩时间
    m_hookTime += deltaTime;
    
    // 上钩等待时间
    const float32 hookWaitTime = 2.0f;
    
    if (m_hookTime >= hookWaitTime) {
        // 如果没有及时收杆，鱼会跑掉
        m_fishingState = FishingState::FAILED;
        std::cout << "Fish got away!" << std::endl;
    }
}

void FishingSystem::handleReelingState(float32 deltaTime) {
    // 更新收杆进度
    calculateReelingProgress(deltaTime);
    
    // 计算鱼线张力
    calculateLineTension();
    
    // 检查是否捕获成功
    if (checkCatchSuccess()) {
        m_fishingState = FishingState::CAUGHT;
        std::cout << "Fish caught!" << std::endl;
        handleCatchSuccess();
        return;
    }
    
    // 检查是否钓鱼失败
    if (checkCatchFailure()) {
        m_fishingState = FishingState::FAILED;
        std::cout << "Failed to catch fish" << std::endl;
        handleCatchFailure();
    }
}

void FishingSystem::handleCaughtState(float32 deltaTime) {
    // 捕获状态处理
    // 可以显示捕获信息，然后切换到 idle 状态
    const float32 caughtDisplayTime = 3.0f;
    m_fishingTime += deltaTime;
    
    if (m_fishingTime >= caughtDisplayTime) {
        m_fishingState = FishingState::IDLE;
        m_fishingTime = 0.0f;
    }
}

void FishingSystem::handleFailedState(float32 deltaTime) {
    // 失败状态处理
    // 可以显示失败信息，然后切换到 idle 状态
    const float32 failedDisplayTime = 2.0f;
    m_fishingTime += deltaTime;
    
    if (m_fishingTime >= failedDisplayTime) {
        m_fishingState = FishingState::IDLE;
        m_fishingTime = 0.0f;
    }
}

bool FishingSystem::spawnFish() {
    // 生成随机鱼
    if (m_fishManager) {
        // 模拟鱼的生成概率
        const float32 spawnChance = 0.05f; // 5% chance per update
        if (rand() / static_cast<float32>(RAND_MAX) < spawnChance) {
            // 生成随机鱼
            // 这里简化处理，实际应该根据钓鱼点和玩家等级等因素生成
            Vector2f position = {0.0f, 0.0f};
            auto fish = m_fishManager->spawnRandomFish({}, position);
            if (fish) {
                // 设置当前鱼信息
                m_currentFish = fish->getTypeID();
                m_currentFishSize = fish->getSize();
                m_currentFishWeight = fish->getWeight();
                m_fishStrength = fish->getStrength();
                return true;
            }
        }
    }
    return false;
}

void FishingSystem::calculateFishSizeAndWeight() {
    // 计算鱼的大小和重量
    // 这里简化处理，实际应该根据鱼的类型和随机因素计算
}

void FishingSystem::calculateReelingProgress(float32 deltaTime) {
    // 计算收杆进度
    // 基于收杆力量和鱼的力量
    const float32 reelSpeed = m_reelPower * 0.05f;
    const float32 fishResistance = m_fishStrength * 0.02f;
    
    m_reelingProgress += (reelSpeed - fishResistance) * deltaTime;
    
    // 限制进度范围
    if (m_reelingProgress < 0.0f) {
        m_reelingProgress = 0.0f;
    } else if (m_reelingProgress > 1.0f) {
        m_reelingProgress = 1.0f;
    }
}

void FishingSystem::calculateLineTension() {
    // 计算鱼线张力
    // 基于收杆力量和鱼的力量
    m_lineTension = std::abs(m_reelPower - m_fishStrength * 0.5f);
    
    // 限制张力范围
    if (m_lineTension < 0.0f) {
        m_lineTension = 0.0f;
    } else if (m_lineTension > 1.0f) {
        m_lineTension = 1.0f;
    }
}

bool FishingSystem::checkCatchSuccess() {
    // 检查是否捕获成功
    return m_reelingProgress >= 1.0f;
}

bool FishingSystem::checkCatchFailure() {
    // 检查是否钓鱼失败
    // 鱼线张力过大或收杆进度为0
    return m_lineTension >= 1.0f || m_reelingProgress <= 0.0f;
}

void FishingSystem::handleCatchSuccess() {
    // 处理捕获成功
    if (m_playerData) {
        // 增加玩家经验
        m_playerData->experience += 10.0f;
        
        // 增加玩家金钱
        m_playerData->money += 50;
        
        // 添加鱼到已捕获列表
        m_playerData->caughtFish.push_back(m_currentFish);
    }
}

void FishingSystem::handleCatchFailure() {
    // 处理钓鱼失败
    // 可以减少一些物品耐久度等
}

// 全局钓鱼系统实例
FishingSystem* g_fishingSystem = nullptr;

} // namespace FishingGame
