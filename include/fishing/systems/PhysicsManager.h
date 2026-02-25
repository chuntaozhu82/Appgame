#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "fishing/core/Types.h"
#include <string>
#include <vector>
#include <map>

namespace FishingGame {

// 前向声明
class FishInstance;

// 物理对象接口
class PhysicsObject {
public:
    virtual ~PhysicsObject() = default;

    // 获取对象ID
    virtual uint32 getID() const = 0;

    // 获取对象位置
    virtual const Vector2f& getPosition() const = 0;

    // 设置对象位置
    virtual void setPosition(const Vector2f& position) = 0;

    // 获取对象速度
    virtual const Vector2f& getVelocity() const = 0;

    // 设置对象速度
    virtual void setVelocity(const Vector2f& velocity) = 0;

    // 获取对象加速度
    virtual const Vector2f& getAcceleration() const = 0;

    // 设置对象加速度
    virtual void setAcceleration(const Vector2f& acceleration) = 0;

    // 获取对象质量
    virtual float32 getMass() const = 0;

    // 获取对象半径
    virtual float32 getRadius() const = 0;

    // 获取对象类型
    virtual CollisionType getCollisionType() const = 0;

    // 检查对象是否活跃
    virtual bool isActive() const = 0;

    // 应用力
    virtual void applyForce(const Vector2f& force) = 0;

    // 更新对象物理状态
    virtual void update(float32 deltaTime) = 0;

    // 处理碰撞
    virtual void handleCollision(PhysicsObject* other) = 0;
};

// 鱼物理对象
class FishPhysicsObject : public PhysicsObject {
public:
    FishPhysicsObject(FishInstance* fish);
    ~FishPhysicsObject() override;

    // 获取对象ID
    uint32 getID() const override;

    // 获取对象位置
    const Vector2f& getPosition() const override;

    // 设置对象位置
    void setPosition(const Vector2f& position) override;

    // 获取对象速度
    const Vector2f& getVelocity() const override;

    // 设置对象速度
    void setVelocity(const Vector2f& velocity) override;

    // 获取对象加速度
    const Vector2f& getAcceleration() const override;

    // 设置对象加速度
    void setAcceleration(const Vector2f& acceleration) override;

    // 获取对象质量
    float32 getMass() const override;

    // 获取对象半径
    float32 getRadius() const override;

    // 获取对象类型
    CollisionType getCollisionType() const override;

    // 检查对象是否活跃
    bool isActive() const override;

    // 应用力
    void applyForce(const Vector2f& force) override;

    // 更新对象物理状态
    void update(float32 deltaTime) override;

    // 处理碰撞
    void handleCollision(PhysicsObject* other) override;

    // 获取鱼实例
    FishInstance* getFishInstance() const;

private:
    // 鱼实例
    FishInstance* m_fish;

    // 对象ID
    uint32 m_id;

    // 质量
    float32 m_mass;

    // 半径
    float32 m_radius;
};

// 物理管理器类
class PhysicsManager {
public:
    PhysicsManager();
    ~PhysicsManager();

    // 初始化物理管理器
    bool init();

    // 清理物理管理器
    void cleanup();

    // 更新物理管理器
    void update(float32 deltaTime);

    // 添加物理对象
    void addPhysicsObject(PhysicsObject* object);

    // 移除物理对象
    void removePhysicsObject(PhysicsObject* object);

    // 清除所有物理对象
    void clearPhysicsObjects();

    // 获取所有物理对象
    const std::vector<PhysicsObject*>& getPhysicsObjects() const;

    // 获取指定类型的物理对象
    std::vector<PhysicsObject*> getPhysicsObjectsByType(CollisionType type) const;

    // 检查碰撞
    bool checkCollision(PhysicsObject* object1, PhysicsObject* object2) const;

    // 检测区域内的碰撞
    std::vector<PhysicsObject*> detectCollisionsInArea(const Rectf& area) const;

    // 应用重力
    void applyGravity(PhysicsObject* object);

    // 应用摩擦力
    void applyFriction(PhysicsObject* object);

    // 应用阻力
    void applyDrag(PhysicsObject* object);

    // 计算距离
    float32 calculateDistance(const Vector2f& pos1, const Vector2f& pos2) const;

    // 计算向量长度
    float32 calculateVectorLength(const Vector2f& vec) const;

    // 归一化向量
    void normalizeVector(Vector2f& vec) const;

    // 设置重力
    void setGravity(const Vector2f& gravity);

    // 获取重力
    const Vector2f& getGravity() const;

    // 设置摩擦力
    void setFriction(float32 friction);

    // 获取摩擦力
    float32 getFriction() const;

    // 设置阻力
    void setDrag(float32 drag);

    // 获取阻力
    float32 getDrag() const;

    // 设置时间步长
    void setTimeStep(float32 timeStep);

    // 获取时间步长
    float32 getTimeStep() const;

    // 设置迭代次数
    void setIterations(int32 iterations);

    // 获取迭代次数
    int32 getIterations() const;

private:
    // 物理对象列表
    std::vector<PhysicsObject*> m_physicsObjects;

    // 重力
    Vector2f m_gravity;

    // 摩擦力
    float32 m_friction;

    // 阻力
    float32 m_drag;

    // 时间步长
    float32 m_timeStep;

    // 迭代次数
    int32 m_iterations;

    // 碰撞检测
    void detectCollisions();

    // 解决碰撞
    void resolveCollisions();

    // 积分物理状态
    void integratePhysics(float32 deltaTime);

    // 应用物理规则
    void applyPhysicsRules(PhysicsObject* object, float32 deltaTime);
};

} // namespace FishingGame

#endif // PHYSICS_MANAGER_H
