#ifndef CONFIG_H
#define CONFIG_H

#include "fishing/core/Types.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace FishingGame {

// 配置值类型
enum class ConfigValueType {
    INT,
    FLOAT,
    BOOL,
    STRING,
    VECTOR
};

// 配置值基类
class ConfigValue {
public:
    virtual ~ConfigValue() = default;
    virtual ConfigValueType getType() const = 0;
    virtual std::string toString() const = 0;
};

// 整型配置值
class ConfigValueInt : public ConfigValue {
public:
    ConfigValueInt(int32 value);
    ConfigValueType getType() const override;
    std::string toString() const override;
    int32 getValue() const;
private:
    int32 m_value;
};

// 浮点型配置值
class ConfigValueFloat : public ConfigValue {
public:
    ConfigValueFloat(float32 value);
    ConfigValueType getType() const override;
    std::string toString() const override;
    float32 getValue() const;
private:
    float32 m_value;
};

// 布尔型配置值
class ConfigValueBool : public ConfigValue {
public:
    ConfigValueBool(bool value);
    ConfigValueType getType() const override;
    std::string toString() const override;
    bool getValue() const;
private:
    bool m_value;
};

// 字符串配置值
class ConfigValueString : public ConfigValue {
public:
    ConfigValueString(const std::string& value);
    ConfigValueType getType() const override;
    std::string toString() const override;
    const std::string& getValue() const;
private:
    std::string m_value;
};

// 向量配置值
class ConfigValueVector : public ConfigValue {
public:
    ConfigValueVector(const std::vector<float32>& value);
    ConfigValueType getType() const override;
    std::string toString() const override;
    const std::vector<float32>& getValue() const;
private:
    std::vector<float32> m_value;
};

// 配置系统类
class Config {
public:
    Config();
    ~Config();

    // 加载配置文件
    bool load(const std::string& filename);

    // 保存配置文件
    bool save(const std::string& filename) const;

    // 设置配置值
    void set(const std::string& key, int32 value);
    void set(const std::string& key, float32 value);
    void set(const std::string& key, bool value);
    void set(const std::string& key, const std::string& value);
    void set(const std::string& key, const std::vector<float32>& value);

    // 获取配置值
    int32 getInt(const std::string& key, int32 defaultValue = 0) const;
    float32 getFloat(const std::string& key, float32 defaultValue = 0.0f) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    std::vector<float32> getVector(const std::string& key, const std::vector<float32>& defaultValue = {}) const;

    // 检查配置键是否存在
    bool hasKey(const std::string& key) const;

    // 移除配置键
    void remove(const std::string& key);

    // 清空所有配置
    void clear();

    // 打印所有配置
    void print() const;

private:
    // 配置数据
    std::map<std::string, ConfigValue*> m_values;

    // 解析配置文件行
    void parseLine(const std::string& line);

    // 解析配置值
    ConfigValue* parseValue(const std::string& valueStr, ConfigValueType type);
};

// 全局配置实例
extern Config* g_config;

} // namespace FishingGame

#endif // CONFIG_H
