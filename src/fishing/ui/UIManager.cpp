#include "fishing/ui/UIManager.h"
#include "fishing/ui/HUD.h"
#include "fishing/ui/InventoryUI.h"
#include "fishing/ui/ShopUI.h"
#include "fishing/platform/Platform.h"
#include <iostream>

namespace FishingGame {

// UIElement implementation
UIElement::~UIElement() {
}

// BaseUIElement implementation
BaseUIElement::BaseUIElement(UIElementType type, const std::string& name, uint32 id)
    : m_type(type),
      m_name(name),
      m_id(id),
      m_position{0.0f, 0.0f},
      m_size{100.0f, 50.0f},
      m_anchor(UIAnchor::TOP_LEFT),
      m_alignment(UIAlignment::TOP_LEFT),
      m_visible(true),
      m_enabled(true),
      m_scale(1.0f),
      m_alpha(1.0f),
      m_parent(nullptr),
      m_uiManager(nullptr)
{
}

BaseUIElement::~BaseUIElement() {
    cleanup();
}

bool BaseUIElement::init() {
    // 初始化子元素
    for (auto& child : m_children) {
        if (!child->init()) {
            return false;
        }
    }
    
    std::cout << "BaseUIElement initialized: " << m_name << " (ID: " << m_id << ")" << std::endl;
    return true;
}

void BaseUIElement::cleanup() {
    // 清理子元素
    for (auto& child : m_children) {
        child->cleanup();
        delete child;
    }
    m_children.clear();
    
    std::cout << "BaseUIElement cleaned up: " << m_name << " (ID: " << m_id << ")" << std::endl;
}

void BaseUIElement::update(float32 deltaTime) {
    if (!m_visible || !m_enabled) {
        return;
    }
    
    // 更新子元素
    for (auto& child : m_children) {
        child->update(deltaTime);
    }
}

void BaseUIElement::render() {
    if (!m_visible) {
        return;
    }
    
    // 渲染子元素
    for (auto& child : m_children) {
        child->render();
    }
}

bool BaseUIElement::handleInput(int32 inputType, int32 inputValue, float32 x, float32 y) {
    if (!m_visible || !m_enabled) {
        return false;
    }
    
    // 处理子元素输入
    for (auto& child : m_children) {
        if (child->handleInput(inputType, inputValue, x, y)) {
            return true;
        }
    }
    
    return false;
}

UIElementType BaseUIElement::getType() const {
    return m_type;
}

std::string BaseUIElement::getName() const {
    return m_name;
}

uint32 BaseUIElement::getID() const {
    return m_id;
}

void BaseUIElement::setPosition(float32 x, float32 y) {
    m_position[0] = x;
    m_position[1] = y;
}

void BaseUIElement::getPosition(float32& x, float32& y) const {
    x = m_position[0];
    y = m_position[1];
}

void BaseUIElement::setSize(float32 width, float32 height) {
    m_size[0] = width;
    m_size[1] = height;
}

void BaseUIElement::getSize(float32& width, float32& height) const {
    width = m_size[0];
    height = m_size[1];
}

void BaseUIElement::setAnchor(UIAnchor anchor) {
    m_anchor = anchor;
}

UIAnchor BaseUIElement::getAnchor() const {
    return m_anchor;
}

void BaseUIElement::setAlignment(UIAlignment alignment) {
    m_alignment = alignment;
}

UIAlignment BaseUIElement::getAlignment() const {
    return m_alignment;
}

void BaseUIElement::setVisible(bool visible) {
    m_visible = visible;
}

bool BaseUIElement::isVisible() const {
    return m_visible;
}

void BaseUIElement::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool BaseUIElement::isEnabled() const {
    return m_enabled;
}

void BaseUIElement::setScale(float32 scale) {
    m_scale = scale;
    
    // 更新子元素缩放
    for (auto& child : m_children) {
        child->setScale(scale);
    }
}

float32 BaseUIElement::getScale() const {
    return m_scale;
}

void BaseUIElement::setAlpha(float32 alpha) {
    m_alpha = alpha;
    
    // 更新子元素透明度
    for (auto& child : m_children) {
        child->setAlpha(alpha);
    }
}

float32 BaseUIElement::getAlpha() const {
    return m_alpha;
}

void BaseUIElement::setParent(UIElement* parent) {
    m_parent = parent;
}

UIElement* BaseUIElement::getParent() const {
    return m_parent;
}

bool BaseUIElement::addChild(UIElement* child) {
    if (!child) {
        return false;
    }
    
    // 设置子元素的父元素
    child->setParent(this);
    child->setUIManager(m_uiManager);
    
    // 添加到子元素列表
    m_children.push_back(child);
    
    std::cout << "Added child element: " << child->getName() << " to parent: " << m_name << std::endl;
    return true;
}

void BaseUIElement::removeChild(UIElement* child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        // 清理子元素
        (*it)->cleanup();
        delete *it;
        
        // 从列表中移除
        m_children.erase(it);
        
        std::cout << "Removed child element from parent: " << m_name << std::endl;
    }
}

void BaseUIElement::removeAllChildren() {
    // 清理所有子元素
    for (auto& child : m_children) {
        child->cleanup();
        delete child;
    }
    m_children.clear();
    
    std::cout << "Removed all child elements from parent: " << m_name << std::endl;
}

UIElement* BaseUIElement::getChild(uint32 id) const {
    for (auto& child : m_children) {
        if (child->getID() == id) {
            return child;
        }
    }
    return nullptr;
}

const std::vector<UIElement*>& BaseUIElement::getChildren() const {
    return m_children;
}

bool BaseUIElement::containsPoint(float32 x, float32 y) const {
    // 计算边界矩形
    Rectf bounds;
    getBounds(bounds);
    
    // 检查点是否在边界内
    return x >= bounds.x && x <= bounds.x + bounds.width && y >= bounds.y && y <= bounds.y + bounds.height;
}

void BaseUIElement::getBounds(Rectf& bounds) const {
    // 计算边界矩形
    calculateBounds();
    
    // 设置边界
    bounds.x = m_position[0];
    bounds.y = m_position[1];
    bounds.width = m_size[0];
    bounds.height = m_size[1];
}

void BaseUIElement::resize(float32 width, float32 height) {
    // 设置新大小
    setSize(width, height);
    
    // 调整子元素大小
    resizeChildren(width, height);
}

UIManager* BaseUIElement::getUIManager() const {
    return m_uiManager;
}

void BaseUIElement::setUIManager(UIManager* uiManager) {
    m_uiManager = uiManager;
    
    // 设置子元素的UI管理器
    for (auto& child : m_children) {
        child->setUIManager(uiManager);
    }
}

void BaseUIElement::calculateAbsolutePosition() {
    // 计算绝对位置
    if (m_parent) {
        float32 parentX, parentY;
        m_parent->getPosition(parentX, parentY);
        // 基于父元素位置计算绝对位置
        // TODO: 实现更复杂的绝对位置计算，包括锚点和对齐方式
    }
}

void BaseUIElement::calculateBounds() {
    // 计算边界矩形
    // 基于当前位置和大小计算边界
    // TODO: 实现更复杂的边界计算，包括旋转和缩放
}

void BaseUIElement::resizeChildren(float32 width, float32 height) {
    // 调整子元素大小
    for (auto& child : m_children) {
        // 基于父元素大小比例调整子元素大小
        // TODO: 实现更复杂的子元素大小调整逻辑
        float32 childWidth = width * 0.8f; // 示例：子元素宽度为父元素的80%
        float32 childHeight = height * 0.8f; // 示例：子元素高度为父元素的80%
        child->resize(childWidth, childHeight);
    }
}

// UIManager implementation
UIManager::UIManager()
    : m_hud(nullptr),
      m_inventoryUI(nullptr),
      m_shopUI(nullptr),
      m_platform(nullptr),
      m_screenWidth(1920),
      m_screenHeight(1080),
      m_uiScaleFactor(1.0f),
      m_inputType(InputType::MOUSE),
      m_deviceType(DeviceType::DESKTOP),
      m_nextElementId(1)
{
}

UIManager::~UIManager() {
    cleanup();
}

bool UIManager::init() {
    // 初始化HUD
    initHUD();
    
    // 初始化背包UI
    initInventoryUI();
    
    // 初始化商店UI
    initShopUI();
    
    // 初始化UI元素
    // TODO: 实现UI元素初始化
    
    std::cout << "UIManager initialized successfully" << std::endl;
    return true;
}

void UIManager::cleanup() {
    // 清理HUD
    if (m_hud) {
        m_hud->cleanup();
        delete m_hud;
        m_hud = nullptr;
    }
    
    // 清理背包UI
    if (m_inventoryUI) {
        m_inventoryUI->cleanup();
        delete m_inventoryUI;
        m_inventoryUI = nullptr;
    }
    
    // 清理商店UI
    if (m_shopUI) {
        m_shopUI->cleanup();
        delete m_shopUI;
        m_shopUI = nullptr;
    }
    
    // 清理UI元素
    for (auto& element : m_uiElements) {
        element->cleanup();
        delete element;
    }
    m_uiElements.clear();
    m_uiElementsById.clear();
    m_uiElementsByName.clear();
    
    std::cout << "UIManager cleaned up" << std::endl;
}

void UIManager::update(float32 deltaTime) {
    // 更新HUD
    if (m_hud) {
        m_hud->update(deltaTime);
    }
    
    // 更新背包UI
    if (m_inventoryUI) {
        m_inventoryUI->update(deltaTime);
    }
    
    // 更新商店UI
    if (m_shopUI) {
        m_shopUI->update(deltaTime);
    }
    
    // 更新UI元素
    for (auto& element : m_uiElements) {
        element->update(deltaTime);
    }
}

void UIManager::render() {
    // 渲染HUD
    if (m_hud) {
        m_hud->render();
    }
    
    // 渲染背包UI
    if (m_inventoryUI) {
        m_inventoryUI->render();
    }
    
    // 渲染商店UI
    if (m_shopUI) {
        m_shopUI->render();
    }
    
    // 渲染UI元素
    for (auto& element : m_uiElements) {
        element->render();
    }
}

bool UIManager::handleInput(int32 inputType, int32 inputValue, float32 x, float32 y) {
    // 处理商店UI输入
    if (m_shopUI && m_shopUI->isVisible()) {
        if (m_shopUI->handleInput(inputType, inputValue, x, y)) {
            return true;
        }
    }
    
    // 处理背包UI输入
    if (m_inventoryUI && m_inventoryUI->isVisible()) {
        if (m_inventoryUI->handleInput(inputType, inputValue, x, y)) {
            return true;
        }
    }
    
    // 处理HUD输入
    if (m_hud && m_hud->isVisible()) {
        if (m_hud->handleInput(inputType, inputValue, x, y)) {
            return true;
        }
    }
    
    // 处理UI元素输入
    for (auto& element : m_uiElements) {
        if (element->handleInput(inputType, inputValue, x, y)) {
            return true;
        }
    }
    
    return false;
}

bool UIManager::addUIElement(UIElement* element) {
    if (!element) {
        std::cerr << "Invalid UI element" << std::endl;
        return false;
    }
    
    // 初始化UI元素
    if (!element->init()) {
        std::cerr << "Failed to initialize UI element: " << element->getName() << std::endl;
        return false;
    }
    
    // 设置UI管理器
    element->setUIManager(this);
    
    // 添加到UI元素列表
    m_uiElements.push_back(element);
    m_uiElementsById[element->getID()] = element;
    m_uiElementsByName[element->getName()] = element;
    
    std::cout << "Added UI element: " << element->getName() << " (ID: " << element->getID() << ")" << std::endl;
    return true;
}

void UIManager::removeUIElement(UIElement* element) {
    if (!element) {
        return;
    }
    
    // 从列表中移除
    auto it = std::find(m_uiElements.begin(), m_uiElements.end(), element);
    if (it != m_uiElements.end()) {
        // 清理UI元素
        element->cleanup();
        
        // 从映射中移除
        m_uiElementsById.erase(element->getID());
        m_uiElementsByName.erase(element->getName());
        
        // 从列表中移除
        m_uiElements.erase(it);
        
        // 删除UI元素
        delete element;
        
        std::cout << "Removed UI element" << std::endl;
    }
}

void UIManager::removeAllUIElements() {
    // 清理所有UI元素
    for (auto& element : m_uiElements) {
        element->cleanup();
        delete element;
    }
    
    // 清空列表和映射
    m_uiElements.clear();
    m_uiElementsById.clear();
    m_uiElementsByName.clear();
    
    std::cout << "Removed all UI elements" << std::endl;
}

UIElement* UIManager::getUIElement(uint32 id) const {
    auto it = m_uiElementsById.find(id);
    if (it != m_uiElementsById.end()) {
        return it->second;
    }
    return nullptr;
}

UIElement* UIManager::getUIElement(const std::string& name) const {
    auto it = m_uiElementsByName.find(name);
    if (it != m_uiElementsByName.end()) {
        return it->second;
    }
    return nullptr;
}

const std::vector<UIElement*>& UIManager::getUIElements() const {
    return m_uiElements;
}

std::vector<UIElement*> UIManager::getUIElementsByType(UIElementType type) const {
    std::vector<UIElement*> elements;
    for (auto& element : m_uiElements) {
        if (element->getType() == type) {
            elements.push_back(element);
        }
    }
    return elements;
}

void UIManager::showHUD() {
    if (m_hud) {
        m_hud->show();
    }
}

void UIManager::hideHUD() {
    if (m_hud) {
        m_hud->hide();
    }
}

HUD* UIManager::getHUD() const {
    return m_hud;
}

void UIManager::showInventoryUI() {
    if (m_inventoryUI) {
        m_inventoryUI->show();
    }
}

void UIManager::hideInventoryUI() {
    if (m_inventoryUI) {
        m_inventoryUI->hide();
    }
}

InventoryUI* UIManager::getInventoryUI() const {
    return m_inventoryUI;
}

void UIManager::showShopUI() {
    if (m_shopUI) {
        m_shopUI->show();
    }
}

void UIManager::hideShopUI() {
    if (m_shopUI) {
        m_shopUI->hide();
    }
}

ShopUI* UIManager::getShopUI() const {
    return m_shopUI;
}

void UIManager::setPlatform(Platform* platform) {
    m_platform = platform;
    
    // 更新输入类型和设备类型
    if (m_platform) {
        m_inputType = m_platform->getInputType();
        m_deviceType = m_platform->getDeviceType();
        m_uiScaleFactor = m_platform->getUIScaleFactor();
        
        // 根据设备类型和输入类型调整UI
        adjustUIForDeviceType();
        adjustUIForInputType();
    }
}

Platform* UIManager::getPlatform() const {
    return m_platform;
}

void UIManager::setScreenSize(int32 width, int32 height) {
    m_screenWidth = width;
    m_screenHeight = height;
    
    // 调整UI大小
    resize(width, height);
}

void UIManager::getScreenSize(int32& width, int32& height) const {
    width = m_screenWidth;
    height = m_screenHeight;
}

void UIManager::resize(int32 width, int32 height) {
    // 设置屏幕大小
    setScreenSize(width, height);
    
    // 调整UI元素大小
    resizeUIElements(width, height);
    
    // 调整HUD大小
    if (m_hud) {
        m_hud->resize(width, height);
    }
    
    // 调整背包UI大小
    if (m_inventoryUI) {
        m_inventoryUI->resize(width, height);
    }
    
    // 调整商店UI大小
    if (m_shopUI) {
        m_shopUI->resize(width, height);
    }
    
    std::cout << "Resized UI to: " << width << "x" << height << std::endl;
}

float32 UIManager::getUIScaleFactor() const {
    return m_uiScaleFactor;
}

void UIManager::setUIScaleFactor(float32 scaleFactor) {
    m_uiScaleFactor = scaleFactor;
    
    // 调整UI元素缩放
    for (auto& element : m_uiElements) {
        element->setScale(scaleFactor);
    }
    
    // 调整HUD缩放
    if (m_hud) {
        m_hud->setUIScaleFactor(scaleFactor);
    }
    
    // 调整背包UI缩放
    if (m_inventoryUI) {
        m_inventoryUI->setUIScaleFactor(scaleFactor);
    }
    
    // 调整商店UI缩放
    if (m_shopUI) {
        m_shopUI->setUIScaleFactor(scaleFactor);
    }
    
    std::cout << "Set UI scale factor to: " << scaleFactor << std::endl;
}

InputType UIManager::getInputType() const {
    return m_inputType;
}

void UIManager::setInputType(InputType inputType) {
    m_inputType = inputType;
    
    // 根据输入类型调整UI
    adjustUIForInputType();
}

DeviceType UIManager::getDeviceType() const {
    return m_deviceType;
}

void UIManager::setDeviceType(DeviceType deviceType) {
    m_deviceType = deviceType;
    
    // 根据设备类型调整UI
    adjustUIForDeviceType();
}

UIElement* UIManager::createButton(const std::string& name, const std::string& text, float32 x, float32 y, float32 width, float32 height) {
    // 创建按钮UI元素
    // TODO: 实现按钮创建
    uint32 id = generateElementId();
    UIElement* button = new BaseUIElement(UIElementType::BUTTON, name, id);
    button->setPosition(x, y);
    button->setSize(width, height);
    return button;
}

UIElement* UIManager::createLabel(const std::string& name, const std::string& text, float32 x, float32 y, float32 fontSize) {
    // 创建标签UI元素
    // TODO: 实现标签创建
    uint32 id = generateElementId();
    UIElement* label = new BaseUIElement(UIElementType::LABEL, name, id);
    label->setPosition(x, y);
    label->setSize(fontSize * text.length() * 0.6f, fontSize * 1.2f);
    return label;
}

UIElement* UIManager::createImage(const std::string& name, const std::string& imagePath, float32 x, float32 y, float32 width, float32 height) {
    // 创建图像UI元素
    // TODO: 实现图像创建
    uint32 id = generateElementId();
    UIElement* image = new BaseUIElement(UIElementType::IMAGE, name, id);
    image->setPosition(x, y);
    image->setSize(width, height);
    return image;
}

UIElement* UIManager::createProgressBar(const std::string& name, float32 x, float32 y, float32 width, float32 height, float32 progress) {
    // 创建进度条UI元素
    // TODO: 实现进度条创建
    uint32 id = generateElementId();
    UIElement* progressBar = new BaseUIElement(UIElementType::PROGRESS_BAR, name, id);
    progressBar->setPosition(x, y);
    progressBar->setSize(width, height);
    return progressBar;
}

UIElement* UIManager::createSlider(const std::string& name, float32 x, float32 y, float32 width, float32 height, float32 value) {
    // 创建滑块UI元素
    // TODO: 实现滑块创建
    uint32 id = generateElementId();
    UIElement* slider = new BaseUIElement(UIElementType::SLIDER, name, id);
    slider->setPosition(x, y);
    slider->setSize(width, height);
    return slider;
}

UIElement* UIManager::createCheckbox(const std::string& name, const std::string& text, float32 x, float32 y, bool checked) {
    // 创建复选框UI元素
    // TODO: 实现复选框创建
    uint32 id = generateElementId();
    UIElement* checkbox = new BaseUIElement(UIElementType::CHECKBOX, name, id);
    checkbox->setPosition(x, y);
    checkbox->setSize(20.0f, 20.0f);
    return checkbox;
}

UIElement* UIManager::createRadioButton(const std::string& name, const std::string& text, float32 x, float32 y, bool checked) {
    // 创建单选按钮UI元素
    // TODO: 实现单选按钮创建
    uint32 id = generateElementId();
    UIElement* radioButton = new BaseUIElement(UIElementType::RADIO_BUTTON, name, id);
    radioButton->setPosition(x, y);
    radioButton->setSize(20.0f, 20.0f);
    return radioButton;
}

UIElement* UIManager::createTextInput(const std::string& name, float32 x, float32 y, float32 width, float32 height, const std::string& text) {
    // 创建文本输入UI元素
    // TODO: 实现文本输入创建
    uint32 id = generateElementId();
    UIElement* textInput = new BaseUIElement(UIElementType::TEXT_INPUT, name, id);
    textInput->setPosition(x, y);
    textInput->setSize(width, height);
    return textInput;
}

UIElement* UIManager::createPanel(const std::string& name, float32 x, float32 y, float32 width, float32 height) {
    // 创建面板UI元素
    // TODO: 实现面板创建
    uint32 id = generateElementId();
    UIElement* panel = new BaseUIElement(UIElementType::PANEL, name, id);
    panel->setPosition(x, y);
    panel->setSize(width, height);
    return panel;
}

UIElement* UIManager::createWindow(const std::string& name, const std::string& title, float32 x, float32 y, float32 width, float32 height) {
    // 创建窗口UI元素
    // TODO: 实现窗口创建
    uint32 id = generateElementId();
    UIElement* window = new BaseUIElement(UIElementType::WINDOW, name, id);
    window->setPosition(x, y);
    window->setSize(width, height);
    return window;
}

UIElement* UIManager::createScrollView(const std::string& name, float32 x, float32 y, float32 width, float32 height) {
    // 创建滚动视图UI元素
    // TODO: 实现滚动视图创建
    uint32 id = generateElementId();
    UIElement* scrollView = new BaseUIElement(UIElementType::SCROLL_VIEW, name, id);
    scrollView->setPosition(x, y);
    scrollView->setSize(width, height);
    return scrollView;
}

UIElement* UIManager::createGridView(const std::string& name, float32 x, float32 y, float32 width, float32 height, int32 rows, int32 columns) {
    // 创建网格视图UI元素
    // TODO: 实现网格视图创建
    uint32 id = generateElementId();
    UIElement* gridView = new BaseUIElement(UIElementType::GRID_VIEW, name, id);
    gridView->setPosition(x, y);
    gridView->setSize(width, height);
    return gridView;
}

UIElement* UIManager::createListView(const std::string& name, float32 x, float32 y, float32 width, float32 height) {
    // 创建列表视图UI元素
    // TODO: 实现列表视图创建
    uint32 id = generateElementId();
    UIElement* listView = new BaseUIElement(UIElementType::LIST_VIEW, name, id);
    listView->setPosition(x, y);
    listView->setSize(width, height);
    return listView;
}

void UIManager::initHUD() {
    // 创建HUD
    m_hud = new HUD();
    m_hud->setUIManager(this);
    m_hud->init();
    
    std::cout << "Initialized HUD" << std::endl;
}

void UIManager::initInventoryUI() {
    // 创建背包UI
    m_inventoryUI = new InventoryUI();
    m_inventoryUI->setUIManager(this);
    m_inventoryUI->init();
    m_inventoryUI->hide();
    
    std::cout << "Initialized InventoryUI" << std::endl;
}

void UIManager::initShopUI() {
    // 创建商店UI
    m_shopUI = new ShopUI();
    m_shopUI->setUIManager(this);
    m_shopUI->init();
    m_shopUI->hide();
    
    std::cout << "Initialized ShopUI" << std::endl;
}

uint32 UIManager::generateElementId() {
    return m_nextElementId++;
}

void UIManager::resizeUIElements(int32 width, int32 height) {
    // 调整UI元素大小
    for (auto& element : m_uiElements) {
        // 根据屏幕大小调整UI元素大小
        // TODO: 实现响应式调整逻辑
        float32 scaleX = static_cast<float32>(width) / 1920.0f;
        float32 scaleY = static_cast<float32>(height) / 1080.0f;
        float32 scale = std::min(scaleX, scaleY);
        
        // 调整元素缩放
        element->setScale(scale * m_uiScaleFactor);
    }
}

void UIManager::adjustUIForDeviceType() {
    // 根据设备类型调整UI
    switch (m_deviceType) {
        case DeviceType::SMARTPHONE:
            // 智能手机：增大UI元素，简化布局
            setUIScaleFactor(1.5f);
            break;
        case DeviceType::TABLET:
            // 平板电脑：中等UI元素大小
            setUIScaleFactor(1.25f);
            break;
        case DeviceType::LAPTOP:
        case DeviceType::DESKTOP:
        default:
            // 桌面设备：标准UI元素大小
            setUIScaleFactor(1.0f);
            break;
    }
    
    // 调整HUD
    if (m_hud) {
        m_hud->adjustForDeviceType(m_deviceType);
    }
    
    // 调整背包UI
    if (m_inventoryUI) {
        m_inventoryUI->adjustForDeviceType(m_deviceType);
    }
    
    // 调整商店UI
    if (m_shopUI) {
        m_shopUI->adjustForDeviceType(m_deviceType);
    }
    
    std::cout << "Adjusted UI for device type: " << static_cast<int>(m_deviceType) << std::endl;
}

void UIManager::adjustUIForInputType() {
    // 根据输入类型调整UI
    switch (m_inputType) {
        case InputType::TOUCH:
            // 触摸输入：增大点击区域
            // TODO: 实现触摸输入调整
            break;
        case InputType::MOUSE:
        case InputType::BOTH:
        default:
            // 鼠标输入：标准点击区域
            // TODO: 实现鼠标输入调整
            break;
    }
    
    // 调整HUD
    if (m_hud) {
        m_hud->adjustForInputType(m_inputType);
    }
    
    // 调整背包UI
    if (m_inventoryUI) {
        m_inventoryUI->adjustForInputType(m_inputType);
    }
    
    // 调整商店UI
    if (m_shopUI) {
        m_shopUI->adjustForInputType(m_inputType);
    }
    
    std::cout << "Adjusted UI for input type: " << static_cast<int>(m_inputType) << std::endl;
}

// Global UI manager instance
UIManager* g_uiManager = nullptr;

} // namespace FishingGame
