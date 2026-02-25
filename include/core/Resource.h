#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <future>
#include <mutex>

namespace Appgame {

// 资源类型
enum class ResourceType {
    TEXTURE,     // 纹理资源
    SOUND,       // 音效资源
    MUSIC,       // 音乐资源
    SHADER,      // 着色器资源
    MODEL,       // 模型资源
    FONT,        // 字体资源
    DATA,        // 数据资源
    UNKNOWN      // 未知资源类型
};

// 资源状态
enum class ResourceStatus {
    UNLOADED,    // 未加载
    LOADING,     // 加载中
    LOADED,      // 已加载
    FAILED       // 加载失败
};

// 资源基类
class Resource {
public:
    virtual ~Resource() = default;

    // 获取资源名称
    const std::string& getName() const;

    // 获取资源路径
    const std::string& getPath() const;

    // 获取资源类型
    ResourceType getType() const;

    // 获取资源状态
    ResourceStatus getStatus() const;

    // 加载资源
    virtual bool load() = 0;

    // 卸载资源
    virtual void unload() = 0;

    // 获取资源大小（字节）
    virtual size_t getSize() const = 0;

protected:
    Resource(const std::string& name, const std::string& path, ResourceType type);

    // 更新资源状态
    void setStatus(ResourceStatus status);

private:
    std::string m_name;
    std::string m_path;
    ResourceType m_type;
    ResourceStatus m_status;
};

// 资源加载器抽象类
class ResourceLoader {
public:
    virtual ~ResourceLoader() = default;

    // 加载资源
    virtual std::unique_ptr<Resource> load(const std::string& path, ResourceType type) = 0;

    // 卸载资源
    virtual void unload(Resource* resource) = 0;

    // 检查资源是否存在
    virtual bool exists(const std::string& path) const = 0;

    // 获取资源大小
    virtual size_t getSize(const std::string& path) const = 0;
};

// 资源加载请求
struct ResourceLoadRequest {
    std::string path;
    ResourceType type;
    std::function<void(std::unique_ptr<Resource>)> callback;
};

// 资源管理器类
class ResourceManager {
public:
    static ResourceManager& getInstance();

    // 初始化资源管理器
    bool init();

    // 清理资源管理器
    void cleanup();

    // 设置资源加载器
    void setLoader(ResourceType type, std::unique_ptr<ResourceLoader> loader);

    // 加载资源（同步）
    std::shared_ptr<Resource> loadResource(const std::string& path, ResourceType type);

    // 加载资源（异步）
    void loadResourceAsync(const std::string& path, ResourceType type, std::function<void(std::shared_ptr<Resource>)> callback);

    // 卸载资源
    void unloadResource(const std::string& path);

    // 卸载所有资源
    void unloadAllResources();

    // 获取资源
    std::shared_ptr<Resource> getResource(const std::string& path) const;

    // 检查资源是否已加载
    bool isResourceLoaded(const std::string& path) const;

    // 预加载资源
    void preloadResources(const std::vector<std::pair<std::string, ResourceType>>& resources);

    // 获取已加载资源数量
    size_t getLoadedResourceCount() const;

    // 获取总内存使用量
    size_t getTotalMemoryUsage() const;

private:
    ResourceManager();
    ~ResourceManager();

    // 内部方法
    void processLoadRequests();

    // 资源加载器映射
    std::unordered_map<ResourceType, std::unique_ptr<ResourceLoader>> m_loaders;

    // 已加载资源映射
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;

    // 加载请求队列
    std::vector<ResourceLoadRequest> m_loadRequests;

    // 线程和同步
    std::thread m_loadThread;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_running;

    // 内存使用统计
    size_t m_totalMemoryUsage;
};

// 资源缓存类
class ResourceCache {
public:
    static ResourceCache& getInstance();

    // 设置缓存大小限制（字节）
    void setCacheSizeLimit(size_t limit);

    // 缓存资源
    void cacheResource(const std::string& key, std::shared_ptr<Resource> resource);

    // 获取缓存的资源
    std::shared_ptr<Resource> getCachedResource(const std::string& key);

    // 从缓存中移除资源
    void removeCachedResource(const std::string& key);

    // 清空缓存
    void clearCache();

    // 获取缓存使用情况
    size_t getCacheSize() const;
    size_t getCacheLimit() const;

private:
    ResourceCache();
    ~ResourceCache();

    // 缓存项
    struct CacheItem {
        std::string key;
        std::shared_ptr<Resource> resource;
        size_t size;
        std::chrono::steady_clock::time_point lastAccessed;
    };

    // 缓存容器
    std::unordered_map<std::string, CacheItem> m_cache;

    // 缓存大小限制
    size_t m_cacheSizeLimit;
    size_t m_currentCacheSize;

    // 互斥锁
    mutable std::mutex m_mutex;

    // 内部方法
    void evictResources();
};

} // namespace Appgame

#endif // RESOURCE_H
