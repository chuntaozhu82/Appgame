#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <future>

namespace Appgame {

// 存储类型
enum class StorageType {
    LOCAL,       // 本地存储
    CLOUD,       // 云存储
    TEMP         // 临时存储
};

// 存储接口抽象类
class StorageInterface {
public:
    virtual ~StorageInterface() = default;

    // 保存数据
    virtual bool save(const std::string& key, const std::string& value) = 0;
    virtual bool save(const std::string& key, int value) = 0;
    virtual bool save(const std::string& key, float value) = 0;
    virtual bool save(const std::string& key, bool value) = 0;
    virtual bool save(const std::string& key, const std::vector<unsigned char>& value) = 0;

    // 加载数据
    virtual bool load(const std::string& key, std::string& value) = 0;
    virtual bool load(const std::string& key, int& value) = 0;
    virtual bool load(const std::string& key, float& value) = 0;
    virtual bool load(const std::string& key, bool& value) = 0;
    virtual bool load(const std::string& key, std::vector<unsigned char>& value) = 0;

    // 删除数据
    virtual bool remove(const std::string& key) = 0;

    // 检查数据是否存在
    virtual bool exists(const std::string& key) = 0;

    // 清除所有数据
    virtual bool clear() = 0;

    // 获取所有键
    virtual std::vector<std::string> getAllKeys() = 0;
};

// 加密存储接口
class EncryptedStorageInterface : public StorageInterface {
public:
    virtual ~EncryptedStorageInterface() = default;

    // 设置加密密钥
    virtual void setEncryptionKey(const std::string& key) = 0;

    // 检查是否已加密
    virtual bool isEncrypted() const = 0;
};

// 存储管理器类
class StorageManager {
public:
    static StorageManager& getInstance();

    // 初始化存储系统
    bool init();

    // 清理存储系统
    void cleanup();

    // 获取存储接口
    StorageInterface* getStorage(StorageType type = StorageType::LOCAL);

    // 获取加密存储接口
    EncryptedStorageInterface* getEncryptedStorage(StorageType type = StorageType::LOCAL);

    // 保存数据（同步）
    bool save(StorageType type, const std::string& key, const std::string& value);
    bool save(StorageType type, const std::string& key, int value);
    bool save(StorageType type, const std::string& key, float value);
    bool save(StorageType type, const std::string& key, bool value);
    bool save(StorageType type, const std::string& key, const std::vector<unsigned char>& value);

    // 加载数据（同步）
    bool load(StorageType type, const std::string& key, std::string& value);
    bool load(StorageType type, const std::string& key, int& value);
    bool load(StorageType type, const std::string& key, float& value);
    bool load(StorageType type, const std::string& key, bool& value);
    bool load(StorageType type, const std::string& key, std::vector<unsigned char>& value);

    // 保存数据（异步）
    std::future<bool> saveAsync(StorageType type, const std::string& key, const std::string& value);
    std::future<bool> saveAsync(StorageType type, const std::string& key, int value);
    std::future<bool> saveAsync(StorageType type, const std::string& key, float value);
    std::future<bool> saveAsync(StorageType type, const std::string& key, bool value);
    std::future<bool> saveAsync(StorageType type, const std::string& key, const std::vector<unsigned char>& value);

    // 加载数据（异步）
    std::future<bool> loadAsync(StorageType type, const std::string& key, std::string& value);
    std::future<bool> loadAsync(StorageType type, const std::string& key, int& value);
    std::future<bool> loadAsync(StorageType type, const std::string& key, float& value);
    std::future<bool> loadAsync(StorageType type, const std::string& key, bool& value);
    std::future<bool> loadAsync(StorageType type, const std::string& key, std::vector<unsigned char>& value);

    // 删除数据
    bool remove(StorageType type, const std::string& key);

    // 检查数据是否存在
    bool exists(StorageType type, const std::string& key);

    // 清除所有数据
    bool clear(StorageType type);

    // 获取所有键
    std::vector<std::string> getAllKeys(StorageType type);

    // 设置加密密钥
    void setEncryptionKey(const std::string& key);

private:
    StorageManager();
    ~StorageManager();

    // 内部方法
    void initializeStorages();

    // 存储接口映射
    std::unordered_map<StorageType, std::unique_ptr<StorageInterface>> m_storages;
    std::unordered_map<StorageType, std::unique_ptr<EncryptedStorageInterface>> m_encryptedStorages;

    // 加密密钥
    std::string m_encryptionKey;

    // 初始化状态
    bool m_initialized;
};

// 数据存储助手类（单例模式，提供便捷方法）
class Storage {
public:
    // 保存数据
    static bool save(const std::string& key, const std::string& value);
    static bool save(const std::string& key, int value);
    static bool save(const std::string& key, float value);
    static bool save(const std::string& key, bool value);
    static bool save(const std::string& key, const std::vector<unsigned char>& value);

    // 加载数据
    static bool load(const std::string& key, std::string& value);
    static bool load(const std::string& key, int& value);
    static bool load(const std::string& key, float& value);
    static bool load(const std::string& key, bool& value);
    static bool load(const std::string& key, std::vector<unsigned char>& value);

    // 保存数据（异步）
    static std::future<bool> saveAsync(const std::string& key, const std::string& value);
    static std::future<bool> saveAsync(const std::string& key, int value);
    static std::future<bool> saveAsync(const std::string& key, float value);
    static std::future<bool> saveAsync(const std::string& key, bool value);
    static std::future<bool> saveAsync(const std::string& key, const std::vector<unsigned char>& value);

    // 加载数据（异步）
    static std::future<bool> loadAsync(const std::string& key, std::string& value);
    static std::future<bool> loadAsync(const std::string& key, int& value);
    static std::future<bool> loadAsync(const std::string& key, float& value);
    static std::future<bool> loadAsync(const std::string& key, bool& value);
    static std::future<bool> loadAsync(const std::string& key, std::vector<unsigned char>& value);

    // 删除数据
    static bool remove(const std::string& key);

    // 检查数据是否存在
    static bool exists(const std::string& key);

    // 清除所有数据
    static bool clear();

    // 获取所有键
    static std::vector<std::string> getAllKeys();

    // 加密存储
    static bool saveEncrypted(const std::string& key, const std::string& value);
    static bool loadEncrypted(const std::string& key, std::string& value);

private:
    static StorageManager& getManager();
};

} // namespace Appgame

#endif // STORAGE_H
