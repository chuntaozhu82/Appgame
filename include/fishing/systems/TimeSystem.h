#ifndef TIME_SYSTEM_H
#define TIME_SYSTEM_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include <string>
#include <vector>
#include <map>

namespace FishingGame {

// 时间系统类
class TimeSystem {
public:
    TimeSystem();
    ~TimeSystem();

    // 初始化时间系统
    bool init();

    // 清理时间系统
    void cleanup();

    // 更新时间系统
    void update(float32 deltaTime);

    // 加载时间数据
    bool loadTimeData(const std::string& filePath);

    // 获取当前时间类型
    TimeType getCurrentTime() const;

    // 获取当前时间数据
    const TimeData* getCurrentTimeData() const;

    // 获取时间数据
    const TimeData* getTimeData(TimeType type) const;

    // 获取所有时间数据
    const std::map<TimeType, TimeData>& getTimeData() const;

    // 切换时间
    bool changeTime(TimeType type);

    // 推进时间
    bool advanceTime();

    // 获取游戏时间
    float32 getGameTime() const;

    // 设置游戏时间
    void setGameTime(float32 time);

    // 获取游戏时间流速
    float32 getTimeSpeed() const;

    // 设置游戏时间流速
    void setTimeSpeed(float32 speed);

    // 获取时间持续时间
    float32 getTimeDuration() const;

    // 获取时间剩余时间
    float32 getTimeRemainingTime() const;

    // 检查时间是否正在变化
    bool isTimeChanging() const;

    // 获取时间变化进度
    float32 getTimeChangeProgress() const;

    // 获取鱼的生成倍率
    float32 getFishSpawnMultiplier() const;

    // 获取钓鱼成功率倍率
    float32 getCatchRateMultiplier() const;

    // 获取时间名称
    std::string getTimeName(TimeType type) const;

    // 获取当前时间名称
    std::string getCurrentTimeName() const;

    // 获取时间描述
    std::string getTimeDescription(TimeType type) const;

    // 获取当前时间描述
    std::string getCurrentTimeDescription() const;

    // 获取时间背景路径
    std::string getTimeBackgroundPath(TimeType type) const;

    // 获取当前时间背景路径
    std::string getCurrentTimeBackgroundPath() const;

    // 获取时间开始时间
    float32 getTimeStartTime(TimeType type) const;

    // 获取时间结束时间
    float32 getTimeEndTime(TimeType type) const;

    // 获取当前时间在一天中的比例
    float32 getTimeOfDay() const;

    // 设置时间为白天
    void setToDaytime();

    // 设置时间为夜晚
    void setToNighttime();

    // 检查是否是白天
    bool isDaytime() const;

    // 检查是否是夜晚
    bool isNighttime() const;

    // 检查是否是早晨
    bool isMorning() const;

    // 检查是否是中午
    bool isNoon() const;

    // 检查是否是傍晚
    bool isEvening() const;

    // 获取一天中的小时数
    int32 getHours() const;

    // 获取一天中的分钟数
    int32 getMinutes() const;

    // 获取一天中的秒数
    int32 getSeconds() const;

    // 获取时间字符串
    std::string getTimeString() const;

    // 获取当前时间的光照强度
    float32 getLightIntensity() const;

    // 获取当前时间的温度
    float32 getTemperature() const;

private:
    // 时间数据
    std::map<TimeType, TimeData> m_timeData;

    // 当前时间类型
    TimeType m_currentTime;

    // 下一个时间类型
    TimeType m_nextTime;

    // 游戏时间
    float32 m_gameTime;

    // 时间流速
    float32 m_timeSpeed;

    // 时间持续时间
    float32 m_timeDuration;

    // 时间剩余时间
    float32 m_timeRemainingTime;

    // 时间是否正在变化
    bool m_timeChanging;

    // 时间变化进度
    float32 m_timeChangeProgress;

    // 时间变化持续时间
    float32 m_timeChangeDuration;

    // 初始化默认时间数据
    void initDefaultTimeData();

    // 检查时间数据是否有效
    bool validateTimeData() const;

    // 计算时间效果
    void calculateTimeEffects();

    // 处理时间变化
    void handleTimeChange(float32 deltaTime);

    // 计算当前时间类型
    TimeType calculateCurrentTimeType() const;

    // 计算光照强度
    float32 calculateLightIntensity() const;

    // 计算温度
    float32 calculateTemperature() const;

    // 格式化时间字符串
    std::string formatTimeString(int32 hours, int32 minutes, int32 seconds) const;
};

} // namespace FishingGame

#endif // TIME_SYSTEM_H
