#include "fishing/test/TestFramework.h"
#include "fishing/core/Config.h"
#include <fstream>

using namespace FishingGame;

TEST_SUITE(Config) {

TEST(Config, CreateConfig) {
    Config config;
    ASSERT_TRUE(true);
}

TEST(Config, SetAndGetString) {
    Config config;
    config.setValue("test_string", "hello world");
    
    std::string value;
    bool result = config.getValue("test_string", value);
    
    ASSERT_TRUE(result);
    ASSERT_EQ("hello world", value);
}

TEST(Config, SetAndGetInteger) {
    Config config;
    config.setValue("test_int", 42);
    
    int32 value;
    bool result = config.getValue("test_int", value);
    
    ASSERT_TRUE(result);
    ASSERT_EQ(42, value);
}

TEST(Config, SetAndGetFloat) {
    Config config;
    config.setValue("test_float", 3.14f);
    
    float32 value;
    bool result = config.getValue("test_float", value);
    
    ASSERT_TRUE(result);
    ASSERT_NEAR(3.14f, value, 0.001f);
}

TEST(Config, SetAndGetBool) {
    Config config;
    config.setValue("test_bool_true", true);
    config.setValue("test_bool_false", false);
    
    bool valueTrue;
    bool resultTrue = config.getValue("test_bool_true", valueTrue);
    
    ASSERT_TRUE(resultTrue);
    ASSERT_TRUE(valueTrue);
    
    bool valueFalse;
    bool resultFalse = config.getValue("test_bool_false", valueFalse);
    
    ASSERT_TRUE(resultFalse);
    ASSERT_FALSE(valueFalse);
}

TEST(Config, GetNonExistentValue) {
    Config config;
    
    std::string value;
    bool result = config.getValue("non_existent", value);
    
    ASSERT_FALSE(result);
}

TEST(Config, HasValue) {
    Config config;
    config.setValue("test_key", "test_value");
    
    ASSERT_TRUE(config.hasValue("test_key"));
    ASSERT_FALSE(config.hasValue("non_existent"));
}

TEST(Config, RemoveValue) {
    Config config;
    config.setValue("test_key", "test_value");
    
    ASSERT_TRUE(config.hasValue("test_key"));
    
    config.removeValue("test_key");
    
    ASSERT_FALSE(config.hasValue("test_key"));
}

TEST(Config, Clear) {
    Config config;
    config.setValue("key1", "value1");
    config.setValue("key2", "value2");
    config.setValue("key3", "value3");
    
    ASSERT_TRUE(config.hasValue("key1"));
    ASSERT_TRUE(config.hasValue("key2"));
    ASSERT_TRUE(config.hasValue("key3"));
    
    config.clear();
    
    ASSERT_FALSE(config.hasValue("key1"));
    ASSERT_FALSE(config.hasValue("key2"));
    ASSERT_FALSE(config.hasValue("key3"));
}

TEST(Config, SaveAndLoad) {
    Config config;
    config.setValue("string_key", "test_string");
    config.setValue("int_key", 123);
    config.setValue("float_key", 45.67f);
    config.setValue("bool_key", true);
    
    bool saveResult = config.saveToFile("test_config.txt");
    ASSERT_TRUE(saveResult);
    
    Config loadedConfig;
    bool loadResult = loadedConfig.loadFromFile("test_config.txt");
    ASSERT_TRUE(loadResult);
    
    std::string stringValue;
    ASSERT_TRUE(loadedConfig.getValue("string_key", stringValue));
    ASSERT_EQ("test_string", stringValue);
    
    int32 intValue;
    ASSERT_TRUE(loadedConfig.getValue("int_key", intValue));
    ASSERT_EQ(123, intValue);
    
    float32 floatValue;
    ASSERT_TRUE(loadedConfig.getValue("float_key", floatValue));
    ASSERT_NEAR(45.67f, floatValue, 0.01f);
    
    bool boolValue;
    ASSERT_TRUE(loadedConfig.getValue("bool_key", boolValue));
    ASSERT_TRUE(boolValue);
    
    std::remove("test_config.txt");
}

TEST(Config, OverwriteValue) {
    Config config;
    config.setValue("test_key", "original");
    
    std::string value;
    config.getValue("test_key", value);
    ASSERT_EQ("original", value);
    
    config.setValue("test_key", "updated");
    
    config.getValue("test_key", value);
    ASSERT_EQ("updated", value);
}

TEST(Config, GetKeys) {
    Config config;
    config.setValue("key1", "value1");
    config.setValue("key2", "value2");
    config.setValue("key3", "value3");
    
    std::vector<std::string> keys = config.getKeys();
    
    ASSERT_EQ(3, keys.size());
    
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    for (const auto& key : keys) {
        if (key == "key1") hasKey1 = true;
        if (key == "key2") hasKey2 = true;
        if (key == "key3") hasKey3 = true;
    }
    
    ASSERT_TRUE(hasKey1);
    ASSERT_TRUE(hasKey2);
    ASSERT_TRUE(hasKey3);
}

}
