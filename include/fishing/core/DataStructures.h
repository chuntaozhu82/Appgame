#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "fishing/core/Types.h"
#include <string>
#include <vector>
#include <map>

namespace FishingGame {

// 鱼的类型数据结构
typedef struct {
    FishTypeID id;
    std::string name;
    std::string description;
    Rarity rarity;
    float32 minSize;
    float32 maxSize;
    float32 minWeight;
    float32 maxWeight;
    float32 baseValue;
    float32 catchDifficulty;
    std::vector<WeatherType> preferredWeather;
    std::vector<TimeType> preferredTime;
    float32 spawnRate;
    std::string modelPath;
    std::string texturePath;
} FishType;

// 物品数据结构
typedef struct {
    ItemID id;
    std::string name;
    std::string description;
    ItemType type;
    Rarity rarity;
    int32 value;
    int32 maxStack;
    bool isUsable;
    bool isEquippable;
    bool isConsumable;
    std::string iconPath;
    std::string modelPath;
    // 物品属性
    std::map<std::string, float32> attributes;
} Item;

// 钓鱼点数据结构
typedef struct {
    FishingSpotID id;
    std::string name;
    std::string description;
    Vector2f position;
    float32 size;
    float32 depth;
    std::vector<FishTypeID> availableFish;
    std::string backgroundPath;
    std::string waterTexturePath;
    float32 fishSpawnRate;
    float32 baseCatchRate;
} FishingSpot;

// 玩家数据结构
typedef struct {
    std::string name;
    int32 level;
    float32 experience;
    int32 money;
    Vector2f position;
    ItemID equippedRod;
    ItemID equippedBait;
    ItemID equippedLure;
    ItemID equippedBoat;
    std::vector<ItemID> inventory;
    std::vector<FishTypeID> caughtFish;
    std::vector<TaskID> activeTasks;
    std::vector<TaskID> completedTasks;
    std::vector<AchievementID> unlockedAchievements;
    float32 castingSkill;
    float32 reelingSkill;
    float32 luck;
} PlayerData;

// 任务数据结构
typedef struct {
    TaskID id;
    std::string title;
    std::string description;
    std::string objective;
    int32 requiredCount;
    int32 currentCount;
    bool isCompleted;
    bool isClaimed;
    int32 rewardMoney;
    int32 rewardExperience;
    std::vector<ItemID> rewardItems;
    std::vector<FishTypeID> requiredFish;
    std::vector<ItemID> requiredItems;
} Task;

// 成就数据结构
typedef struct {
    AchievementID id;
    std::string title;
    std::string description;
    std::string iconPath;
    int32 requiredCount;
    int32 currentCount;
    bool isUnlocked;
    int32 rewardExperience;
    std::vector<ItemID> rewardItems;
} Achievement;

// 商店物品数据结构
typedef struct {
    ItemID itemId;
    int32 price;
    int32 stock;
    bool isAvailable;
    std::vector<TimeType> availableTimes;
    std::vector<WeatherType> availableWeathers;
} ShopItem;

// 天气数据结构
typedef struct {
    WeatherType type;
    std::string name;
    std::string description;
    float32 duration;
    float32 intensity;
    std::string backgroundPath;
    std::string particleEffectPath;
    float32 fishSpawnMultiplier;
    float32 catchRateMultiplier;
} WeatherData;

// 时间数据结构
typedef struct {
    TimeType type;
    std::string name;
    std::string description;
    float32 startTime;
    float32 endTime;
    std::string backgroundPath;
    float32 fishSpawnMultiplier;
    float32 catchRateMultiplier;
} TimeData;

// 游戏状态数据结构
typedef struct {
    PlayerData player;
    FishingSpotID currentFishingSpot;
    WeatherType currentWeather;
    TimeType currentTime;
    float32 gameTime;
    bool isPaused;
    bool isFishing;
    FishingState fishingState;
    FishTypeID currentFish;
    float32 fishSize;
    float32 fishWeight;
    float32 reelingProgress;
    float32 fishStrength;
    float32 lineTension;
} GameState;

// 配置数据结构
typedef struct {
    int32 screenWidth;
    int32 screenHeight;
    bool fullscreen;
    int32 framerateLimit;
    int32 volumeMusic;
    int32 volumeSFX;
    int32 volumeAmbient;
    Language language;
    Difficulty difficulty;
    StorageType saveType;
    bool enableVSync;
    bool enableParticles;
    bool enableShadows;
    bool enableBloom;
    int32 textureQuality;
    int32 shadowQuality;
    int32 antiAliasing;
} GameConfig;

// 网络数据结构
typedef struct {
    std::string serverAddress;
    int32 serverPort;
    bool isConnected;
    NetworkStatus status;
    std::string playerName;
    std::string sessionID;
} NetworkData;

// 性能数据结构
typedef struct {
    float32 averageFramerate;
    float32 minimumFramerate;
    float32 maximumFramerate;
    float32 frameTime;
    float32 cpuUsage;
    float32 memoryUsage;
    float32 gpuUsage;
    float32 drawCalls;
    float32 triangles;
    float32 vertices;
} PerformanceData;

} // namespace FishingGame

#endif // DATA_STRUCTURES_H
