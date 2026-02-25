#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace FishingGame {

// 前向声明
class UIElement;
class HUD;
class InventoryUI;
class ShopUI;
class Platform;

// UI元素类型
enum class UIElementType {
    BUTTON,
    LABEL,
    IMAGE,
    PROGRESS_BAR,
    SLIDER,
    CHECKBOX,
    RADIO_BUTTON,
    TEXT_INPUT,
    PANEL,
    WINDOW,
    SCROLL_VIEW,
    GRID_VIEW,
    LIST_VIEW
};

// UI对齐方式
enum class UIAlignment {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_CENTER,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

// UI锚点
enum class UIAnchor {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_CENTER,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

// UI元素接口
class UIElement {
public:
    virtual ~UIElement() = default;

    // 初始化UI元素
    virtual bool init() = 0;

    // 清理UI元素
    virtual void cleanup() = 0;

    // 更新UI元素
    virtual void update(float32 deltaTime) = 0;

    // 渲染UI元素
    virtual void render() = 0;

    // 处理输入
    virtual bool handleInput(int32 inputType, int32 inputValue, float32 x, float32 y) = 0;

    // 获取UI元素类型
    virtual UIElementType getType() const = 0;

    // 获取UI元素名称
    virtual std::string getName() const = 0;

    // 获取UI元素ID
    virtual uint32 getID() const = 0;

    // 设置UI元素位置
    virtual void setPosition(float32 x, float32 y) = 0;

    // 获取UI元素位置
    virtual void getPosition(float32& x, float32& y) const = 0;

    // 设置UI元素大小
    virtual void setSize(float32 width, float32 height) = 0;

    // 获取UI元素大小
    virtual void getSize(float32& width, float32& height) const = 0;

    // 设置UI元素锚点
    virtual void setAnchor(UIAnchor anchor) = 0;

    // 获取UI元素锚点
    virtual UIAnchor getAnchor() const = 0;

    // 设置UI元素对齐方式
    virtual void setAlignment(UIAlignment alignment) = 0;

    // 获取UI元素对齐方式
    virtual UIAlignment getAlignment() const = 0;

    // 设置UI元素可见性
    virtual void setVisible(bool visible) = 0;

    // 获取UI元素可见性
    virtual bool isVisible() const = 0;

    // 设置UI元素启用状态
    virtual void setEnabled(bool enabled) = 0;

    // 获取UI元素启用状态
    virtual bool isEnabled() const = 0;

    // 设置UI元素缩放因子
    virtual void setScale(float32 scale) = 0;

    // 获取UI元素缩放因子
    virtual float32 getScale() const = 0;

    // 设置UI元素透明度
    virtual void setAlpha(float32 alpha) = 0;

    // 获取UI元素透明度
    virtual float32 getAlpha() const = 0;

    // 设置UI元素父元素
    virtual void setParent(UIElement* parent) = 0;

    // 获取UI元素父元素
    virtual UIElement* getParent() const = 0;

    // 添加子元素
    virtual bool addChild(UIElement* child) = 0;

    // 移除子元素
    virtual void removeChild(UIElement* child) = 0;

    // 移除所有子元素
    virtual void removeAllChildren() = 0;

    // 获取子元素
    virtual UIElement* getChild(uint32 id) const = 0;

    // 获取所有子元素
    virtual const std::vector<UIElement*>& getChildren() const = 0;

    // 检查是否包含点
    virtual bool containsPoint(float32 x, float32 y) const = 0;

    // 获取边界矩形
    virtual void getBounds(Rectf& bounds) const = 0;

    // 响应式调整大小
    virtual void resize(float32 width, float32 height) = 0;

    // 获取UI管理器
    virtual UIManager* getUIManager() const = 0;

    // 设置UI管理器
    virtual void setUIManager(UIManager* uiManager) = 0;
};

// 基础UI元素类
class BaseUIElement : public UIElement {
public:
    BaseUIElement(UIElementType type, const std::string& name, uint32 id);
    ~BaseUIElement() override;

    // 初始化UI元素
    bool init() override;

    // 清理UI元素
    void cleanup() override;

    // 更新UI元素
    void update(float32 deltaTime) override;

    // 渲染UI元素
    void render() override;

    // 处理输入
    bool handleInput(int32 inputType, int32 inputValue, float32 x, float32 y) override;

    // 获取UI元素类型
    UIElementType getType() const override;

    // 获取UI元素名称
    std::string getName() const override;

    // 获取UI元素ID
    uint32 getID() const override;

    // 设置UI元素位置
    void setPosition(float32 x, float32 y) override;

    // 获取UI元素位置
    void getPosition(float32& x, float32& y) const override;

    // 设置UI元素大小
    void setSize(float32 width, float32 height) override;

    // 获取UI元素大小
    void getSize(float32& width, float32& height) const override;

    // 设置UI元素锚点
    void setAnchor(UIAnchor anchor) override;

    // 获取UI元素锚点
    UIAnchor getAnchor() const override;

    // 设置UI元素对齐方式
    void setAlignment(UIAlignment alignment) override;

    // 获取UI元素对齐方式
    UIAlignment getAlignment() const override;

    // 设置UI元素可见性
    void setVisible(bool visible) override;

    // 获取UI元素可见性
    bool isVisible() const override;

    // 设置UI元素启用状态
    void setEnabled(bool enabled) override;

    // 获取UI元素启用状态
    bool isEnabled() const override;

    // 设置UI元素缩放因子
    void setScale(float32 scale) override;

    // 获取UI元素缩放因子
    float32 getScale() const override;

    // 设置UI元素透明度
    void setAlpha(float32 alpha) override;

    // 获取UI元素透明度
    float32 getAlpha() const override;

    // 设置UI元素父元素
    void setParent(UIElement* parent) override;

    // 获取UI元素父元素
    UIElement* getParent() const override;

    // 添加子元素
    bool addChild(UIElement* child) override;

    // 移除子元素
    void removeChild(UIElement* child) override;

    // 移除所有子元素
    void removeAllChildren() override;

    // 获取子元素
    UIElement* getChild(uint32 id) const override;

    // 获取所有子元素
    const std::vector<UIElement*>& getChildren() const override;

    // 检查是否包含点
    bool containsPoint(float32 x, float32 y) const override;

    // 获取边界矩形
    void getBounds(Rectf& bounds) const override;

    // 响应式调整大小
    void resize(float32 width, float32 height) override;

    // 获取UI管理器
    UIManager* getUIManager() const override;

    // 设置UI管理器
    void setUIManager(UIManager* uiManager) override;

protected:
    // UI元素类型
    UIElementType m_type;

    // UI元素名称
    std::string m_name;

    // UI元素ID
    uint32 m_id;

    // UI元素位置
    Vector2f m_position;

    // UI元素大小
    Vector2f m_size;

    // UI元素锚点
    UIAnchor m_anchor;

    // UI元素对齐方式
    UIAlignment m_alignment;

    // UI元素可见性
    bool m_visible;

    // UI元素启用状态
    bool m_enabled;

    // UI元素缩放因子
    float32 m_scale;

    // UI元素透明度
    float32 m_alpha;

    // 父元素
    UIElement* m_parent;

    // 子元素
    std::vector<UIElement*> m_children;

    // UI管理器
    UIManager* m_uiManager;

    // 计算绝对位置
    void calculateAbsolutePosition();

    // 计算边界矩形
    void calculateBounds();

    // 响应式调整子元素大小
    void resizeChildren(float32 width, float32 height);
};

// UI管理器类
class UIManager {
public:
    UIManager();
    ~UIManager();

    // 初始化UI管理器
    bool init();

    // 清理UI管理器
    void cleanup();

    // 更新UI管理器
    void update(float32 deltaTime);

    // 渲染UI管理器
    void render();

    // 处理输入
    bool handleInput(int32 inputType, int32 inputValue, float32 x, float32 y);

    // 添加UI元素
    bool addUIElement(UIElement* element);

    // 移除UI元素
    void removeUIElement(UIElement* element);

    // 移除所有UI元素
    void removeAllUIElements();

    // 获取UI元素
    UIElement* getUIElement(uint32 id) const;

    // 获取UI元素
    UIElement* getUIElement(const std::string& name) const;

    // 获取所有UI元素
    const std::vector<UIElement*>& getUIElements() const;

    // 获取指定类型的UI元素
    std::vector<UIElement*> getUIElementsByType(UIElementType type) const;

    // 显示HUD
    void showHUD();

    // 隐藏HUD
    void hideHUD();

    // 获取HUD
    HUD* getHUD() const;

    // 显示背包UI
    void showInventoryUI();

    // 隐藏背包UI
    void hideInventoryUI();

    // 获取背包UI
    InventoryUI* getInventoryUI() const;

    // 显示商店UI
    void showShopUI();

    // 隐藏商店UI
    void hideShopUI();

    // 获取商店UI
    ShopUI* getShopUI() const;

    // 设置平台
    void setPlatform(Platform* platform);

    // 获取平台
    Platform* getPlatform() const;

    // 设置屏幕大小
    void setScreenSize(int32 width, int32 height);

    // 获取屏幕大小
    void getScreenSize(int32& width, int32& height) const;

    // 响应式调整大小
    void resize(int32 width, int32 height);

    // 获取UI缩放因子
    float32 getUIScaleFactor() const;

    // 设置UI缩放因子
    void setUIScaleFactor(float32 scaleFactor);

    // 获取输入类型
    InputType getInputType() const;

    // 设置输入类型
    void setInputType(InputType inputType);

    // 获取设备类型
    DeviceType getDeviceType() const;

    // 设置设备类型
    void setDeviceType(DeviceType deviceType);

    // 创建按钮
    UIElement* createButton(const std::string& name, const std::string& text, float32 x, float32 y, float32 width, float32 height);

    // 创建标签
    UIElement* createLabel(const std::string& name, const std::string& text, float32 x, float32 y, float32 fontSize);

    // 创建图像
    UIElement* createImage(const std::string& name, const std::string& imagePath, float32 x, float32 y, float32 width, float32 height);

    // 创建进度条
    UIElement* createProgressBar(const std::string& name, float32 x, float32 y, float32 width, float32 height, float32 progress);

    // 创建滑块
    UIElement* createSlider(const std::string& name, float32 x, float32 y, float32 width, float32 height, float32 value);

    // 创建复选框
    UIElement* createCheckbox(const std::string& name, const std::string& text, float32 x, float32 y, bool checked);

    // 创建单选按钮
    UIElement* createRadioButton(const std::string& name, const std::string& text, float32 x, float32 y, bool checked);

    // 创建文本输入
    UIElement* createTextInput(const std::string& name, float32 x, float32 y, float32 width, float32 height, const std::string& text);

    // 创建面板
    UIElement* createPanel(const std::string& name, float32 x, float32 y, float32 width, float32 height);

    // 创建窗口
    UIElement* createWindow(const std::string& name, const std::string& title, float32 x, float32 y, float32 width, float32 height);

    // 创建滚动视图
    UIElement* createScrollView(const std::string& name, float32 x, float32 y, float32 width, float32 height);

    // 创建网格视图
    UIElement* createGridView(const std::string& name, float32 x, float32 y, float32 width, float32 height, int32 rows, int32 columns);

    // 创建列表视图
    UIElement* createListView(const std::string& name, float32 x, float32 y, float32 width, float32 height);

private:
    // UI元素
    std::vector<UIElement*> m_uiElements;

    // UI元素映射（按ID）
    std::map<uint32, UIElement*> m_uiElementsById;

    // UI元素映射（按名称）
    std::map<std::string, UIElement*> m_uiElementsByName;

    // HUD
    HUD* m_hud;

    // 背包UI
    InventoryUI* m_inventoryUI;

    // 商店UI
    ShopUI* m_shopUI;

    // 平台
    Platform* m_platform;

    // 屏幕大小
    int32 m_screenWidth;
    int32 m_screenHeight;

    // UI缩放因子
    float32 m_uiScaleFactor;

    // 输入类型
    InputType m_inputType;

    // 设备类型
    DeviceType m_deviceType;

    // 下一个UI元素ID
    uint32 m_nextElementId;

    // 初始化HUD
    void initHUD();

    // 初始化背包UI
    void initInventoryUI();

    // 初始化商店UI
    void initShopUI();

    // 生成UI元素ID
    uint32 generateElementId();

    // 响应式调整UI元素大小
    void resizeUIElements(int32 width, int32 height);

    // 根据设备类型调整UI
    void adjustUIForDeviceType();

    // 根据输入类型调整UI
    void adjustUIForInputType();
};

// 全局UI管理器实例
extern UIManager* g_uiManager;

} // namespace FishingGame

#endif // UI_MANAGER_H
