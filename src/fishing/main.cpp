#include "fishing/platform/Platform.h"
#include "fishing/ui/UIManager.h"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Starting Fishing Game..." << std::endl;
    
    // 初始化平台
    FishingGame::g_platform = FishingGame::PlatformFactory::createPlatform();
    
    if (!FishingGame::g_platform) {
        std::cerr << "Failed to create platform" << std::endl;
        return 1;
    }
    
    // 初始化平台
    if (!FishingGame::g_platform->init()) {
        std::cerr << "Failed to initialize platform" << std::endl;
        delete FishingGame::g_platform;
        return 1;
    }
    
    // 获取平台信息
    std::cout << "Platform: " << FishingGame::g_platform->getPlatformName() << " " << FishingGame::g_platform->getPlatformVersion() << std::endl;
    
    // 获取屏幕信息
    auto screenInfo = FishingGame::g_platform->getScreenInfo();
    std::cout << "Screen: " << screenInfo.width << "x" << screenInfo.height << " (" << screenInfo.aspectRatio << ")" << std::endl;
    std::cout << "DPI: " << screenInfo.dpi << std::endl;
    
    // 获取设备信息
    std::string deviceTypeStr;
    switch (screenInfo.deviceType) {
        case FishingGame::DeviceType::DESKTOP:
            deviceTypeStr = "Desktop";
            break;
        case FishingGame::DeviceType::LAPTOP:
            deviceTypeStr = "Laptop";
            break;
        case FishingGame::DeviceType::TABLET:
            deviceTypeStr = "Tablet";
            break;
        case FishingGame::DeviceType::SMARTPHONE:
            deviceTypeStr = "Smartphone";
            break;
    }
    std::cout << "Device Type: " << deviceTypeStr << std::endl;
    
    // 获取输入信息
    std::string inputTypeStr;
    switch (screenInfo.inputType) {
        case FishingGame::InputType::MOUSE:
            inputTypeStr = "Mouse";
            break;
        case FishingGame::InputType::TOUCH:
            inputTypeStr = "Touch";
            break;
        case FishingGame::InputType::BOTH:
            inputTypeStr = "Both";
            break;
    }
    std::cout << "Input Type: " << inputTypeStr << std::endl;
    
    // 获取缩放因子
    std::cout << "Scale Factor: " << FishingGame::g_platform->getScaleFactor() << std::endl;
    std::cout << "UI Scale Factor: " << FishingGame::g_platform->getUIScaleFactor() << std::endl;
    
    // 初始化UI管理器
    std::cout << "Initializing UI Manager..." << std::endl;
    FishingGame::g_uiManager = new FishingGame::UIManager();
    FishingGame::g_uiManager->setPlatform(FishingGame::g_platform);
    FishingGame::g_uiManager->setScreenSize(screenInfo.width, screenInfo.height);
    FishingGame::g_uiManager->init();
    
    // 运行游戏主循环
    std::cout << "Running game loop..." << std::endl;
    
    // 模拟游戏运行
    bool running = true;
    int32 frameCount = 0;
    while (running && frameCount < 100) { // 只运行100帧，避免无限循环
        // 运行平台消息循环
        if (!FishingGame::g_platform->runMessageLoop()) {
            running = false;
        }
        
        // 模拟游戏逻辑
        // TODO: 实现游戏主逻辑
        
        // 更新UI管理器
        FishingGame::g_uiManager->update(0.016f); // 约60FPS
        
        // 渲染UI管理器
        FishingGame::g_uiManager->render();
        
        // 短暂睡眠，避免CPU占用过高
        FishingGame::g_platform->sleep(16); // 约60FPS
        
        // 增加帧计数
        frameCount++;
    }
    
    // 清理UI管理器
    std::cout << "Cleaning up UI Manager..." << std::endl;
    FishingGame::g_uiManager->cleanup();
    delete FishingGame::g_uiManager;
    
    // 清理平台
    std::cout << "Cleaning up platform..." << std::endl;
    FishingGame::g_platform->cleanup();
    delete FishingGame::g_platform;
    
    std::cout << "Fishing Game exited." << std::endl;
    return 0;
}
