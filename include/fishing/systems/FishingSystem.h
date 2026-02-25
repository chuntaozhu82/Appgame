#ifndef FISHING_SYSTEM_H
#define FISHING_SYSTEM_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include <string>
#include <vector>
#include <memory>

namespace FishingGame {

// 前向声明
class FishManager;
class PhysicsManager;
class FishType;

// 钓鱼系统类
class FishingSystem {
public:
    FishingSystem();
    ~FishingSystem();

    // 初始化钓鱼系统
    bool init();

    // 清理钓鱼系统
    void cleanup();

    // 更新钓鱼系统
    void update(float32 deltaTime);

    // 开始钓鱼
    bool startFishing(FishingSpotID spotId);

    // 结束钓鱼
    void stopFishing();

    // 投掷鱼竿
    bool castRod(float32 power, float32 angle);

    // 收杆
    bool reelIn(float32 power);

    // 放弃钓鱼
    void abandonFishing();

    // 获取当前钓鱼状态
    FishingState getFishingState() const;

    // 获取当前钓鱼点
    FishingSpotID getCurrentFishingSpot() const;

    // 获取当前鱼
    FishTypeID getCurrentFish() const;

    // 获取当前鱼的大小
    float32 getCurrentFishSize() const;

    // 获取当前鱼的重量
    float32 getCurrentFishWeight() const;

    // 获取收杆进度
    float32 getReelingProgress() const;

    // 获取鱼的力量
    float32 getFishStrength() const;

    // 获取鱼线张力
    float32 getLineTension() const;

    // 检查是否正在钓鱼
    bool isFishing() const;

    // 获取鱼管理器
    FishManager* getFishManager() const;

    // 获取物理管理器
    PhysicsManager* getPhysicsManager() const;

    // 设置玩家数据
    void setPlayerData(PlayerData* playerData);

    // 获取玩家数据
    PlayerData* getPlayerData() const;

private:
    // 钓鱼状态
    FishingState m_fishingState;

    // 当前钓鱼点
    FishingSpotID m_currentFishingSpot;

    // 当前鱼
    FishTypeID m_currentFish;

    // 当前鱼的大小
    float32 m_currentFishSize;

    // 当前鱼的重量
    float32 m_currentFishWeight;

    // 收杆进度
    float32 m_reelingProgress;

    // 鱼的力量
    float32 m_fishStrength;

    // 鱼线张力
    float32 m_lineTension;

    // 是否正在钓鱼
    bool m_isFishing;

    // 鱼管理器
    std::unique_ptr<FishManager> m_fishManager;

    // 物理管理器
    std::unique_ptr<PhysicsManager> m_physicsManager;

    // 玩家数据
    PlayerData* m_playerData;

    // 投掷参数
    float32 m_castPower;
    float32 m_castAngle;

    // 收杆参数
    float32 m_reelPower;

    // 钓鱼时间
    float32 m_fishingTime;

    // 鱼上钩时间
    float32 m_hookTime;

    // 最大钓鱼时间
    float32 m_maxFishingTime;

    // 最大收杆时间
    float32 m_maxReelingTime;

    // 初始化钓鱼状态
    void initFishingState();

    // 处理钓鱼状态
    void handleFishingState(float32 deltaTime);

    // 处理投掷状态
    void handleCastingState(float32 deltaTime);

    // 处理等待状态
    void handleWaitingState(float32 deltaTime);

    // 处理上钩状态
    void handleHookedState(float32 deltaTime);

    // 处理收杆状态
    void handleReelingState(float32 deltaTime);

    // 处理捕获状态
    void handleCaughtState(float32 deltaTime);

    // 处理失败状态
    void handleFailedState(float32 deltaTime);

    // 生成鱼
    bool spawnFish();

    // 计算鱼的大小和重量
    void calculateFishSizeAndWeight();

    // 计算收杆进度
    void calculateReelingProgress(float32 deltaTime);

    // 计算鱼线张力
    void calculateLineTension();

    // 检查是否捕获成功
    bool checkCatchSuccess();

    // 检查是否钓鱼失败
    bool checkCatchFailure();

    // 处理捕获成功
    void handleCatchSuccess();

    // 处理钓鱼失败
    void handleCatchFailure();
};

// 全局钓鱼系统实例
extern FishingSystem* g_fishingSystem;

} // namespace FishingGame

#endif // FISHING_SYSTEM_H
