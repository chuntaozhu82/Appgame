#ifndef INPUT_H
#define INPUT_H

#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Appgame {

// 输入事件类型
enum class InputEventType {
    TOUCH_DOWN,      // 触摸开始
    TOUCH_MOVE,      // 触摸移动
    TOUCH_UP,        // 触摸结束
    KEY_DOWN,        // 键盘按下
    KEY_UP,          // 键盘释放
    MOUSE_MOVE,      // 鼠标移动
    MOUSE_DOWN,      // 鼠标按下
    MOUSE_UP,        // 鼠标释放
    MOUSE_SCROLL     // 鼠标滚轮
};

// 触摸点结构体
truct TouchPoint {
    int id;           // 触摸点ID
    float x;          // x坐标
    float y;          // y坐标
    float pressure;   // 压力值（如果支持）

    TouchPoint(int id = 0, float x = 0.0f, float y = 0.0f, float pressure = 1.0f)
        : id(id), x(x), y(y), pressure(pressure) {}
};

// 按键枚举
enum class KeyCode {
    // 字母键
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    // 数字键
    NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    
    // 功能键
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    
    // 控制键
    ESCAPE, TAB, SHIFT, CTRL, ALT, SPACE,
    ENTER, BACKSPACE, DELETE, INSERT,
    HOME, END, PAGE_UP, PAGE_DOWN,
    LEFT, RIGHT, UP, DOWN,
    
    // 其他键
    UNKNOWN
};

// 鼠标按键枚举
enum class MouseButton {
    LEFT,   // 左键
    RIGHT,  // 右键
    MIDDLE, // 中键
    UNKNOWN
};

// 输入事件结构体
truct InputEvent {
    InputEventType type;   // 事件类型
    
    // 事件数据
    union {
        TouchPoint touch;   // 触摸事件数据
        KeyCode key;        // 键盘事件数据
        MouseButton mouse;  // 鼠标事件数据
        float scroll;       // 鼠标滚轮数据
    } data;
    
    // 通用数据
    float x;               // x坐标（用于鼠标事件）
    float y;               // y坐标（用于鼠标事件）
    bool isRepeat;         // 是否是重复事件（用于键盘事件）
};

// 输入设备抽象类
class InputDevice {
publc:
    virtual ~InputDevice() = default;

    // 初始化输入设备
    virtual bool init() = 0;

    // 清理输入设备
    virtual void cleanup() = 0;

    // 处理输入事件
    virtual void processEvents() = 0;

    // 检查按键状态
    virtual bool isKeyPressed(KeyCode key) const = 0;

    // 检查鼠标按键状态
    virtual bool isMouseButtonPressed(MouseButton button) const = 0;

    // 获取鼠标位置
    virtual void getMousePosition(float& x, float& y) const = 0;

    // 获取触摸点
    virtual const std::vector<TouchPoint>& getTouchPoints() const = 0;
};

// 输入事件监听器
class InputListener {
publc:
    virtual ~InputListener() = default;

    // 处理输入事件
    virtual bool onInputEvent(const InputEvent& event) = 0;
};

// 输入处理器类
class InputHandler {
publc:
    InputHandler(std::unique_ptr<InputDevice> device);
    ~InputHandler();

    // 初始化输入处理器
    bool init();

    // 更新输入状态
    void update();

    // 注册事件监听器
    void addListener(InputListener* listener);

    // 移除事件监听器
    void removeListener(InputListener* listener);

    // 检查按键状态
    bool isKeyPressed(KeyCode key) const;

    // 检查按键是否刚刚按下
    bool isKeyJustPressed(KeyCode key) const;

    // 检查按键是否刚刚释放
    bool isKeyJustReleased(KeyCode key) const;

    // 检查鼠标按键状态
    bool isMouseButtonPressed(MouseButton button) const;

    // 检查鼠标按键是否刚刚按下
    bool isMouseButtonJustPressed(MouseButton button) const;

    // 检查鼠标按键是否刚刚释放
    bool isMouseButtonJustReleased(MouseButton button) const;

    // 获取鼠标位置
    void getMousePosition(float& x, float& y) const;

    // 获取鼠标移动 delta
    void getMouseDelta(float& dx, float& dy) const;

    // 获取触摸点
    const std::vector<TouchPoint>& getTouchPoints() const;

    // 获取输入设备
    InputDevice* getDevice();

private:
    std::unique_ptr<InputDevice> m_device;
    std::vector<InputListener*> m_listeners;
    
    // 按键状态
    std::unordered_map<KeyCode, bool> m_keyStates;
    std::unordered_map<KeyCode, bool> m_prevKeyStates;
    
    // 鼠标按键状态
    std::unordered_map<MouseButton, bool> m_mouseButtonStates;
    std::unordered_map<MouseButton, bool> m_prevMouseButtonStates;
    
    // 鼠标位置
    float m_mouseX;
    float m_mouseY;
    float m_prevMouseX;
    float m_prevMouseY;
    
    // 触摸点
    std::vector<TouchPoint> m_touchPoints;

    // 内部方法
    void processKeyEvents();
    void processMouseEvents();
    void processTouchEvents();
    void notifyListeners(const InputEvent& event);
};

// 输入管理器类
class InputManager {
publc:
    static InputManager& getInstance();

    // 初始化输入系统
    bool init();

    // 清理输入系统
    void cleanup();

    // 创建输入处理器
    std::unique_ptr<InputHandler> createInputHandler();

    // 获取全局输入处理器
    InputHandler* getGlobalInputHandler();

private:
    InputManager();
    ~InputManager();

    bool m_initialized;
    std::unique_ptr<InputHandler> m_globalInputHandler;
};

} // namespace Appgame

#endif // INPUT_H
