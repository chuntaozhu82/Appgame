#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <memory>
#include <vector>

namespace Appgame {

// 颜色结构体
struct Color {
    float r, g, b, a;

    Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}
};

// 顶点结构体
struct Vertex {
    float x, y, z;
    float u, v;
    Color color;

    Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f,
           float u = 0.0f, float v = 0.0f,
           const Color& color = Color())
        : x(x), y(y), z(z), u(u), v(v), color(color) {}
};

// 矩形结构体
struct Rect {
    float x, y, width, height;

    Rect(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
        : x(x), y(y), width(width), height(height) {}
};

// 图形设备抽象类
class GraphicsDevice {
public:
    virtual ~GraphicsDevice() = default;

    // 初始化图形设备
    virtual bool init() = 0;

    // 清理图形设备
    virtual void cleanup() = 0;

    // 交换缓冲区
    virtual void swapBuffers() = 0;

    // 设置视口
    virtual void setViewport(int x, int y, int width, int height) = 0;

    // 获取视口大小
    virtual void getViewport(int& x, int& y, int& width, int& height) const = 0;

    // 清除屏幕
    virtual void clear(const Color& color) = 0;
};

// 着色器类
class Shader {
public:
    virtual ~Shader() = default;

    // 编译着色器
    virtual bool compile(const std::string& vertexSource, const std::string& fragmentSource) = 0;

    // 使用着色器
    virtual void use() = 0;

    // 设置 uniforms
    virtual void setUniform1i(const std::string& name, int value) = 0;
    virtual void setUniform1f(const std::string& name, float value) = 0;
    virtual void setUniform2f(const std::string& name, float x, float y) = 0;
    virtual void setUniform3f(const std::string& name, float x, float y, float z) = 0;
    virtual void setUniform4f(const std::string& name, float x, float y, float z, float w) = 0;
    virtual void setUniformMatrix4f(const std::string& name, const float* matrix) = 0;
};

// 纹理类
class Texture {
public:
    virtual ~Texture() = default;

    // 加载纹理
    virtual bool loadFromFile(const std::string& filePath) = 0;
    virtual bool loadFromMemory(const void* data, size_t size) = 0;

    // 绑定纹理
    virtual void bind(int unit = 0) = 0;

    // 获取纹理尺寸
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

// 渲染器类
class Renderer {
public:
    Renderer(std::unique_ptr<GraphicsDevice> device);
    ~Renderer();

    // 初始化渲染器
    bool init();

    // 开始渲染
    void beginRender();

    // 结束渲染
    void endRender();

    // 绘制精灵
    void drawSprite(const Texture& texture, const Rect& srcRect, const Rect& dstRect, float rotation = 0.0f, const Color& color = Color(1.0f, 1.0f, 1.0f, 1.0f));

    // 绘制矩形
    void drawRect(const Rect& rect, const Color& color);

    // 绘制线条
    void drawLine(float x1, float y1, float x2, float y2, float width, const Color& color);

    // 设置渲染目标
    void setTargetTexture(Texture* texture = nullptr);

    // 获取图形设备
    GraphicsDevice* getDevice();

private:
    std::unique_ptr<GraphicsDevice> m_device;
    std::unique_ptr<Shader> m_defaultShader;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    // 内部方法
    void flush();
    void setupDefaultShader();
};

// 图形管理器类
class GraphicsManager {
public:
    static GraphicsManager& getInstance();

    // 初始化图形系统
    bool init();

    // 清理图形系统
    void cleanup();

    // 创建渲染器
    std::unique_ptr<Renderer> createRenderer();

    // 创建着色器
    std::unique_ptr<Shader> createShader();

    // 创建纹理
    std::unique_ptr<Texture> createTexture();

private:
    GraphicsManager();
    ~GraphicsManager();

    bool m_initialized;
};

} // namespace Appgame

#endif // GRAPHICS_H
