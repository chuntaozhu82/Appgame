#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace FishingGame {

// 基本类型定义
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef float float32;
typedef double float64;

// 坐标类型
typedef float32 Vector2f[2];
typedef float32 Vector3f[3];
typedef int32 Vector2i[2];
typedef int32 Vector3i[3];

// 颜色类型
typedef struct {
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;
} Color;

// 矩形类型
typedef struct {
    float32 x;
    float32 y;
    float32 width;
    float32 height;
} Rectf;

typedef struct {
    int32 x;
    int32 y;
    int32 width;
    int32 height;
} Recti;

// 游戏对象ID
typedef uint32 ObjectID;

// 任务ID
typedef uint32 TaskID;

// 成就ID
typedef uint32 AchievementID;

// 物品ID
typedef uint32 ItemID;

// 鱼的类型ID
typedef uint32 FishTypeID;

// 钓鱼点ID
typedef uint32 FishingSpotID;

// 天气类型
enum class WeatherType {
    SUNNY,
    CLOUDY,
    RAINY,
    STORMY,
    SNOWY
};

// 时间类型
enum class TimeType {
    MORNING,
    NOON,
    EVENING,
    NIGHT
};

// 钓鱼状态
enum class FishingState {
    IDLE,
    CASTING,
    WAITING,
    HOOKED,
    REELING,
    CAUGHT,
    FAILED
};

// 物品类型
enum class ItemType {
    ROD,
    BAIT,
    LURE,
    BOAT,
    ACCESSORY,
    CONSUMABLE,
    COLLECTIBLE
};

// 稀有度
enum class Rarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};

// 游戏难度
enum class Difficulty {
    EASY,
    NORMAL,
    HARD,
    EXPERT
};

// 语言类型
enum class Language {
    ENGLISH,
    CHINESE,
    JAPANESE,
    KOREAN,
    GERMAN,
    FRENCH,
    SPANISH
};

// 音频类型
enum class AudioType {
    MUSIC,
    SFX,
    AMBIENT
};

// 粒子类型
enum class ParticleType {
    WATER_SPLASH,
    FISH_CATCH,
    BOAT_WAKE,
    RAIN,
    SNOW,
    FIREWORK
};

// 碰撞类型
enum class CollisionType {
    NONE,
    FISH,
    BOAT,
    OBSTACLE,
    TERRAIN
};

// 网络状态
enum class NetworkStatus {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    ERROR
};

// 存储类型
enum class StorageType {
    LOCAL,
    CLOUD,
    AUTO
};

// 日志级别
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

// 工具函数
inline Color RGBA(uint8 r, uint8 g, uint8 b, uint8 a = 255) {
    return {r, g, b, a};
}

inline Color RGB(uint8 r, uint8 g, uint8 b) {
    return {r, g, b, 255};
}

inline Rectf Rect(float32 x, float32 y, float32 width, float32 height) {
    return {x, y, width, height};
}

inline Recti Rect(int32 x, int32 y, int32 width, int32 height) {
    return {x, y, width, height};
}

inline Vector2f Vec2(float32 x, float32 y) {
    Vector2f vec = {x, y};
    return vec;
}

inline Vector3f Vec3(float32 x, float32 y, float32 z) {
    Vector3f vec = {x, y, z};
    return vec;
}

} // namespace FishingGame

#endif // TYPES_H
