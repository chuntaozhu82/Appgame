#include "fishing/core/Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace FishingGame {

// ConfigValueInt implementation
ConfigValueInt::ConfigValueInt(int32 value)
    : m_value(value)
{}

ConfigValueType ConfigValueInt::getType() const {
    return ConfigValueType::INT;
}

std::string ConfigValueInt::toString() const {
    return std::to_string(m_value);
}

int32 ConfigValueInt::getValue() const {
    return m_value;
}

// ConfigValueFloat implementation
ConfigValueFloat::ConfigValueFloat(float32 value)
    : m_value(value)
{}

ConfigValueType ConfigValueFloat::getType() const {
    return ConfigValueType::FLOAT;
}

std::string ConfigValueFloat::toString() const {
    return std::to_string(m_value);
}

float32 ConfigValueFloat::getValue() const {
    return m_value;
}

// ConfigValueBool implementation
ConfigValueBool::ConfigValueBool(bool value)
    : m_value(value)
{}

ConfigValueType ConfigValueBool::getType() const {
    return ConfigValueType::BOOL;
}

std::string ConfigValueBool::toString() const {
    return m_value ? "true" : "false";
}

bool ConfigValueBool::getValue() const {
    return m_value;
}

// ConfigValueString implementation
ConfigValueString::ConfigValueString(const std::string& value)
    : m_value(value)
{}

ConfigValueType ConfigValueString::getType() const {
    return ConfigValueType::STRING;
}

std::string ConfigValueString::toString() const {
    return m_value;
}

const std::string& ConfigValueString::getValue() const {
    return m_value;
}

// ConfigValueVector implementation
ConfigValueVector::ConfigValueVector(const std::vector<float32>& value)
    : m_value(value)
{}

ConfigValueType ConfigValueVector::getType() const {
    return ConfigValueType::VECTOR;
}

std::string ConfigValueVector::toString() const {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < m_value.size(); ++i) {
        if (i > 0) {
            ss << ", ";
        }
        ss << m_value[i];
    }
    ss << "]";
    return ss.str();
}

const std::vector<float32>& ConfigValueVector::getValue() const {
    return m_value;
}

// Config implementation
Config::Config() {
}

Config::~Config() {
    clear();
}

bool Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#') {
            continue;
        }
        parseLine(line);
    }

    file.close();
    return true;
}

bool Config::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing: " << filename << std::endl;
        return false;
    }

    file << "# Fishing Game Configuration" << std::endl;
    file << "# Last modified: " << __DATE__ << " " << __TIME__ << std::endl;
    file << std::endl;

    for (const auto& pair : m_values) {
        const std::string& key = pair.first;
        ConfigValue* value = pair.second;
        file << key << " = " << value->toString() << std::endl;
    }

    file.close();
    return true;
}

void Config::set(const std::string& key, int32 value) {
    if (m_values.find(key) != m_values.end()) {
        delete m_values[key];
    }
    m_values[key] = new ConfigValueInt(value);
}

void Config::set(const std::string& key, float32 value) {
    if (m_values.find(key) != m_values.end()) {
        delete m_values[key];
    }
    m_values[key] = new ConfigValueFloat(value);
}

void Config::set(const std::string& key, bool value) {
    if (m_values.find(key) != m_values.end()) {
        delete m_values[key];
    }
    m_values[key] = new ConfigValueBool(value);
}

void Config::set(const std::string& key, const std::string& value) {
    if (m_values.find(key) != m_values.end()) {
        delete m_values[key];
    }
    m_values[key] = new ConfigValueString(value);
}

void Config::set(const std::string& key, const std::vector<float32>& value) {
    if (m_values.find(key) != m_values.end()) {
        delete m_values[key];
    }
    m_values[key] = new ConfigValueVector(value);
}

int32 Config::getInt(const std::string& key, int32 defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end() && it->second->getType() == ConfigValueType::INT) {
        return static_cast<ConfigValueInt*>(it->second)->getValue();
    }
    return defaultValue;
}

float32 Config::getFloat(const std::string& key, float32 defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end() && it->second->getType() == ConfigValueType::FLOAT) {
        return static_cast<ConfigValueFloat*>(it->second)->getValue();
    }
    return defaultValue;
}

bool Config::getBool(const std::string& key, bool defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end() && it->second->getType() == ConfigValueType::BOOL) {
        return static_cast<ConfigValueBool*>(it->second)->getValue();
    }
    return defaultValue;
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end() && it->second->getType() == ConfigValueType::STRING) {
        return static_cast<ConfigValueString*>(it->second)->getValue();
    }
    return defaultValue;
}

std::vector<float32> Config::getVector(const std::string& key, const std::vector<float32>& defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end() && it->second->getType() == ConfigValueType::VECTOR) {
        return static_cast<ConfigValueVector*>(it->second)->getValue();
    }
    return defaultValue;
}

bool Config::hasKey(const std::string& key) const {
    return m_values.find(key) != m_values.end();
}

void Config::remove(const std::string& key) {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        delete it->second;
        m_values.erase(it);
    }
}

void Config::clear() {
    for (auto& pair : m_values) {
        delete pair.second;
    }
    m_values.clear();
}

void Config::print() const {
    std::cout << "=== Configuration ===" << std::endl;
    for (const auto& pair : m_values) {
        const std::string& key = pair.first;
        ConfigValue* value = pair.second;
        std::cout << key << " = " << value->toString() << std::endl;
    }
    std::cout << "====================" << std::endl;
}

void Config::parseLine(const std::string& line) {
    // 查找等号
    size_t equalsPos = line.find('=');
    if (equalsPos == std::string::npos) {
        return;
    }

    // 提取键
    std::string key = line.substr(0, equalsPos);
    // 去除首尾空格
    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);

    // 提取值
    std::string valueStr = line.substr(equalsPos + 1);
    // 去除首尾空格
    valueStr.erase(0, valueStr.find_first_not_of(" \t"));
    valueStr.erase(valueStr.find_last_not_of(" \t") + 1);

    // 尝试解析不同类型的值
    // 1. 尝试解析布尔值
    if (valueStr == "true" || valueStr == "false") {
        set(key, valueStr == "true");
        return;
    }

    // 2. 尝试解析整型
    try {
        size_t pos;
        int32 intValue = std::stoi(valueStr, &pos);
        if (pos == valueStr.size()) {
            set(key, intValue);
            return;
        }
    } catch (...) {
    }

    // 3. 尝试解析浮点型
    try {
        size_t pos;
        float32 floatValue = std::stof(valueStr, &pos);
        if (pos == valueStr.size()) {
            set(key, floatValue);
            return;
        }
    } catch (...) {
    }

    // 4. 默认为字符串
    set(key, valueStr);
}

ConfigValue* Config::parseValue(const std::string& valueStr, ConfigValueType type) {
    switch (type) {
        case ConfigValueType::INT:
            try {
                return new ConfigValueInt(std::stoi(valueStr));
            } catch (...) {
                return new ConfigValueInt(0);
            }
        case ConfigValueType::FLOAT:
            try {
                return new ConfigValueFloat(std::stof(valueStr));
            } catch (...) {
                return new ConfigValueFloat(0.0f);
            }
        case ConfigValueType::BOOL:
            return new ConfigValueBool(valueStr == "true" || valueStr == "1");
        case ConfigValueType::STRING:
            return new ConfigValueString(valueStr);
        case ConfigValueType::VECTOR:
            // 解析向量格式 [1.0, 2.0, 3.0]
            std::vector<float32> vec;
            std::string vecStr = valueStr;
            // 去除首尾括号
            if (!vecStr.empty() && vecStr.front() == '[') {
                vecStr.erase(0, 1);
            }
            if (!vecStr.empty() && vecStr.back() == ']') {
                vecStr.pop_back();
            }
            // 分割逗号
            std::stringstream ss(vecStr);
            std::string token;
            while (std::getline(ss, token, ',')) {
                // 去除空格
                token.erase(0, token.find_first_not_of(" \t"));
                token.erase(token.find_last_not_of(" \t") + 1);
                try {
                    vec.push_back(std::stof(token));
                } catch (...) {
                }
            }
            return new ConfigValueVector(vec);
        default:
            return nullptr;
    }
}

// Global config instance
Config* g_config = nullptr;

} // namespace FishingGame
