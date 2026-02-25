#include "fishing/test/TestFramework.h"
#include "fishing/ui/UIManager.h"
#include "fishing/platform/Platform.h"

using namespace FishingGame;

TEST_SUITE(UIManager) {

TEST(UIManager, CreateUIManager) {
    UIManager uiManager;
    ASSERT_TRUE(true);
}

TEST(UIManager, InitializeUIManager) {
    UIManager uiManager;
    bool result = uiManager.init();
    ASSERT_TRUE(result);
    
    uiManager.cleanup();
}

TEST(UIManager, GetHUD) {
    UIManager uiManager;
    uiManager.init();
    
    HUD* hud = uiManager.getHUD();
    ASSERT_NOT_NULL(hud);
    
    uiManager.cleanup();
}

TEST(UIManager, GetInventoryUI) {
    UIManager uiManager;
    uiManager.init();
    
    InventoryUI* inventoryUI = uiManager.getInventoryUI();
    ASSERT_NOT_NULL(inventoryUI);
    
    uiManager.cleanup();
}

TEST(UIManager, GetShopUI) {
    UIManager uiManager;
    uiManager.init();
    
    ShopUI* shopUI = uiManager.getShopUI();
    ASSERT_NOT_NULL(shopUI);
    
    uiManager.cleanup();
}

TEST(UIManager, ShowHideHUD) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.showHUD();
    HUD* hud = uiManager.getHUD();
    ASSERT_TRUE(hud->isVisible());
    
    uiManager.hideHUD();
    ASSERT_FALSE(hud->isVisible());
    
    uiManager.cleanup();
}

TEST(UIManager, ShowHideInventoryUI) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.showInventoryUI();
    InventoryUI* inventoryUI = uiManager.getInventoryUI();
    ASSERT_TRUE(inventoryUI->isVisible());
    
    uiManager.hideInventoryUI();
    ASSERT_FALSE(inventoryUI->isVisible());
    
    uiManager.cleanup();
}

TEST(UIManager, ShowHideShopUI) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.showShopUI();
    ShopUI* shopUI = uiManager.getShopUI();
    ASSERT_TRUE(shopUI->isVisible());
    
    uiManager.hideShopUI();
    ASSERT_FALSE(shopUI->isVisible());
    
    uiManager.cleanup();
}

TEST(UIManager, SetPlatform) {
    UIManager uiManager;
    uiManager.init();
    
    Platform* platform = PlatformFactory::createPlatform();
    platform->init();
    
    uiManager.setPlatform(platform);
    
    Platform* retrievedPlatform = uiManager.getPlatform();
    ASSERT_EQ(platform, retrievedPlatform);
    
    uiManager.cleanup();
    platform->cleanup();
    delete platform;
}

TEST(UIManager, SetScreenSize) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.setScreenSize(1920, 1080);
    
    int32 width, height;
    uiManager.getScreenSize(width, height);
    
    ASSERT_EQ(1920, width);
    ASSERT_EQ(1080, height);
    
    uiManager.cleanup();
}

TEST(UIManager, Resize) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.resize(1280, 720);
    
    int32 width, height;
    uiManager.getScreenSize(width, height);
    
    ASSERT_EQ(1280, width);
    ASSERT_EQ(720, height);
    
    uiManager.cleanup();
}

TEST(UIManager, GetUIScaleFactor) {
    UIManager uiManager;
    uiManager.init();
    
    float scaleFactor = uiManager.getUIScaleFactor();
    ASSERT_TRUE(scaleFactor > 0);
    
    uiManager.cleanup();
}

TEST(UIManager, SetUIScaleFactor) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.setUIScaleFactor(1.5f);
    
    float scaleFactor = uiManager.getUIScaleFactor();
    ASSERT_NEAR(1.5f, scaleFactor, 0.001f);
    
    uiManager.cleanup();
}

TEST(UIManager, GetInputType) {
    UIManager uiManager;
    uiManager.init();
    
    InputType inputType = uiManager.getInputType();
    ASSERT_TRUE(inputType == InputType::MOUSE ||
                inputType == InputType::TOUCH ||
                inputType == InputType::BOTH);
    
    uiManager.cleanup();
}

TEST(UIManager, SetInputType) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.setInputType(InputType::TOUCH);
    
    InputType inputType = uiManager.getInputType();
    ASSERT_EQ(InputType::TOUCH, inputType);
    
    uiManager.cleanup();
}

TEST(UIManager, GetDeviceType) {
    UIManager uiManager;
    uiManager.init();
    
    DeviceType deviceType = uiManager.getDeviceType();
    ASSERT_TRUE(deviceType == DeviceType::DESKTOP ||
                deviceType == DeviceType::LAPTOP ||
                deviceType == DeviceType::TABLET ||
                deviceType == DeviceType::SMARTPHONE);
    
    uiManager.cleanup();
}

TEST(UIManager, SetDeviceType) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.setDeviceType(DeviceType::SMARTPHONE);
    
    DeviceType deviceType = uiManager.getDeviceType();
    ASSERT_EQ(DeviceType::SMARTPHONE, deviceType);
    
    uiManager.cleanup();
}

TEST(UIManager, CreateButton) {
    UIManager uiManager;
    uiManager.init();
    
    UIElement* button = uiManager.createButton("testButton", "Click Me", 100, 100, 200, 50);
    ASSERT_NOT_NULL(button);
    
    ASSERT_EQ(UIElementType::BUTTON, button->getType());
    ASSERT_EQ("testButton", button->getName());
    
    uiManager.cleanup();
}

TEST(UIManager, CreateLabel) {
    UIManager uiManager;
    uiManager.init();
    
    UIElement* label = uiManager.createLabel("testLabel", "Hello", 100, 100, 16);
    ASSERT_NOT_NULL(label);
    
    ASSERT_EQ(UIElementType::LABEL, label->getType());
    ASSERT_EQ("testLabel", label->getName());
    
    uiManager.cleanup();
}

TEST(UIManager, CreatePanel) {
    UIManager uiManager;
    uiManager.init();
    
    UIElement* panel = uiManager.createPanel("testPanel", 100, 100, 300, 200);
    ASSERT_NOT_NULL(panel);
    
    ASSERT_EQ(UIElementType::PANEL, panel->getType());
    ASSERT_EQ("testPanel", panel->getName());
    
    uiManager.cleanup();
}

TEST(UIManager, AddUIElement) {
    UIManager uiManager;
    uiManager.init();
    
    UIElement* button = uiManager.createButton("testButton", "Click Me", 100, 100, 200, 50);
    bool result = uiManager.addUIElement(button);
    
    ASSERT_TRUE(result);
    
    UIElement* retrieved = uiManager.getUIElement("testButton");
    ASSERT_EQ(button, retrieved);
    
    uiManager.cleanup();
}

TEST(UIManager, RemoveUIElement) {
    UIManager uiManager;
    uiManager.init();
    
    UIElement* button = uiManager.createButton("testButton", "Click Me", 100, 100, 200, 50);
    uiManager.addUIElement(button);
    
    uiManager.removeUIElement(button);
    
    UIElement* retrieved = uiManager.getUIElement("testButton");
    ASSERT_NULL(retrieved);
    
    uiManager.cleanup();
}

TEST(UIManager, Update) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.update(0.016f);
    
    ASSERT_TRUE(true);
    
    uiManager.cleanup();
}

TEST(UIManager, Render) {
    UIManager uiManager;
    uiManager.init();
    
    uiManager.render();
    
    ASSERT_TRUE(true);
    
    uiManager.cleanup();
}

}
