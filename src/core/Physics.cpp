#include "core/Physics.h"

namespace Appgame {

// RigidBody 类实现

RigidBody::RigidBody(b2Body* body)
    : m_body(body), m_userData(nullptr) {
    m_body->SetUserData(this);
}

RigidBody::~RigidBody() {
    // 注意：刚体的销毁由PhysicsWorld负责
}

void RigidBody::setPosition(float x, float y) {
    m_body->SetTransform(b2Vec2(x, y), m_body->GetAngle());
}

void RigidBody::getPosition(float& x, float& y) const {
    b2Vec2 pos = m_body->GetPosition();
    x = pos.x;
    y = pos.y;
}

void RigidBody::setAngle(float angle) {
    m_body->SetTransform(m_body->GetPosition(), angle);
}

float RigidBody::getAngle() const {
    return m_body->GetAngle();
}

void RigidBody::setLinearVelocity(float vx, float vy) {
    m_body->SetLinearVelocity(b2Vec2(vx, vy));
}

void RigidBody::getLinearVelocity(float& vx, float& vy) const {
    b2Vec2 vel = m_body->GetLinearVelocity();
    vx = vel.x;
    vy = vel.y;
}

void RigidBody::setAngularVelocity(float omega) {
    m_body->SetAngularVelocity(omega);
}

float RigidBody::getAngularVelocity() const {
    return m_body->GetAngularVelocity();
}

void RigidBody::applyForce(float fx, float fy, float px, float py) {
    m_body->ApplyForce(b2Vec2(fx, fy), b2Vec2(px, py), true);
}

void RigidBody::applyImpulse(float ix, float iy, float px, float py) {
    m_body->ApplyImpulse(b2Vec2(ix, iy), b2Vec2(px, py), true);
}

void RigidBody::applyTorque(float torque) {
    m_body->ApplyTorque(torque, true);
}

void RigidBody::setMass(float mass) {
    b2MassData massData;
    m_body->GetMassData(&massData);
    massData.mass = mass;
    m_body->SetMassData(&massData);
}

float RigidBody::getMass() const {
    return m_body->GetMass();
}

void RigidBody::setUserData(void* data) {
    m_userData = data;
}

void* RigidBody::getUserData() const {
    return m_userData;
}

void RigidBody::setEnabled(bool enabled) {
    m_body->SetEnabled(enabled);
}

bool RigidBody::isEnabled() const {
    return m_body->IsEnabled();
}

b2Body* RigidBody::getB2Body() {
    return m_body;
}

// PhysicsWorld::PhysicsContactListener 类实现

PhysicsWorld::PhysicsContactListener::PhysicsContactListener(PhysicsWorld* world)
    : m_world(world) {
}

void PhysicsWorld::PhysicsContactListener::BeginContact(b2Contact* contact) {
    if (m_world->m_contactListener) {
        ContactInfo info;
        info.bodyA = static_cast<RigidBody*>(contact->GetFixtureA()->GetBody()->GetUserData());
        info.bodyB = static_cast<RigidBody*>(contact->GetFixtureB()->GetBody()->GetUserData());
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);
        info.normalX = worldManifold.normal.x;
        info.normalY = worldManifold.normal.y;
        info.impulse = 0.0f; // 碰撞开始时冲量为0
        m_world->m_contactListener->onContactBegin(info);
    }
}

void PhysicsWorld::PhysicsContactListener::EndContact(b2Contact* contact) {
    if (m_world->m_contactListener) {
        ContactInfo info;
        info.bodyA = static_cast<RigidBody*>(contact->GetFixtureA()->GetBody()->GetUserData());
        info.bodyB = static_cast<RigidBody*>(contact->GetFixtureB()->GetBody()->GetUserData());
        info.normalX = 0.0f;
        info.normalY = 0.0f;
        info.impulse = 0.0f;
        m_world->m_contactListener->onContactEnd(info);
    }
}

void PhysicsWorld::PhysicsContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    // 可以在这里修改碰撞参数
}

void PhysicsWorld::PhysicsContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    if (m_world->m_contactListener) {
        ContactInfo info;
        info.bodyA = static_cast<RigidBody*>(contact->GetFixtureA()->GetBody()->GetUserData());
        info.bodyB = static_cast<RigidBody*>(contact->GetFixtureB()->GetBody()->GetUserData());
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);
        info.normalX = worldManifold.normal.x;
        info.normalY = worldManifold.normal.y;
        info.impulse = impulse->normalImpulses[0];
        m_world->m_contactListener->onContactPersist(info);
    }
}

// PhysicsWorld 类实现

PhysicsWorld::PhysicsWorld(const PhysicsWorldConfig& config)
    : m_config(config), m_contactListener(nullptr) {
    // 创建Box2D世界
    m_world = new b2World(b2Vec2(config.gravityX, config.gravityY));
    m_world->SetAllowSleeping(config.sleepEnabled);
    
    // 创建碰撞监听器
    m_physicsContactListener = std::make_unique<PhysicsContactListener>(this);
    m_world->SetContactListener(m_physicsContactListener.get());
}

PhysicsWorld::~PhysicsWorld() {
    clearBodies();
    delete m_world;
}

void PhysicsWorld::update(float deltaTime) {
    // 使用配置的时间步长和迭代次数
    m_world->Step(m_config.timeStep, m_config.velocityIterations, m_config.positionIterations);
}

std::unique_ptr<RigidBody> PhysicsWorld::createBody(const BodyDef& def, const ShapeDef& shapeDef) {
    // 创建刚体定义
    b2BodyDef bodyDef;
    switch (def.type) {
    case BodyType::STATIC:
        bodyDef.type = b2_staticBody;
        break;
    case BodyType::DYNAMIC:
        bodyDef.type = b2_dynamicBody;
        break;
    case BodyType::KINEMATIC:
        bodyDef.type = b2_kinematicBody;
        break;
    }
    bodyDef.position.Set(def.x, def.y);
    bodyDef.angle = def.angle;
    bodyDef.linearDamping = def.linearDamping;
    bodyDef.angularDamping = def.angularDamping;
    bodyDef.fixedRotation = def.fixedRotation;
    bodyDef.bullet = def.bullet;
    
    // 创建刚体
    b2Body* body = m_world->CreateBody(&bodyDef);
    
    // 创建形状
    b2FixtureDef fixtureDef;
    fixtureDef.density = shapeDef.density;
    fixtureDef.friction = shapeDef.friction;
    fixtureDef.restitution = shapeDef.restitution;
    
    b2Shape* shape = nullptr;
    switch (shapeDef.type) {
    case ShapeType::CIRCLE:
        {
            b2CircleShape* circle = new b2CircleShape();
            circle->m_radius = shapeDef.radius;
            shape = circle;
        }
        break;
    case ShapeType::BOX:
        {
            b2PolygonShape* box = new b2PolygonShape();
            box->SetAsBox(shapeDef.width * 0.5f, shapeDef.height * 0.5f);
            shape = box;
        }
        break;
    case ShapeType::POLYGON:
        {
            b2PolygonShape* polygon = new b2PolygonShape();
            polygon->Set(shapeDef.vertices.data(), shapeDef.vertices.size());
            shape = polygon;
        }
        break;
    case ShapeType::CHAIN:
        {
            b2ChainShape* chain = new b2ChainShape();
            chain->CreateChain(shapeDef.vertices.data(), shapeDef.vertices.size());
            shape = chain;
        }
        break;
    }
    
    if (shape) {
        fixtureDef.shape = shape;
        body->CreateFixture(&fixtureDef);
        delete shape;
    }
    
    // 创建RigidBody包装器
    auto rigidBody = std::make_unique<RigidBody>(body);
    m_bodies.push_back(rigidBody.get());
    return rigidBody;
}

void PhysicsWorld::destroyBody(RigidBody* body) {
    if (body) {
        // 从列表中移除
        auto it = std::find(m_bodies.begin(), m_bodies.end(), body);
        if (it != m_bodies.end()) {
            m_bodies.erase(it);
        }
        
        // 销毁Box2D刚体
        m_world->DestroyBody(body->getB2Body());
    }
}

void PhysicsWorld::setContactListener(ContactListener* listener) {
    m_contactListener = listener;
}

bool PhysicsWorld::raycast(float startX, float startY, float endX, float endY, std::function<bool(RigidBody* body, float pointX, float pointY, float normalX, float normalY)> callback) {
    struct RaycastCallback : public b2RayCastCallback {
        std::function<bool(RigidBody* body, float pointX, float pointY, float normalX, float normalY)> callback;
        bool hit;
        
        RaycastCallback(std::function<bool(RigidBody*, float, float, float, float)> cb)
            : callback(cb), hit(false) {}
        
        float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
            RigidBody* body = static_cast<RigidBody*>(fixture->GetBody()->GetUserData());
            if (body && callback(body, point.x, point.y, normal.x, normal.y)) {
                hit = true;
                return 0.0f; // 停止进一步检测
            }
            return 1.0f; // 继续检测
        }
    };
    
    RaycastCallback callbackObj(callback);
    m_world->RayCast(&callbackObj, b2Vec2(startX, startY), b2Vec2(endX, endY));
    return callbackObj.hit;
}

void PhysicsWorld::setGravity(float x, float y) {
    m_world->SetGravity(b2Vec2(x, y));
}

void PhysicsWorld::getGravity(float& x, float& y) const {
    b2Vec2 gravity = m_world->GetGravity();
    x = gravity.x;
    y = gravity.y;
}

void PhysicsWorld::setSleepEnabled(bool enabled) {
    m_world->SetAllowSleeping(enabled);
}

void PhysicsWorld::clearBodies() {
    // 销毁所有刚体
    for (auto body : m_bodies) {
        m_world->DestroyBody(body->getB2Body());
    }
    m_bodies.clear();
}

b2World* PhysicsWorld::getB2World() {
    return m_world;
}

// PhysicsManager 类实现

PhysicsManager::PhysicsManager()
    : m_initialized(false) {
}

PhysicsManager::~PhysicsManager() {
    cleanup();
}

PhysicsManager& PhysicsManager::getInstance() {
    static PhysicsManager instance;
    return instance;
}

bool PhysicsManager::init() {
    if (!m_initialized) {
        // Box2D初始化
        m_initialized = true;
    }
    return true;
}

void PhysicsManager::cleanup() {
    if (m_initialized) {
        m_initialized = false;
    }
}

std::unique_ptr<PhysicsWorld> PhysicsManager::createWorld(const PhysicsWorldConfig& config) {
    return std::make_unique<PhysicsWorld>(config);
}

} // namespace Appgame
