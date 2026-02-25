#include "core/GameLoop.h"
#include "core/Graphics.h"
#include "core/Input.h"
#include "core/Resource.h"
#include "core/Physics.h"
#include "core/Audio.h"
#include "core/Storage.h"
#include "core/Log.h"

#include <iostream>

using namespace Appgame;

class HelloWorldGame : public GameLoop {
public:
    HelloWorldGame() {
        setName("Hello World Game");
        setFixedTimeStep(true);
        setTargetFPS(60);
    }

    ~HelloWorldGame() {
    }

protected:
    bool onInit() override {
        // 初始化日志系统
        LogManager::getInstance().init(LogTarget::BOTH, "hello_world.log");
        LogManager::getInstance().setLogLevel(LogLevel::INFO);
        
        LOG_INFO("Initializing Hello World Game");

        // 初始化图形系统
        if (!Graphics::init()) {
            LOG_FATAL("Failed to initialize graphics system");
            return false;
        }

        // 创建窗口
        if (!Graphics::createWindow(800, 600, "Hello World")) {
            LOG_FATAL("Failed to create window");
            return false;
        }

        // 初始化输入系统
        if (!Input::init()) {
            LOG_FATAL("Failed to initialize input system");
            return false;
        }

        // 初始化资源系统
        if (!ResourceManager::getInstance().init()) {
            LOG_FATAL("Failed to initialize resource system");
            return false;
        }

        // 初始化物理系统
        if (!PhysicsManager::getInstance().init()) {
            LOG_FATAL("Failed to initialize physics system");
            return false;
        }

        // 初始化音频系统
        if (!AudioManager::getInstance().init()) {
            LOG_WARN("Failed to initialize audio system, continuing without audio");
        }

        // 初始化存储系统
        if (!StorageManager::getInstance().init()) {
            LOG_FATAL("Failed to initialize storage system");
            return false;
        }

        // 加载测试数据
        loadTestData();

        LOG_INFO("Game initialized successfully");
        return true;
    }

    void onUpdate(float deltaTime) override {
        // 更新游戏逻辑
        m_positionX += m_velocityX * deltaTime;
        m_positionY += m_velocityY * deltaTime;

        // 边界检查
        if (m_positionX < 0 || m_positionX > 750) {
            m_velocityX *= -1;
        }
        if (m_positionY < 0 || m_positionY > 550) {
            m_velocityY *= -1;
        }

        // 处理输入
        handleInput();
    }

    void onRender() override {
        // 清屏
        Graphics::getRenderer()->clear(Color(0.1f, 0.1f, 0.2f, 1.0f));

        // 绘制矩形
        Graphics::getRenderer()->drawRect(
            Rect(m_positionX, m_positionY, 50, 50),
            Color(1.0f, 0.5f, 0.0f, 1.0f),
            true
        );

        // 绘制文本
        // TODO: Implement text rendering

        // 交换缓冲区
        Graphics::swapBuffers();
    }

    void onCleanup() override {
        LOG_INFO("Cleaning up game resources");

        // 清理资源
        AudioManager::getInstance().cleanup();
        PhysicsManager::getInstance().cleanup();
        ResourceManager::getInstance().cleanup();
        Input::cleanup();
        Graphics::cleanup();
        StorageManager::getInstance().cleanup();

        LOG_INFO("Game cleanup completed");
    }

private:
    void handleInput() {
        // 检查按键状态
        if (Input::isKeyPressed(KeyCode::ESCAPE)) {
            quit();
        }

        // 检查鼠标状态
        if (Input::isMouseButtonPressed(MouseButton::LEFT)) {
            auto mousePos = Input::getMousePosition();
            LOG_INFO("Mouse clicked at: " + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
        }

        // 检查触摸状态
        for (const auto& touch : Input::getTouches()) {
            if (touch.state == TouchState::PRESSED) {
                LOG_INFO("Touch at: " + std::to_string(touch.x) + ", " + std::to_string(touch.y));
            }
        }
    }

    void loadTestData() {
        // 保存测试数据到存储
        Storage::save("player_name", "Test Player");
        Storage::save("player_score", 1000);
        Storage::save("sound_enabled", true);

        // 加载测试数据
        std::string playerName;
        int playerScore = 0;
        bool soundEnabled = false;

        if (Storage::load("player_name", playerName)) {
            LOG_INFO("Loaded player name: " + playerName);
        }

        if (Storage::load("player_score", playerScore)) {
            LOG_INFO("Loaded player score: " + std::to_string(playerScore));
        }

        if (Storage::load("sound_enabled", soundEnabled)) {
            LOG_INFO("Loaded sound enabled: " + std::to_string(soundEnabled));
        }

        // 测试加密存储
        Storage::saveEncrypted("secret_data", "This is a secret message");
        std::string secretData;
        if (Storage::loadEncrypted("secret_data", secretData)) {
            LOG_INFO("Loaded encrypted data: " + secretData);
        }
    }

private:
    float m_positionX = 100.0f;
    float m_positionY = 100.0f;
    float m_velocityX = 100.0f;
    float m_velocityY = 100.0f;
};

int main(int argc, char* argv[]) {
    LOG_INFO("Starting Hello World Game");

    try {
        HelloWorldGame game;
        game.run();
    } catch (const std::exception& e) {
        LOG_FATAL("Exception: " + std::string(e.what()));
        return 1;
    }

    LOG_INFO("Game exited successfully");
    return 0;
}
