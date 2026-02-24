#include "core/GameLoop.h"
#include <thread>

namespace Appgame {

GameLoop::GameLoop()
    : m_timeStepMode(TimeStepMode::FIXED)
    , m_fixedTimeStep(1.0f / 60.0f) // 默认60fps
    , m_targetFrameTime(1.0f / 60.0f) // 默认60fps
    , m_running(false)
    , m_paused(false)
    , m_accumulator(0.0f)
    , m_frameCount(0)
    , m_frameTimeSum(0.0f)
    , m_lastFrameTime(0.0f)
{
    m_stats.fps = 0.0f;
    m_stats.avgFrameTime = 0.0f;
    m_stats.maxFrameTime = 0.0f;
    m_lastTime = std::chrono::steady_clock::now();
}

GameLoop::~GameLoop() {
    stop();
}

void GameLoop::start() {
    if (!m_running) {
        m_running = true;
        m_paused = false;
        m_lastTime = std::chrono::steady_clock::now();
        run();
    }
}

void GameLoop::stop() {
    m_running = false;
}

void GameLoop::pause() {
    m_paused = true;
}

void GameLoop::resume() {
    m_paused = false;
    m_lastTime = std::chrono::steady_clock::now();
    m_accumulator = 0.0f;
}

void GameLoop::setTimeStepMode(TimeStepMode mode) {
    m_timeStepMode = mode;
}

void GameLoop::setTargetFPS(int fps) {
    m_targetFrameTime = 1.0f / fps;
}

void GameLoop::setFixedTimeStep(float step) {
    m_fixedTimeStep = step;
}

const GameLoop::Stats& GameLoop::getStats() const {
    return m_stats;
}

void GameLoop::setUpdateCallback(std::function<void(float)> callback) {
    m_updateCallback = callback;
}

void GameLoop::setRenderCallback(std::function<void(float)> callback) {
    m_renderCallback = callback;
}

void GameLoop::run() {
    while (m_running) {
        m_currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(m_currentTime - m_lastTime).count();
        m_lastTime = m_currentTime;

        // 限制最大deltaTime，防止大延迟导致的问题
        if (deltaTime > 0.1f) {
            deltaTime = 0.1f;
        }

        if (!m_paused) {
            if (m_timeStepMode == TimeStepMode::FIXED) {
                // 固定时间步长模式
                m_accumulator += deltaTime;
                while (m_accumulator >= m_fixedTimeStep) {
                    if (m_updateCallback) {
                        m_updateCallback(m_fixedTimeStep);
                    }
                    m_accumulator -= m_fixedTimeStep;
                }

                // 渲染插值
                float alpha = m_accumulator / m_fixedTimeStep;
                if (m_renderCallback) {
                    m_renderCallback(alpha);
                }
            } else {
                // 可变时间步长模式
                if (m_updateCallback) {
                    m_updateCallback(deltaTime);
                }
                if (m_renderCallback) {
                    m_renderCallback(deltaTime);
                }
            }

            // 更新统计信息
            updateStats();

            // 帧率控制
            float frameTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_currentTime).count();
            float sleepTime = m_targetFrameTime - frameTime;
            if (sleepTime > 0.0f) {
                std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
            }
        }
    }
}

void GameLoop::updateStats() {
    m_frameCount++;
    float frameTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_currentTime).count() * 1000.0f; // 转换为毫秒
    m_frameTimeSum += frameTime;
    m_lastFrameTime = frameTime;

    if (frameTime > m_stats.maxFrameTime) {
        m_stats.maxFrameTime = frameTime;
    }

    // 每秒更新一次统计信息
    static float lastSecond = 0.0f;
    lastSecond += frameTime / 1000.0f;
    if (lastSecond >= 1.0f) {
        m_stats.fps = m_frameCount / lastSecond;
        m_stats.avgFrameTime = m_frameTimeSum / m_frameCount;
        m_frameCount = 0;
        m_frameTimeSum = 0.0f;
        m_stats.maxFrameTime = 0.0f;
        lastSecond = 0.0f;
    }
}

} // namespace Appgame
