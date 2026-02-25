#include "fishing/test/TestFramework.h"
#include "fishing/platform/Platform.h"
#include "fishing/platform/default/DefaultPlatform.h"

using namespace FishingGame;

TEST_SUITE(Platform) {

TEST(Platform, CreatePlatform) {
    Platform* platform = PlatformFactory::createPlatform();
    ASSERT_NOT_NULL(platform);
    delete platform;
}

TEST(Platform, InitializePlatform) {
    Platform* platform = PlatformFactory::createPlatform();
    ASSERT_NOT_NULL(platform);
    
    bool result = platform->init();
    ASSERT_TRUE(result);
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, GetScreenInfo) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    ScreenInfo screenInfo = platform->getScreenInfo();
    
    ASSERT_TRUE(screenInfo.width > 0);
    ASSERT_TRUE(screenInfo.height > 0);
    ASSERT_TRUE(screenInfo.aspectRatio > 0);
    ASSERT_TRUE(screenInfo.dpi > 0);
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, GetDeviceType) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    DeviceType deviceType = platform->getDeviceType();
    
    ASSERT_TRUE(deviceType == DeviceType::DESKTOP ||
                deviceType == DeviceType::LAPTOP ||
                deviceType == DeviceType::TABLET ||
                deviceType == DeviceType::SMARTPHONE);
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, GetInputType) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    InputType inputType = platform->getInputType();
    
    ASSERT_TRUE(inputType == InputType::MOUSE ||
                inputType == InputType::TOUCH ||
                inputType == InputType::BOTH);
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, GetScaleFactor) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    float scaleFactor = platform->getScaleFactor();
    ASSERT_TRUE(scaleFactor > 0);
    
    float uiScaleFactor = platform->getUIScaleFactor();
    ASSERT_TRUE(uiScaleFactor > 0);
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, SetFullscreen) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    bool result = platform->setFullscreen(true);
    ASSERT_TRUE(result);
    ASSERT_TRUE(platform->isFullscreen());
    
    result = platform->setFullscreen(false);
    ASSERT_TRUE(result);
    ASSERT_FALSE(platform->isFullscreen());
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, GetPlatformName) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    std::string name = platform->getPlatformName();
    ASSERT_FALSE(name.empty());
    
    std::string version = platform->getPlatformVersion();
    ASSERT_FALSE(version.empty());
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, GetTime) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    unsigned long time1 = platform->getTime();
    platform->sleep(10);
    unsigned long time2 = platform->getTime();
    
    ASSERT_TRUE(time2 >= time1);
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, HasTouchSupport) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    InputType inputType = platform->getInputType();
    bool hasTouch = platform->hasTouchSupport();
    
    if (inputType == InputType::TOUCH || inputType == InputType::BOTH) {
        ASSERT_TRUE(hasTouch);
    } else {
        ASSERT_FALSE(hasTouch);
    }
    
    platform->cleanup();
    delete platform;
}

TEST(Platform, HasMouseSupport) {
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    InputType inputType = platform->getInputType();
    bool hasMouse = platform->hasMouseSupport();
    
    if (inputType == InputType::MOUSE || inputType == InputType::BOTH) {
        ASSERT_TRUE(hasMouse);
    } else {
        ASSERT_FALSE(hasMouse);
    }
    
    platform->cleanup();
    delete platform;
}

}
