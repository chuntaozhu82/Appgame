#include "core/Graphics.h"

namespace Appgame {

Renderer::Renderer(std::unique_ptr<GraphicsDevice> device)
    : m_device(std::move(device))
{
}

Renderer::~Renderer() {
}

bool Renderer::init() {
    if (!m_device->init()) {
        return false;
    }

    setupDefaultShader();
    return true;
}

void Renderer::beginRender() {
    m_vertices.clear();
    m_indices.clear();
}

void Renderer::endRender() {
    flush();
    m_device->swapBuffers();
}

void Renderer::drawSprite(const Texture& texture, const Rect& srcRect, const Rect& dstRect, float rotation, const Color& color) {
    // 计算顶点
    float centerX = dstRect.x + dstRect.width * 0.5f;
    float centerY = dstRect.y + dstRect.height * 0.5f;

    // 生成四个顶点
    std::vector<Vertex> vertices;
    vertices.reserve(4);

    // 左上角
    float x1 = dstRect.x - centerX;
    float y1 = dstRect.y - centerY;
    vertices.emplace_back(x1, y1, 0.0f, srcRect.x, srcRect.y, color);

    // 右上角
    float x2 = dstRect.x + dstRect.width - centerX;
    float y2 = dstRect.y - centerY;
    vertices.emplace_back(x2, y2, 0.0f, srcRect.x + srcRect.width, srcRect.y, color);

    // 左下角
    float x3 = dstRect.x - centerX;
    float y3 = dstRect.y + dstRect.height - centerY;
    vertices.emplace_back(x3, y3, 0.0f, srcRect.x, srcRect.y + srcRect.height, color);

    // 右下角
    float x4 = dstRect.x + dstRect.width - centerX;
    float y4 = dstRect.y + dstRect.height - centerY;
    vertices.emplace_back(x4, y4, 0.0f, srcRect.x + srcRect.width, srcRect.y + srcRect.height, color);

    // 应用旋转
    if (rotation != 0.0f) {
        float cosTheta = cosf(rotation);
        float sinTheta = sinf(rotation);

        for (auto& vertex : vertices) {
            float x = vertex.x;
            float y = vertex.y;
            vertex.x = x * cosTheta - y * sinTheta + centerX;
            vertex.y = x * sinTheta + y * cosTheta + centerY;
        }
    } else {
        // 不旋转时直接偏移
        for (auto& vertex : vertices) {
            vertex.x += centerX;
            vertex.y += centerY;
        }
    }

    // 添加到顶点缓冲区
    unsigned int baseIndex = m_vertices.size();
    m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());

    // 添加索引
    m_indices.push_back(baseIndex);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 3);

    // 检查是否需要刷新
    if (m_vertices.size() > 1000 || m_indices.size() > 3000) {
        flush();
    }
}

void Renderer::drawRect(const Rect& rect, const Color& color) {
    // 添加四个顶点
    unsigned int baseIndex = m_vertices.size();
    m_vertices.emplace_back(rect.x, rect.y, 0.0f, 0.0f, 0.0f, color);
    m_vertices.emplace_back(rect.x + rect.width, rect.y, 0.0f, 0.0f, 0.0f, color);
    m_vertices.emplace_back(rect.x, rect.y + rect.height, 0.0f, 0.0f, 0.0f, color);
    m_vertices.emplace_back(rect.x + rect.width, rect.y + rect.height, 0.0f, 0.0f, 0.0f, color);

    // 添加索引
    m_indices.push_back(baseIndex);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 3);
}

void Renderer::drawLine(float x1, float y1, float x2, float y2, float width, const Color& color) {
    // 计算线条方向
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);

    // 计算垂直方向向量
    float nx = -dy / length * width * 0.5f;
    float ny = dx / length * width * 0.5f;

    // 添加四个顶点
    unsigned int baseIndex = m_vertices.size();
    m_vertices.emplace_back(x1 + nx, y1 + ny, 0.0f, 0.0f, 0.0f, color);
    m_vertices.emplace_back(x2 + nx, y2 + ny, 0.0f, 0.0f, 0.0f, color);
    m_vertices.emplace_back(x1 - nx, y1 - ny, 0.0f, 0.0f, 0.0f, color);
    m_vertices.emplace_back(x2 - nx, y2 - ny, 0.0f, 0.0f, 0.0f, color);

    // 添加索引
    m_indices.push_back(baseIndex);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 3);
}

void Renderer::setTargetTexture(Texture* texture) {
    // 这里需要实现渲染目标的设置
}

GraphicsDevice* Renderer::getDevice() {
    return m_device.get();
}

void Renderer::flush() {
    if (m_vertices.empty() || m_indices.empty()) {
        return;
    }

    // 使用默认着色器
    m_defaultShader->use();

    // 这里需要实现实际的渲染逻辑
    // 包括设置顶点缓冲区、索引缓冲区、纹理绑定等

    m_vertices.clear();
    m_indices.clear();
}

void Renderer::setupDefaultShader() {
    // 创建默认着色器
    m_defaultShader = std::make_unique<Shader>();

    // 默认顶点着色器
    std::string vertexSource = R"(
        attribute vec3 position;
        attribute vec2 texCoord;
        attribute vec4 color;
        
        uniform mat4 projection;
        
        varying vec2 vTexCoord;
        varying vec4 vColor;
        
        void main() {
            gl_Position = projection * vec4(position, 1.0);
            vTexCoord = texCoord;
            vColor = color;
        }
    )";

    // 默认片段着色器
    std::string fragmentSource = R"(
        precision mediump float;
        
        varying vec2 vTexCoord;
        varying vec4 vColor;
        
        uniform sampler2D texture;
        
        void main() {
            vec4 texColor = texture2D(texture, vTexCoord);
            gl_FragColor = texColor * vColor;
        }
    )";

    m_defaultShader->compile(vertexSource, fragmentSource);
}

GraphicsManager::GraphicsManager()
    : m_initialized(false)
{
}

GraphicsManager::~GraphicsManager() {
    if (m_initialized) {
        cleanup();
    }
}

GraphicsManager& GraphicsManager::getInstance() {
    static GraphicsManager instance;
    return instance;
}

bool GraphicsManager::init() {
    if (!m_initialized) {
        // 初始化图形系统
        // 这里需要实现平台特定的初始化逻辑
        m_initialized = true;
    }
    return m_initialized;
}

void GraphicsManager::cleanup() {
    if (m_initialized) {
        // 清理图形系统
        m_initialized = false;
    }
}

std::unique_ptr<Renderer> GraphicsManager::createRenderer() {
    // 创建图形设备
    // 这里需要实现平台特定的图形设备创建
    std::unique_ptr<GraphicsDevice> device;
    
    // 创建渲染器
    return std::make_unique<Renderer>(std::move(device));
}

std::unique_ptr<Shader> GraphicsManager::createShader() {
    // 创建着色器
    // 这里需要实现平台特定的着色器创建
    return std::make_unique<Shader>();
}

std::unique_ptr<Texture> GraphicsManager::createTexture() {
    // 创建纹理
    // 这里需要实现平台特定的纹理创建
    return std::make_unique<Texture>();
}

} // namespace Appgame
