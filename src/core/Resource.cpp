#include "core/Resource.h"
#include <chrono>

namespace Appgame {

// Resource 类实现

Resource::Resource(const std::string& name, const std::string& path, ResourceType type)
    : m_name(name), m_path(path), m_type(type), m_status(ResourceStatus::UNLOADED) {
}

const std::string& Resource::getName() const {
    return m_name;
}

const std::string& Resource::getPath() const {
    return m_path;
}

ResourceType Resource::getType() const {
    return m_type;
}

ResourceStatus Resource::getStatus() const {
    return m_status;
}

void Resource::setStatus(ResourceStatus status) {
    m_status = status;
}

// ResourceManager 类实现

ResourceManager::ResourceManager()
    : m_running(false), m_totalMemoryUsage(0) {
}

ResourceManager::~ResourceManager() {
    cleanup();
}

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::init() {
    if (!m_running) {
        m_running = true;
        m_loadThread = std::thread(&ResourceManager::processLoadRequests, this);
    }
    return true;
}

void ResourceManager::cleanup() {
    if (m_running) {
        m_running = false;
        m_condition.notify_one();
        if (m_loadThread.joinable()) {
            m_loadThread.join();
        }
    }
    unloadAllResources();
    m_loaders.clear();
}

void ResourceManager::setLoader(ResourceType type, std::unique_ptr<ResourceLoader> loader) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_loaders[type] = std::move(loader);
}

std::shared_ptr<Resource> ResourceManager::loadResource(const std::string& path, ResourceType type) {
    // 检查资源是否已加载
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_resources.find(path);
        if (it != m_resources.end()) {
            return it->second;
        }
    }

    // 查找对应类型的加载器
    ResourceLoader* loader = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_loaders.find(type);
        if (it != m_loaders.end()) {
            loader = it->second.get();
        }
    }

    if (!loader) {
        return nullptr;
    }

    // 加载资源
    auto resource = loader->load(path, type);
    if (resource && resource->load()) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto resourcePtr = std::shared_ptr<Resource>(resource.release());
        m_resources[path] = resourcePtr;
        m_totalMemoryUsage += resourcePtr->getSize();
        return resourcePtr;
    }

    return nullptr;
}

void ResourceManager::loadResourceAsync(const std::string& path, ResourceType type, std::function<void(std::shared_ptr<Resource>)> callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // 检查资源是否已加载
    auto it = m_resources.find(path);
    if (it != m_resources.end()) {
        // 资源已加载，直接回调
        std::thread([callback, resource = it->second]() {
            callback(resource);
        }).detach();
        return;
    }

    // 添加到加载请求队列
    m_loadRequests.push_back({path, type, [this, callback](std::unique_ptr<Resource> resource) {
        std::shared_ptr<Resource> resourcePtr;
        if (resource && resource->getStatus() == ResourceStatus::LOADED) {
            std::lock_guard<std::mutex> lock(m_mutex);
            resourcePtr = std::shared_ptr<Resource>(resource.release());
            m_resources[resourcePtr->getPath()] = resourcePtr;
            m_totalMemoryUsage += resourcePtr->getSize();
        }
        callback(resourcePtr);
    }});

    m_condition.notify_one();
}

void ResourceManager::unloadResource(const std::string& path) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_resources.find(path);
    if (it != m_resources.end()) {
        m_totalMemoryUsage -= it->second->getSize();
        it->second->unload();
        m_resources.erase(it);
    }
}

void ResourceManager::unloadAllResources() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& pair : m_resources) {
        pair.second->unload();
    }
    m_resources.clear();
    m_totalMemoryUsage = 0;
}

std::shared_ptr<Resource> ResourceManager::getResource(const std::string& path) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_resources.find(path);
    if (it != m_resources.end()) {
        return it->second;
    }
    return nullptr;
}

bool ResourceManager::isResourceLoaded(const std::string& path) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_resources.find(path) != m_resources.end();
}

void ResourceManager::preloadResources(const std::vector<std::pair<std::string, ResourceType>>& resources) {
    for (const auto& pair : resources) {
        loadResource(pair.first, pair.second);
    }
}

size_t ResourceManager::getLoadedResourceCount() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_resources.size();
}

size_t ResourceManager::getTotalMemoryUsage() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_totalMemoryUsage;
}

void ResourceManager::processLoadRequests() {
    while (m_running) {
        std::vector<ResourceLoadRequest> requests;

        // 获取加载请求
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this]() {
                return !m_running || !m_loadRequests.empty();
            });

            if (!m_running) {
                break;
            }

            requests.swap(m_loadRequests);
        }

        // 处理加载请求
        for (const auto& request : requests) {
            // 查找对应类型的加载器
            ResourceLoader* loader = nullptr;
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                auto it = m_loaders.find(request.type);
                if (it != m_loaders.end()) {
                    loader = it->second.get();
                }
            }

            if (loader) {
                auto resource = loader->load(request.path, request.type);
                if (resource) {
                    resource->load();
                }
                request.callback(std::move(resource));
            } else {
                request.callback(nullptr);
            }
        }
    }
}

// ResourceCache 类实现

ResourceCache::ResourceCache()
    : m_cacheSizeLimit(1024 * 1024 * 100), // 默认 100MB
      m_currentCacheSize(0) {
}

ResourceCache::~ResourceCache() {
    clearCache();
}

ResourceCache& ResourceCache::getInstance() {
    static ResourceCache instance;
    return instance;
}

void ResourceCache::setCacheSizeLimit(size_t limit) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cacheSizeLimit = limit;
    evictResources();
}

void ResourceCache::cacheResource(const std::string& key, std::shared_ptr<Resource> resource) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // 检查资源是否已在缓存中
    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        // 更新现有缓存项
        m_currentCacheSize -= it->second.size;
        it->second.resource = resource;
        it->second.size = resource->getSize();
        it->second.lastAccessed = std::chrono::steady_clock::now();
        m_currentCacheSize += resource->getSize();
    } else {
        // 添加新缓存项
        size_t resourceSize = resource->getSize();
        
        // 检查是否需要清理缓存
        if (m_currentCacheSize + resourceSize > m_cacheSizeLimit) {
            evictResources();
        }
        
        // 如果仍然超出限制，不添加
        if (m_currentCacheSize + resourceSize <= m_cacheSizeLimit) {
            m_cache[key] = {
                key,
                resource,
                resourceSize,
                std::chrono::steady_clock::now()
            };
            m_currentCacheSize += resourceSize;
        }
    }
}

std::shared_ptr<Resource> ResourceCache::getCachedResource(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        // 更新最后访问时间
        it->second.lastAccessed = std::chrono::steady_clock::now();
        return it->second.resource;
    }
    return nullptr;
}

void ResourceCache::removeCachedResource(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        m_currentCacheSize -= it->second.size;
        m_cache.erase(it);
    }
}

void ResourceCache::clearCache() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cache.clear();
    m_currentCacheSize = 0;
}

size_t ResourceCache::getCacheSize() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_currentCacheSize;
}

size_t ResourceCache::getCacheLimit() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_cacheSizeLimit;
}

void ResourceCache::evictResources() {
    // 使用 LRU 策略清理资源
    std::vector<std::pair<std::string, std::chrono::steady_clock::time_point>> items;
    items.reserve(m_cache.size());

    for (const auto& pair : m_cache) {
        items.emplace_back(pair.first, pair.second.lastAccessed);
    }

    // 按最后访问时间排序
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // 清理资源直到满足大小限制
    for (const auto& item : items) {
        if (m_currentCacheSize <= m_cacheSizeLimit * 0.8) { // 保留 80% 的空间
            break;
        }

        auto it = m_cache.find(item.first);
        if (it != m_cache.end()) {
            m_currentCacheSize -= it->second.size;
            m_cache.erase(it);
        }
    }
}

} // namespace Appgame
