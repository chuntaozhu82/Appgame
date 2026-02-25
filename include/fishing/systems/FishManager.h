#ifndef FISH_MANAGER_H
#define FISH_MANAGER_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace FishingGame {

// 前向声明
class FishType;

// 鱼实例类
class FishInstance {
public:
    FishInstance(FishTypeID typeId, const Vector2f& position);
    ~FishInstance();

    // 获取鱼的类型ID
    FishTypeID getTypeID() const;

    // 获取鱼的位置
    const Vector2f& getPosition() const;

    // 设置鱼的位置
    void setPosition(const Vector2f& position);

    // 获取鱼的速度
    const Vector2f& getVelocity() const;

    // 设置鱼的速度
    void setVelocity(const Vector2f& velocity);

    // 获取鱼的加速度
    const Vector2f& getAcceleration() const;

    // 设置鱼的加速度
    void setAcceleration(const Vector2f& acceleration);

    // 获取鱼的大小
    float32 getSize() const;

    // 获取鱼的重量
    float32 getWeight() const;

    // 获取鱼的力量
    float32 getStrength() const;

    // 获取鱼的状态
    bool isActive() const;

    // 设置鱼的状态
    void setActive(bool active);

    // 更新鱼的状态
    void update(float32 deltaTime);

    // 应用力
    void applyForce(const Vector2f& force);

    // 检查是否上钩
    bool isHooked() const;

    // 设置上钩状态
    void setHooked(bool hooked);

    // 获取上钩时间
    float32 getHookedTime() const;

    // 重置上钩时间
    void resetHookedTime();

    // 获取鱼的类型
    const FishType* getFishType() const;

private:
    // 鱼的类型ID
    FishTypeID m_typeId;

    // 鱼的位置
    Vector2f m_position;

    // 鱼的速度
    Vector2f m_velocity;

    // 鱼的加速度
    Vector2f m_acceleration;

    // 鱼的大小
    float32 m_size;

    // 鱼的重量
    float32 m_weight;

    // 鱼的力量
    float32 m_strength;

    // 鱼的状态
    bool m_active;

    // 是否上钩
    bool m_hooked;

    // 上钩时间
    float32 m_hookedTime;

    // 鱼的类型
    const FishType* m_fishType;

    // 初始化鱼的属性
    void initFishProperties();

    // 计算鱼的移动
    void calculateMovement(float32 deltaTime);

    // 应用物理规则
    void applyPhysics(float32 deltaTime);

    // 处理鱼的行为
    void handleBehavior(float32 deltaTime);
};

// 鱼管理器类
class FishManager {
public:
    FishManager();
    ~FishManager();

    // 初始化鱼管理器
    bool init();

    // 清理鱼管理器
    void cleanup();

    // 更新鱼管理器
    void update(float32 deltaTime);

    // 加载鱼的类型数据
    bool loadFishTypes(const std::string& filePath);

    // 获取鱼的类型
    const FishType* getFishType(FishTypeID typeId) const;

    // 获取所有鱼的类型
    const std::map<FishTypeID, FishType>& getFishTypes() const;

    // 生成鱼
    FishInstance* spawnFish(FishTypeID typeId, const Vector2f& position);

    // 生成随机鱼
    FishInstance* spawnRandomFish(const std::vector<FishTypeID>& availableFish, const Vector2f& position);

    // 移除鱼
    void removeFish(FishInstance* fish);

    // 清除所有鱼
    void clearFishes();

    // 获取当前所有鱼实例
    const std::vector<FishInstance*>& getFishes() const;

    // 获取指定区域内的鱼
    std::vector<FishInstance*> getFishesInArea(const Rectf& area) const;

    // 获取上钩的鱼
    FishInstance* getHookedFish() const;

    // 检查鱼是否在钓鱼点内
    bool isFishInFishingSpot(const FishInstance* fish, FishingSpotID spotId) const;

    // 设置当前钓鱼点
    void setCurrentFishingSpot(FishingSpotID spotId);

    // 获取当前钓鱼点
    FishingSpotID getCurrentFishingSpot() const;

    // 计算鱼的生成概率
    float32 calculateSpawnProbability(FishTypeID typeId, WeatherType weather, TimeType time) const;

    // 计算鱼的上钩概率
    float32 calculateCatchProbability(FishTypeID typeId, float32 castingSkill, float32 luck) const;

    // 获取鱼的价值
    int32 calculateFishValue(const FishInstance* fish) const;

    // 获取鱼的经验值
    float32 calculateFishExperience(const FishInstance* fish) const;

private:
    // 鱼的类型数据
    std::map<FishTypeID, FishType> m_fishTypes;

    // 当前鱼实例
    std::vector<FishInstance*> m_fishes;

    // 当前钓鱼点
    FishingSpotID m_currentFishingSpot;

    // 上钩的鱼
    FishInstance* m_hookedFish;

    // 加载鱼的类型数据
    bool loadFishTypesFromFile(const std::string& filePath);

    // 初始化默认鱼的类型
    void initDefaultFishTypes();

    // 检查鱼的类型是否存在
    bool hasFishType(FishTypeID typeId) const;

    // 生成随机鱼的大小
    float32 generateRandomSize(const FishType* fishType) const;

    // 生成随机鱼的重量
    float32 generateRandomWeight(const FishType* fishType, float32 size) const;

    // 生成随机鱼的力量
    float32 generateRandomStrength(const FishType* fishType, float32 size) const;
};

} // namespace FishingGame

#endif // FISH_MANAGER_H
