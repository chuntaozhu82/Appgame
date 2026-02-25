#include "core/Storage.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace Appgame {

// 本地存储实现（示例）
class LocalStorage : public StorageInterface {
public:
    LocalStorage() {
        // 初始化本地存储路径
        // 实际项目中需要根据平台获取正确的存储路径
    }

    bool save(const std::string& key, const std::string& value) override {
        // 实现字符串保存
        return true;
    }

    bool save(const std::string& key, int value) override {
        // 实现整数保存
        return true;
    }

    bool save(const std::string& key, float value) override {
        // 实现浮点数保存
        return true;
    }

    bool save(const std::string& key, bool value) override {
        // 实现布尔值保存
        return true;
    }

    bool save(const std::string& key, const std::vector<unsigned char>& value) override {
        // 实现二进制数据保存
        return true;
    }

    bool load(const std::string& key, std::string& value) override {
        // 实现字符串加载
        return false;
    }

    bool load(const std::string& key, int& value) override {
        // 实现整数加载
        return false;
    }

    bool load(const std::string& key, float& value) override {
        // 实现浮点数加载
        return false;
    }

    bool load(const std::string& key, bool& value) override {
        // 实现布尔值加载
        return false;
    }

    bool load(const std::string& key, std::vector<unsigned char>& value) override {
        // 实现二进制数据加载
        return false;
    }

    bool remove(const std::string& key) override {
        // 实现数据删除
        return true;
    }

    bool exists(const std::string& key) override {
        // 实现数据存在性检查
        return false;
    }

    bool clear() override {
        // 实现清除所有数据
        return true;
    }

    std::vector<std::string> getAllKeys() override {
        // 实现获取所有键
        return {};
    }
};

// 加密存储实现（示例）
class EncryptedLocalStorage : public EncryptedStorageInterface {
public:
    EncryptedLocalStorage() : m_encrypted(false) {
    }

    void setEncryptionKey(const std::string& key) override {
        m_encryptionKey = key;
        m_encrypted = !key.empty();
    }

    bool isEncrypted() const override {
        return m_encrypted;
    }

    bool save(const std::string& key, const std::string& value) override {
        // 实现加密保存
        return true;
    }

    bool save(const std::string& key, int value) override {
        // 实现加密保存
        return true;
    }

    bool save(const std::string& key, float value) override {
        // 实现加密保存
        return true;
    }

    bool save(const std::string& key, bool value) override {
        // 实现加密保存
        return true;
    }

    bool save(const std::string& key, const std::vector<unsigned char>& value) override {
        // 实现加密保存
        return true;
    }

    bool load(const std::string& key, std::string& value) override {
        // 实现加密加载
        return false;
    }

    bool load(const std::string& key, int& value) override {
        // 实现加密加载
        return false;
    }

    bool load(const std::string& key, float& value) override {
        // 实现加密加载
        return false;
    }

    bool load(const std::string& key, bool& value) override {
        // 实现加密加载
        return false;
    }

    bool load(const std::string& key, std::vector<unsigned char>& value) override {
        // 实现加密加载
        return false;
    }

    bool remove(const std::string& key) override {
        // 实现加密数据删除
        return true;
    }

    bool exists(const std::string& key) override {
        // 实现加密数据存在性检查
        return false;
    }

    bool clear() override {
        // 实现清除所有加密数据
        return true;
    }

    std::vector<std::string> getAllKeys() override {
        // 实现获取所有加密键
        return {};
    }

private:
    std::string m_encryptionKey;
    bool m_encrypted;
};

// StorageManager 类实现

StorageManager::StorageManager()
    : m_initialized(false) {
}

StorageManager::~StorageManager() {
    cleanup();
}

StorageManager& StorageManager::getInstance() {
    static StorageManager instance;
    return instance;
}

bool StorageManager::init() {
    if (!m_initialized) {
        initializeStorages();
        m_initialized = true;
    }
    return true;
}

void StorageManager::cleanup() {
    m_storages.clear();
    m_encryptedStorages.clear();
    m_initialized = false;
}

void StorageManager::initializeStorages() {
    // 初始化本地存储
    m_storages[StorageType::LOCAL] = std::make_unique<LocalStorage>();
    
    // 初始化云存储（如果需要）
    // m_storages[StorageType::CLOUD] = std::make_unique<CloudStorage>();
    
    // 初始化临时存储
    // m_storages[StorageType::TEMP] = std::make_unique<TempStorage>();
    
    // 初始化加密存储
    m_encryptedStorages[StorageType::LOCAL] = std::make_unique<EncryptedLocalStorage>();
    if (!m_encryptionKey.empty()) {
        m_encryptedStorages[StorageType::LOCAL]->setEncryptionKey(m_encryptionKey);
    }
}

StorageInterface* StorageManager::getStorage(StorageType type) {
    auto it = m_storages.find(type);
    if (it != m_storages.end()) {
        return it->second.get();
    }
    return nullptr;
}

EncryptedStorageInterface* StorageManager::getEncryptedStorage(StorageType type) {
    auto it = m_encryptedStorages.find(type);
    if (it != m_encryptedStorages.end()) {
        return it->second.get();
    }
    return nullptr;
}

bool StorageManager::save(StorageType type, const std::string& key, const std::string& value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->save(key, value);
    }
    return false;
}

bool StorageManager::save(StorageType type, const std::string& key, int value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->save(key, value);
    }
    return false;
}

bool StorageManager::save(StorageType type, const std::string& key, float value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->save(key, value);
    }
    return false;
}

bool StorageManager::save(StorageType type, const std::string& key, bool value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->save(key, value);
    }
    return false;
}

bool StorageManager::save(StorageType type, const std::string& key, const std::vector<unsigned char>& value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->save(key, value);
    }
    return false;
}

bool StorageManager::load(StorageType type, const std::string& key, std::string& value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->load(key, value);
    }
    return false;
}

bool StorageManager::load(StorageType type, const std::string& key, int& value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->load(key, value);
    }
    return false;
}

bool StorageManager::load(StorageType type, const std::string& key, float& value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->load(key, value);
    }
    return false;
}

bool StorageManager::load(StorageType type, const std::string& key, bool& value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->load(key, value);
    }
    return false;
}

bool StorageManager::load(StorageType type, const std::string& key, std::vector<unsigned char>& value) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->load(key, value);
    }
    return false;
}

std::future<bool> StorageManager::saveAsync(StorageType type, const std::string& key, const std::string& value) {
    return std::async(std::launch::async, [this, type, key, value]() {
        return save(type, key, value);
    });
}

std::future<bool> StorageManager::saveAsync(StorageType type, const std::string& key, int value) {
    return std::async(std::launch::async, [this, type, key, value]() {
        return save(type, key, value);
    });
}

std::future<bool> StorageManager::saveAsync(StorageType type, const std::string& key, float value) {
    return std::async(std::launch::async, [this, type, key, value]() {
        return save(type, key, value);
    });
}

std::future<bool> StorageManager::saveAsync(StorageType type, const std::string& key, bool value) {
    return std::async(std::launch::async, [this, type, key, value]() {
        return save(type, key, value);
    });
}

std::future<bool> StorageManager::saveAsync(StorageType type, const std::string& key, const std::vector<unsigned char>& value) {
    return std::async(std::launch::async, [this, type, key, value]() {
        return save(type, key, value);
    });
}

std::future<bool> StorageManager::loadAsync(StorageType type, const std::string& key, std::string& value) {
    return std::async(std::launch::async, [this, type, key, &value]() {
        return load(type, key, value);
    });
}

std::future<bool> StorageManager::loadAsync(StorageType type, const std::string& key, int& value) {
    return std::async(std::launch::async, [this, type, key, &value]() {
        return load(type, key, value);
    });
}

std::future<bool> StorageManager::loadAsync(StorageType type, const std::string& key, float& value) {
    return std::async(std::launch::async, [this, type, key, &value]() {
        return load(type, key, value);
    });
}

std::future<bool> StorageManager::loadAsync(StorageType type, const std::string& key, bool& value) {
    return std::async(std::launch::async, [this, type, key, &value]() {
        return load(type, key, value);
    });
}

std::future<bool> StorageManager::loadAsync(StorageType type, const std::string& key, std::vector<unsigned char>& value) {
    return std::async(std::launch::async, [this, type, key, &value]() {
        return load(type, key, value);
    });
}

bool StorageManager::remove(StorageType type, const std::string& key) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->remove(key);
    }
    return false;
}

bool StorageManager::exists(StorageType type, const std::string& key) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->exists(key);
    }
    return false;
}

bool StorageManager::clear(StorageType type) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->clear();
    }
    return false;
}

std::vector<std::string> StorageManager::getAllKeys(StorageType type) {
    StorageInterface* storage = getStorage(type);
    if (storage) {
        return storage->getAllKeys();
    }
    return {};
}

void StorageManager::setEncryptionKey(const std::string& key) {
    m_encryptionKey = key;
    
    // 更新所有加密存储的密钥
    for (auto& pair : m_encryptedStorages) {
        pair.second->setEncryptionKey(key);
    }
}

// Storage 助手类实现

StorageManager& Storage::getManager() {
    return StorageManager::getInstance();
}

bool Storage::save(const std::string& key, const std::string& value) {
    return getManager().save(StorageType::LOCAL, key, value);
}

bool Storage::save(const std::string& key, int value) {
    return getManager().save(StorageType::LOCAL, key, value);
}

bool Storage::save(const std::string& key, float value) {
    return getManager().save(StorageType::LOCAL, key, value);
}

bool Storage::save(const std::string& key, bool value) {
    return getManager().save(StorageType::LOCAL, key, value);
}

bool Storage::save(const std::string& key, const std::vector<unsigned char>& value) {
    return getManager().save(StorageType::LOCAL, key, value);
}

bool Storage::load(const std::string& key, std::string& value) {
    return getManager().load(StorageType::LOCAL, key, value);
}

bool Storage::load(const std::string& key, int& value) {
    return getManager().load(StorageType::LOCAL, key, value);
}

bool Storage::load(const std::string& key, float& value) {
    return getManager().load(StorageType::LOCAL, key, value);
}

bool Storage::load(const std::string& key, bool& value) {
    return getManager().load(StorageType::LOCAL, key, value);
}

bool Storage::load(const std::string& key, std::vector<unsigned char>& value) {
    return getManager().load(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::saveAsync(const std::string& key, const std::string& value) {
    return getManager().saveAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::saveAsync(const std::string& key, int value) {
    return getManager().saveAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::saveAsync(const std::string& key, float value) {
    return getManager().saveAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::saveAsync(const std::string& key, bool value) {
    return getManager().saveAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::saveAsync(const std::string& key, const std::vector<unsigned char>& value) {
    return getManager().saveAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::loadAsync(const std::string& key, std::string& value) {
    return getManager().loadAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::loadAsync(const std::string& key, int& value) {
    return getManager().loadAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::loadAsync(const std::string& key, float& value) {
    return getManager().loadAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::loadAsync(const std::string& key, bool& value) {
    return getManager().loadAsync(StorageType::LOCAL, key, value);
}

std::future<bool> Storage::loadAsync(const std::string& key, std::vector<unsigned char>& value) {
    return getManager().loadAsync(StorageType::LOCAL, key, value);
}

bool Storage::remove(const std::string& key) {
    return getManager().remove(StorageType::LOCAL, key);
}

bool Storage::exists(const std::string& key) {
    return getManager().exists(StorageType::LOCAL, key);
}

bool Storage::clear() {
    return getManager().clear(StorageType::LOCAL);
}

std::vector<std::string> Storage::getAllKeys() {
    return getManager().getAllKeys(StorageType::LOCAL);
}

bool Storage::saveEncrypted(const std::string& key, const std::string& value) {
    auto storage = getManager().getEncryptedStorage(StorageType::LOCAL);
    if (storage) {
        return storage->save(key, value);
    }
    return false;
}

bool Storage::loadEncrypted(const std::string& key, std::string& value) {
    auto storage = getManager().getEncryptedStorage(StorageType::LOCAL);
    if (storage) {
        return storage->load(key, value);
    }
    return false;
}

} // namespace Appgame
