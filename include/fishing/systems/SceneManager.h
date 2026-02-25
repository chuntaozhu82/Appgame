#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace FishingGame {

// 前向声明
class WeatherSystem;
class TimeSystem;
class Scene;

// 场景类型
enum class SceneType {
    MAIN_MENU,
    GAME_SCENE,
    FISHING_SCENE,
    SHOP_SCENE,
    INVENTORY_SCENE,
    TASK_SCENE,
    ACHIEVEMENT_SCENE,
    SETTINGS_SCENE,
    GAME_OVER_SCENE
};

// 场景接口
class Scene {
public:
    virtual ~Scene() = default;

    // 初始化场景
    virtual bool init() = 0;

    // 清理场景
    virtual void cleanup() = 0;

    // 进入场景
    virtual void enter() = 0;

    // 退出场景
    virtual void exit() = 0;

    // 更新场景
    virtual void update(float32 deltaTime) = 0;

    // 渲染场景
    virtual void render() = 0;

    // 处理输入
    virtual bool handleInput(int32 inputType, int32 inputValue) = 0;

    // 获取场景类型
    virtual SceneType getType() const = 0;

    // 获取场景名称
    virtual std::string getName() const = 0;

    // 检查场景是否活跃
    virtual bool isActive() const = 0;

    // 设置场景活跃状态
    virtual void setActive(bool active) = 0;

    // 获取场景宽度
    virtual int32 getWidth() const = 0;

    // 获取场景高度
    virtual int32 getHeight() const = 0;

    // 设置场景大小
    virtual void setSize(int32 width, int32 height) = 0;

    // 获取天气系统
    virtual WeatherSystem* getWeatherSystem() const = 0;

    // 获取时间系统
    virtual TimeSystem* getTimeSystem() const = 0;
};

// 基础场景类
class BaseScene : public Scene {
public:
    BaseScene(SceneType type, const std::string& name);
    ~BaseScene() override;

    // 初始化场景
    bool init() override;

    // 清理场景
    void cleanup() override;

    // 进入场景
    void enter() override;

    // 退出场景
    void exit() override;

    // 更新场景
    void update(float32 deltaTime) override;

    // 渲染场景
    void render() override;

    // 处理输入
    bool handleInput(int32 inputType, int32 inputValue) override;

    // 获取场景类型
    SceneType getType() const override;

    // 获取场景名称
    std::string getName() const override;

    // 检查场景是否活跃
    bool isActive() const override;

    // 设置场景活跃状态
    void setActive(bool active) override;

    // 获取场景宽度
    int32 getWidth() const override;

    // 获取场景高度
    int32 getHeight() const override;

    // 设置场景大小
    void setSize(int32 width, int32 height) override;

    // 获取天气系统
    WeatherSystem* getWeatherSystem() const override;

    // 获取时间系统
    TimeSystem* getTimeSystem() const override;

    // 设置天气系统
    void setWeatherSystem(WeatherSystem* weatherSystem);

    // 设置时间系统
    void setTimeSystem(TimeSystem* timeSystem);

protected:
    // 场景类型
    SceneType m_type;

    // 场景名称
    std::string m_name;

    // 场景活跃状态
    bool m_active;

    // 场景大小
    int32 m_width;
    int32 m_height;

    // 天气系统
    WeatherSystem* m_weatherSystem;

    // 时间系统
    TimeSystem* m_timeSystem;
};

// 游戏场景类
class GameScene : public BaseScene {
public:
    GameScene();
    ~GameScene() override;

    // 初始化场景
    bool init() override;

    // 清理场景
    void cleanup() override;

    // 进入场景
    void enter() override;

    // 退出场景
    void exit() override;

    // 更新场景
    void update(float32 deltaTime) override;

    // 渲染场景
    void render() override;

    // 处理输入
    bool handleInput(int32 inputType, int32 inputValue) override;

    // 设置当前钓鱼点
    void setCurrentFishingSpot(FishingSpotID spotId);

    // 获取当前钓鱼点
    FishingSpotID getCurrentFishingSpot() const;

private:
    // 当前钓鱼点
    FishingSpotID m_currentFishingSpot;

    // 钓鱼点数据
    std::map<FishingSpotID, FishingSpot> m_fishingSpots;

    // 初始化钓鱼点
    void initFishingSpots();

    // 加载钓鱼点数据
    bool loadFishingSpots(const std::string& filePath);

    // 渲染钓鱼点
    void renderFishingSpot();

    // 渲染天气效果
    void renderWeatherEffects();

    // 渲染时间效果
    void renderTimeEffects();
};

// 场景管理器类
class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // 初始化场景管理器
    bool init();

    // 清理场景管理器
    void cleanup();

    // 更新场景管理器
    void update(float32 deltaTime);

    // 渲染场景管理器
    void render();

    // 处理输入
    bool handleInput(int32 inputType, int32 inputValue);

    // 添加场景
    bool addScene(Scene* scene);

    // 移除场景
    void removeScene(SceneType type);

    // 切换场景
    bool changeScene(SceneType type);

    // 获取当前场景
    Scene* getCurrentScene() const;

    // 获取场景
    Scene* getScene(SceneType type) const;

    // 获取所有场景
    const std::map<SceneType, Scene*>& getScenes() const;

    // 获取天气系统
    WeatherSystem* getWeatherSystem() const;

    // 获取时间系统
    TimeSystem* getTimeSystem() const;

    // 设置天气系统
    void setWeatherSystem(WeatherSystem* weatherSystem);

    // 设置时间系统
    void setTimeSystem(TimeSystem* timeSystem);

    // 获取当前场景类型
    SceneType getCurrentSceneType() const;

    // 检查场景是否存在
    bool hasScene(SceneType type) const;

    // 设置场景大小
    void setSceneSize(int32 width, int32 height);

    // 获取场景大小
    void getSceneSize(int32& width, int32& height) const;

private:
    // 场景映射
    std::map<SceneType, Scene*> m_scenes;

    // 当前场景
    Scene* m_currentScene;

    // 当前场景类型
    SceneType m_currentSceneType;

    // 天气系统
    WeatherSystem* m_weatherSystem;

    // 时间系统
    TimeSystem* m_timeSystem;

    // 场景大小
    int32 m_width;
    int32 m_height;

    // 初始化默认场景
    void initDefaultScenes();

    // 清理所有场景
    void clearScenes();
};

// 全局场景管理器实例
extern SceneManager* g_sceneManager;

} // namespace FishingGame

#endif // SCENE_MANAGER_H
