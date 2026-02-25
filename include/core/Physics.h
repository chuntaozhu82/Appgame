#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>
#include <vector>
#include <functional>
#include <Box2D/Box2D.h>

namespace Appgame {

// 物理世界配置
struct PhysicsWorldConfig {
    float gravityX;     // 重力X分量
    float gravityY;     // 重力Y分量
    bool sleepEnabled;  // 是否启用睡眠
    float timeStep;     // 时间步长
    int velocityIterations;  // 速度迭代次数
    int positionIterations;  // 位置迭代次数

    PhysicsWorldConfig()
        : gravityX(0.0f), gravityY(-9.8f), sleepEnabled(true)
        , timeStep(1.0f / 60.0f), velocityIterations(8), positionIterations(3) {}
};

// 刚体类型
enum class BodyType {
    STATIC,     // 静态刚体
    DYNAMIC,    // 动态刚体
    KINEMATIC   // 运动学刚体
};

// 碰撞形状类型
enum class ShapeType {
    CIRCLE,     // 圆形
    BOX,        // 矩形
    POLYGON,    // 多边形
    CHAIN       // 链形
};

// 碰撞信息
struct ContactInfo {
    void* bodyA;        // 碰撞体A
    void* bodyB;        // 碰撞体B
    float normalX;      // 碰撞法线X分量
    float normalY;      // 碰撞法线Y分量
    float impulse;      // 碰撞冲量
};

// 碰撞回调接口
class ContactListener {
public:
    virtual ~ContactListener() = default;

    // 碰撞开始
    virtual void onContactBegin(const ContactInfo& info) = 0;

    // 碰撞持续
    virtual void onContactPersist(const ContactInfo& info) = 0;

    // 碰撞结束
    virtual void onContactEnd(const ContactInfo& info) = 0;
};

// 刚体定义
struct BodyDef {
    BodyType type;      // 刚体类型
    float x;            // 初始X位置
    float y;            // 初始Y位置
    float angle;        // 初始角度（弧度）
    float linearDamping;    // 线性阻尼
    float angularDamping;   // 角阻尼
    bool fixedRotation;     // 是否固定旋转
    bool bullet;            // 是否为子弹（连续碰撞检测）

    BodyDef()
        : type(BodyType::DYNAMIC), x(0.0f), y(0.0f), angle(0.0f)
        , linearDamping(0.0f), angularDamping(0.0f), fixedRotation(false), bullet(false) {}
};

// 形状定义
struct ShapeDef {
    ShapeType type;     // 形状类型
    float density;      // 密度
    float friction;     // 摩擦力
    float restitution;  // 弹性

    // 圆形参数
    float radius;       // 半径

    // 矩形参数
    float width;        // 宽度
    float height;       // 高度

    // 多边形顶点
    std::vector<b2Vec2> vertices;  // 顶点数组

    ShapeDef()
        : type(ShapeType::BOX), density(1.0f), friction(0.2f), restitution(0.0f)
        , radius(1.0f), width(1.0f), height(1.0f) {}
};

// 刚体类
class RigidBody {
public:
    ~RigidBody();

    // 设置位置
    void setPosition(float x, float y);

    // 获取位置
    void getPosition(float& x, float& y) const;

    // 设置角度
    void setAngle(float angle);

    // 获取角度
    float getAngle() const;

    // 设置线性速度
    void setLinearVelocity(float vx, float vy);

    // 获取线性速度
    void getLinearVelocity(float& vx, float& vy) const;

    // 设置角速度
    void setAngularVelocity(float omega);

    // 获取角速度
    float getAngularVelocity() const;

    // 施加力
    void applyForce(float fx, float fy, float px, float py);

    // 施加冲量
    void applyImpulse(float ix, float iy, float px, float py);

    // 施加扭矩
    void applyTorque(float torque);

    // 设置质量
    void setMass(float mass);

    // 获取质量
    float getMass() const;

    // 设置用户数据
    void setUserData(void* data);

    // 获取用户数据
    void* getUserData() const;

    // 启用/禁用刚体
    void setEnabled(bool enabled);

    // 检查刚体是否启用
    bool isEnabled() const;

    // 获取Box2D刚体指针
    b2Body* getB2Body();

private:
    friend class PhysicsWorld;

    RigidBody(b2Body* body);

    b2Body* m_body;
    void* m_userData;
};

// 物理世界类
class PhysicsWorld {
public:
    PhysicsWorld(const PhysicsWorldConfig& config = PhysicsWorldConfig());
    ~PhysicsWorld();

    // 更新物理世界
    void update(float deltaTime);

    // 创建刚体
    std::unique_ptr<RigidBody> createBody(const BodyDef& def, const ShapeDef& shapeDef);

    // 销毁刚体
    void destroyBody(RigidBody* body);

    // 设置碰撞监听器
    void setContactListener(ContactListener* listener);

    // 射线检测
    bool raycast(float startX, float startY, float endX, float endY, std::function<bool(RigidBody* body, float pointX, float pointY, float normalX, float normalY)> callback);

    // 设置重力
    void setGravity(float x, float y);

    // 获取重力
    void getGravity(float& x, float& y) const;

    // 启用/禁用睡眠
    void setSleepEnabled(bool enabled);

    // 清除所有刚体
    void clearBodies();

    // 获取Box2D世界指针
    b2World* getB2World();

private:
    class PhysicsContactListener : public b2ContactListener {
public:
        PhysicsContactListener(PhysicsWorld* world);

        virtual void BeginContact(b2Contact* contact) override;
        virtual void EndContact(b2Contact* contact) override;
        virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
        virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

    private:
        PhysicsWorld* m_world;
    };

    b2World* m_world;
    PhysicsWorldConfig m_config;
    ContactListener* m_contactListener;
    std::unique_ptr<PhysicsContactListener> m_physicsContactListener;
    std::vector<RigidBody*> m_bodies;
};

// 物理管理器类
class PhysicsManager {
public:
    static PhysicsManager& getInstance();

    // 初始化物理系统
    bool init();

    // 清理物理系统
    void cleanup();

    // 创建物理世界
    std::unique_ptr<PhysicsWorld> createWorld(const PhysicsWorldConfig& config = PhysicsWorldConfig());

private:
    PhysicsManager();
    ~PhysicsManager();

    bool m_initialized;
};

} // namespace Appgame

#endif // PHYSICS_H
