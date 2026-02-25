#include "fishing/systems/SceneManager.h"
#include "fishing/systems/WeatherSystem.h"
#include "fishing/systems/TimeSystem.h"
#include <iostream>

namespace FishingGame {

// Scene implementation
Scene::~Scene() {
}

// BaseScene implementation
BaseScene::BaseScene(SceneType type, const std::string& name)
    : m_type(type),
      m_name(name),
      m_active(false),
      m_width(1920),
      m_height(1080),
      m_weatherSystem(nullptr),
      m_timeSystem(nullptr)
{
}

BaseScene::~BaseScene() {
    cleanup();
}

bool BaseScene::init() {
    std::cout << "BaseScene initialized: " << m_name << std::endl;
    return true;
}

void BaseScene::cleanup() {
    std::cout << "BaseScene cleaned up: " << m_name << std::endl;
}

void BaseScene::enter() {
    m_active = true;
    std::cout << "Entered BaseScene: " << m_name << std::endl;
}

void BaseScene::exit() {
    m_active = false;
    std::cout << "Exited BaseScene: " << m_name << std::endl;
}

void BaseScene::update(float32 deltaTime) {
    if (!m_active) {
        return;
    }
    
    // 更新天气系统
    if (m_weatherSystem) {
        m_weatherSystem->update(deltaTime);
    }
    
    // 更新时间系统
    if (m_timeSystem) {
        m_timeSystem->update(deltaTime);
    }
}

void BaseScene::render() {
    if (!m_active) {
        return;
    }
}

bool BaseScene::handleInput(int32 inputType, int32 inputValue) {
    if (!m_active) {
        return false;
    }
    return false;
}

SceneType BaseScene::getType() const {
    return m_type;
}

std::string BaseScene::getName() const {
    return m_name;
}

bool BaseScene::isActive() const {
    return m_active;
}

void BaseScene::setActive(bool active) {
    m_active = active;
}

int32 BaseScene::getWidth() const {
    return m_width;
}

int32 BaseScene::getHeight() const {
    return m_height;
}

void BaseScene::setSize(int32 width, int32 height) {
    m_width = width;
    m_height = height;
}

WeatherSystem* BaseScene::getWeatherSystem() const {
    return m_weatherSystem;
}

TimeSystem* BaseScene::getTimeSystem() const {
    return m_timeSystem;
}

void BaseScene::setWeatherSystem(WeatherSystem* weatherSystem) {
    m_weatherSystem = weatherSystem;
}

void BaseScene::setTimeSystem(TimeSystem* timeSystem) {
    m_timeSystem = timeSystem;
}

// GameScene implementation
GameScene::GameScene()
    : BaseScene(SceneType::GAME_SCENE, "Game Scene"),
      m_currentFishingSpot(0)
{
}

GameScene::~GameScene() {
    cleanup();
}

bool GameScene::init() {
    if (!BaseScene::init()) {
        return false;
    }
    
    // 初始化钓鱼点
    initFishingSpots();
    
    std::cout << "GameScene initialized" << std::endl;
    return true;
}

void GameScene::cleanup() {
    // 清理钓鱼点数据
    m_fishingSpots.clear();
    
    BaseScene::cleanup();
}

void GameScene::enter() {
    BaseScene::enter();
    
    // 设置默认钓鱼点
    if (!m_fishingSpots.empty()) {
        m_currentFishingSpot = m_fishingSpots.begin()->first;
    }
    
    std::cout << "Entered GameScene, current fishing spot: " << m_currentFishingSpot << std::endl;
}

void GameScene::exit() {
    BaseScene::exit();
}

void GameScene::update(float32 deltaTime) {
    BaseScene::update(deltaTime);
    
    // 更新游戏逻辑
    // TODO: 实现游戏逻辑更新
}

void GameScene::render() {
    BaseScene::render();
    
    // 渲染钓鱼点
    renderFishingSpot();
    
    // 渲染天气效果
    renderWeatherEffects();
    
    // 渲染时间效果
    renderTimeEffects();
}

bool GameScene::handleInput(int32 inputType, int32 inputValue) {
    if (!BaseScene::handleInput(inputType, inputValue)) {
        return false;
    }
    
    // 处理游戏输入
    // TODO: 实现游戏输入处理
    
    return true;
}

void GameScene::setCurrentFishingSpot(FishingSpotID spotId) {
    if (m_fishingSpots.find(spotId) != m_fishingSpots.end()) {
        m_currentFishingSpot = spotId;
        std::cout << "Switched to fishing spot: " << spotId << std::endl;
    } else {
        std::cerr << "Invalid fishing spot ID: " << spotId << std::endl;
    }
}

FishingSpotID GameScene::getCurrentFishingSpot() const {
    return m_currentFishingSpot;
}

void GameScene::initFishingSpots() {
    // 初始化默认钓鱼点
    FishingSpot spot1;
    spot1.id = 1;
    spot1.name = "宁静湖畔";
    spot1.description = "一个宁静的湖畔，适合初学者钓鱼";
    spot1.position[0] = 0.0f;
    spot1.position[1] = 0.0f;
    spot1.size = 500.0f;
    spot1.depth = 10.0f;
    spot1.availableFish = {1, 2, 3}; // 假设这些是鱼的类型ID
    spot1.backgroundPath = "assets/textures/backgrounds/lake.png";
    spot1.waterTexturePath = "assets/textures/water/calm_water.png";
    spot1.fishSpawnRate = 1.0f;
    spot1.baseCatchRate = 0.8f;
    m_fishingSpots[spot1.id] = spot1;
    
    FishingSpot spot2;
    spot2.id = 2;
    spot2.name = "汹涌河流";
    spot2.description = "一条汹涌的河流，有更多种类的鱼";
    spot2.position[0] = 1000.0f;
    spot2.position[1] = 0.0f;
    spot2.size = 800.0f;
    spot2.depth = 15.0f;
    spot2.availableFish = {2, 3, 4, 5};
    spot2.backgroundPath = "assets/textures/backgrounds/river.png";
    spot2.waterTexturePath = "assets/textures/water/river_water.png";
    spot2.fishSpawnRate = 1.2f;
    spot2.baseCatchRate = 0.6f;
    m_fishingSpots[spot2.id] = spot2;
    
    FishingSpot spot3;
    spot3.id = 3;
    spot3.name = "神秘海域";
    spot3.description = "一片神秘的海域，有稀有种类的鱼";
    spot3.position[0] = 0.0f;
    spot3.position[1] = 1000.0f;
    spot3.size = 1000.0f;
    spot3.depth = 20.0f;
    spot3.availableFish = {3, 4, 5, 6, 7};
    spot3.backgroundPath = "assets/textures/backgrounds/ocean.png";
    spot3.waterTexturePath = "assets/textures/water/ocean_water.png";
    spot3.fishSpawnRate = 0.8f;
    spot3.baseCatchRate = 0.4f;
    m_fishingSpots[spot3.id] = spot3;
    
    std::cout << "Initialized " << m_fishingSpots.size() << " fishing spots" << std::endl;
}

bool GameScene::loadFishingSpots(const std::string& filePath) {
    // 加载钓鱼点数据
    // TODO: 实现从文件加载钓鱼点数据
    return true;
}

void GameScene::renderFishingSpot() {
    // 渲染当前钓鱼点
    auto it = m_fishingSpots.find(m_currentFishingSpot);
    if (it != m_fishingSpots.end()) {
        const FishingSpot& spot = it->second;
        // TODO: 实现钓鱼点渲染
        std::cout << "Rendering fishing spot: " << spot.name << std::endl;
    }
}

void GameScene::renderWeatherEffects() {
    // 渲染天气效果
    if (m_weatherSystem) {
        // TODO: 实现天气效果渲染
        std::cout << "Rendering weather effects: " << m_weatherSystem->getCurrentWeatherName() << std::endl;
    }
}

void GameScene::renderTimeEffects() {
    // 渲染时间效果
    if (m_timeSystem) {
        // TODO: 实现时间效果渲染
        std::cout << "Rendering time effects: " << m_timeSystem->getCurrentTimeName() << std::endl;
    }
}

// SceneManager implementation
SceneManager::SceneManager()
    : m_currentScene(nullptr),
      m_currentSceneType(SceneType::MAIN_MENU),
      m_weatherSystem(nullptr),
      m_timeSystem(nullptr),
      m_width(1920),
      m_height(1080)
{
}

SceneManager::~SceneManager() {
    cleanup();
}

bool SceneManager::init() {
    // 初始化默认场景
    initDefaultScenes();
    
    // 切换到主菜单场景
    if (!changeScene(SceneType::MAIN_MENU)) {
        return false;
    }
    
    std::cout << "SceneManager initialized successfully" << std::endl;
    return true;
}

void SceneManager::cleanup() {
    // 清理所有场景
    clearScenes();
    
    std::cout << "SceneManager cleaned up" << std::endl;
}

void SceneManager::update(float32 deltaTime) {
    // 更新当前场景
    if (m_currentScene) {
        m_currentScene->update(deltaTime);
    }
}

void SceneManager::render() {
    // 渲染当前场景
    if (m_currentScene) {
        m_currentScene->render();
    }
}

bool SceneManager::handleInput(int32 inputType, int32 inputValue) {
    // 处理当前场景的输入
    if (m_currentScene) {
        return m_currentScene->handleInput(inputType, inputValue);
    }
    return false;
}

bool SceneManager::addScene(Scene* scene) {
    if (!scene) {
        std::cerr << "Invalid scene" << std::endl;
        return false;
    }
    
    SceneType type = scene->getType();
    
    // 检查场景是否已存在
    if (m_scenes.find(type) != m_scenes.end()) {
        std::cerr << "Scene already exists: " << static_cast<int>(type) << std::endl;
        return false;
    }
    
    // 初始化场景
    if (!scene->init()) {
        std::cerr << "Failed to initialize scene: " << scene->getName() << std::endl;
        return false;
    }
    
    // 设置场景大小
    scene->setSize(m_width, m_height);
    
    // 设置天气系统和时间系统
    scene->setWeatherSystem(m_weatherSystem);
    scene->setTimeSystem(m_timeSystem);
    
    // 添加场景到映射
    m_scenes[type] = scene;
    
    std::cout << "Added scene: " << scene->getName() << " (Type: " << static_cast<int>(type) << ")" << std::endl;
    return true;
}

void SceneManager::removeScene(SceneType type) {
    auto it = m_scenes.find(type);
    if (it != m_scenes.end()) {
        Scene* scene = it->second;
        
        // 如果是当前场景，先切换到其他场景
        if (scene == m_currentScene) {
            // 尝试切换到主菜单场景
            if (m_scenes.find(SceneType::MAIN_MENU) != m_scenes.end()) {
                changeScene(SceneType::MAIN_MENU);
            } else if (m_scenes.size() > 1) {
                // 切换到第一个非当前场景
                for (const auto& pair : m_scenes) {
                    if (pair.first != type) {
                        changeScene(pair.first);
                        break;
                    }
                }
            } else {
                // 没有其他场景，清空当前场景
                m_currentScene = nullptr;
                m_currentSceneType = SceneType::MAIN_MENU;
            }
        }
        
        // 清理场景
        scene->cleanup();
        delete scene;
        
        // 从映射中移除
        m_scenes.erase(it);
        
        std::cout << "Removed scene: " << static_cast<int>(type) << std::endl;
    }
}

bool SceneManager::changeScene(SceneType type) {
    // 检查场景是否存在
    auto it = m_scenes.find(type);
    if (it == m_scenes.end()) {
        std::cerr << "Scene not found: " << static_cast<int>(type) << std::endl;
        return false;
    }
    
    Scene* newScene = it->second;
    
    // 退出当前场景
    if (m_currentScene) {
        m_currentScene->exit();
    }
    
    // 进入新场景
    newScene->enter();
    
    // 更新当前场景
    m_currentScene = newScene;
    m_currentSceneType = type;
    
    std::cout << "Changed scene to: " << newScene->getName() << " (Type: " << static_cast<int>(type) << ")" << std::endl;
    return true;
}

Scene* SceneManager::getCurrentScene() const {
    return m_currentScene;
}

Scene* SceneManager::getScene(SceneType type) const {
    auto it = m_scenes.find(type);
    if (it != m_scenes.end()) {
        return it->second;
    }
    return nullptr;
}

const std::map<SceneType, Scene*>& SceneManager::getScenes() const {
    return m_scenes;
}

WeatherSystem* SceneManager::getWeatherSystem() const {
    return m_weatherSystem;
}

TimeSystem* SceneManager::getTimeSystem() const {
    return m_timeSystem;
}

void SceneManager::setWeatherSystem(WeatherSystem* weatherSystem) {
    m_weatherSystem = weatherSystem;
    
    // 更新所有场景的天气系统
    for (auto& pair : m_scenes) {
        pair.second->setWeatherSystem(weatherSystem);
    }
}

void SceneManager::setTimeSystem(TimeSystem* timeSystem) {
    m_timeSystem = timeSystem;
    
    // 更新所有场景的时间系统
    for (auto& pair : m_scenes) {
        pair.second->setTimeSystem(timeSystem);
    }
}

SceneType SceneManager::getCurrentSceneType() const {
    return m_currentSceneType;
}

bool SceneManager::hasScene(SceneType type) const {
    return m_scenes.find(type) != m_scenes.end();
}

void SceneManager::setSceneSize(int32 width, int32 height) {
    m_width = width;
    m_height = height;
    
    // 更新所有场景的大小
    for (auto& pair : m_scenes) {
        pair.second->setSize(width, height);
    }
    
    std::cout << "Set scene size to: " << width << "x" << height << std::endl;
}

void SceneManager::getSceneSize(int32& width, int32& height) const {
    width = m_width;
    height = m_height;
}

void SceneManager::initDefaultScenes() {
    // 创建并添加默认场景
    
    // 主菜单场景
    Scene* mainMenuScene = new BaseScene(SceneType::MAIN_MENU, "Main Menu");
    addScene(mainMenuScene);
    
    // 游戏场景
    Scene* gameScene = new GameScene();
    addScene(gameScene);
    
    // 钓鱼场景
    Scene* fishingScene = new BaseScene(SceneType::FISHING_SCENE, "Fishing Scene");
    addScene(fishingScene);
    
    // 商店场景
    Scene* shopScene = new BaseScene(SceneType::SHOP_SCENE, "Shop Scene");
    addScene(shopScene);
    
    // 背包场景
    Scene* inventoryScene = new BaseScene(SceneType::INVENTORY_SCENE, "Inventory Scene");
    addScene(inventoryScene);
    
    // 任务场景
    Scene* taskScene = new BaseScene(SceneType::TASK_SCENE, "Task Scene");
    addScene(taskScene);
    
    // 成就场景
    Scene* achievementScene = new BaseScene(SceneType::ACHIEVEMENT_SCENE, "Achievement Scene");
    addScene(achievementScene);
    
    // 设置场景
    Scene* settingsScene = new BaseScene(SceneType::SETTINGS_SCENE, "Settings Scene");
    addScene(settingsScene);
    
    // 游戏结束场景
    Scene* gameOverScene = new BaseScene(SceneType::GAME_OVER_SCENE, "Game Over Scene");
    addScene(gameOverScene);
    
    std::cout << "Initialized " << m_scenes.size() << " default scenes" << std::endl;
}

void SceneManager::clearScenes() {
    // 清理所有场景
    for (auto& pair : m_scenes) {
        Scene* scene = pair.second;
        scene->cleanup();
        delete scene;
    }
    
    m_scenes.clear();
    m_currentScene = nullptr;
    m_currentSceneType = SceneType::MAIN_MENU;
    
    std::cout << "Cleared all scenes" << std::endl;
}

// Global scene manager instance
SceneManager* g_sceneManager = nullptr;

} // namespace FishingGame
