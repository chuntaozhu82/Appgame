#ifndef WEATHER_SYSTEM_H
#define WEATHER_SYSTEM_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include <string>
#include <vector>
#include <map>

namespace FishingGame {

// 天气系统类
class WeatherSystem {
public:
    WeatherSystem();
    ~WeatherSystem();

    // 初始化天气系统
    bool init();

    // 清理天气系统
    void cleanup();

    // 更新天气系统
    void update(float32 deltaTime);

    // 加载天气数据
    bool loadWeatherData(const std::string& filePath);

    // 获取当前天气
    WeatherType getCurrentWeather() const;

    // 获取当前天气数据
    const WeatherData* getCurrentWeatherData() const;

    // 获取天气数据
    const WeatherData* getWeatherData(WeatherType type) const;

    // 获取所有天气数据
    const std::map<WeatherType, WeatherData>& getWeatherData() const;

    // 切换天气
    bool changeWeather(WeatherType type);

    // 随机切换天气
    bool randomizeWeather();

    // 获取天气持续时间
    float32 getWeatherDuration() const;

    // 获取天气剩余时间
    float32 getWeatherRemainingTime() const;

    // 获取天气强度
    float32 getWeatherIntensity() const;

    // 设置天气强度
    void setWeatherIntensity(float32 intensity);

    // 检查天气是否正在变化
    bool isWeatherChanging() const;

    // 获取天气变化进度
    float32 getWeatherChangeProgress() const;

    // 获取鱼的生成倍率
    float32 getFishSpawnMultiplier() const;

    // 获取钓鱼成功率倍率
    float32 getCatchRateMultiplier() const;

    // 获取天气名称
    std::string getWeatherName(WeatherType type) const;

    // 获取当前天气名称
    std::string getCurrentWeatherName() const;

    // 获取天气描述
    std::string getWeatherDescription(WeatherType type) const;

    // 获取当前天气描述
    std::string getCurrentWeatherDescription() const;

    // 获取天气背景路径
    std::string getWeatherBackgroundPath(WeatherType type) const;

    // 获取当前天气背景路径
    std::string getCurrentWeatherBackgroundPath() const;

    // 获取天气粒子效果路径
    std::string getWeatherParticleEffectPath(WeatherType type) const;

    // 获取当前天气粒子效果路径
    std::string getCurrentWeatherParticleEffectPath() const;

    // 检查天气是否适合钓鱼
    bool isGoodWeatherForFishing() const;

    // 获取推荐的钓鱼点
    std::vector<FishingSpotID> getRecommendedFishingSpots() const;

private:
    // 天气数据
    std::map<WeatherType, WeatherData> m_weatherData;

    // 当前天气
    WeatherType m_currentWeather;

    // 下一个天气
    WeatherType m_nextWeather;

    // 天气持续时间
    float32 m_weatherDuration;

    // 天气剩余时间
    float32 m_weatherRemainingTime;

    // 天气强度
    float32 m_weatherIntensity;

    // 天气是否正在变化
    bool m_weatherChanging;

    // 天气变化进度
    float32 m_weatherChangeProgress;

    // 天气变化持续时间
    float32 m_weatherChangeDuration;

    // 初始化默认天气数据
    void initDefaultWeatherData();

    // 检查天气数据是否有效
    bool validateWeatherData() const;

    // 计算天气效果
    void calculateWeatherEffects();

    // 处理天气变化
    void handleWeatherChange(float32 deltaTime);

    // 随机生成天气
    WeatherType generateRandomWeather() const;

    // 获取天气权重
    float32 getWeatherWeight(WeatherType type) const;
};

} // namespace FishingGame

#endif // WEATHER_SYSTEM_H
