#include "fishing/ui/InventoryUI.h"
#include "fishing/ui/UIManager.h"
#include <iostream>

namespace FishingGame {

InventoryUI::InventoryUI()
    : m_uiManager(nullptr),
      m_playerData(nullptr),
      m_visible(false),
      m_width(1920),
      m_height(1080),
      m_uiScaleFactor(1.0f),
      m_inventorySlots(20),
      m_inventoryCapacity(50),
      m_inventoryPanel(nullptr),
      m_inventoryTitle(nullptr),
      m_equipmentPanel(nullptr),
      m_equipmentTitle(nullptr),
      m_closeButton(nullptr)
{
}

InventoryUI::~InventoryUI() {
    cleanup();
}

bool InventoryUI::init() {
    // 初始化UI元素
    initUIElements();
    
    // 初始化背包格子
    initInventorySlots();
    
    // 初始化装备格子
    initEquipmentSlots();
    
    std::cout << "InventoryUI initialized" << std::endl;
    return true;
}

void InventoryUI::cleanup() {
    // 清理UI元素
    for (auto& element : m_uiElements) {
        element->cleanup();
        delete element;
    }
    m_uiElements.clear();
    
    // 清理背包格子
    for (auto& slot : m_inventorySlots) {
        slot->cleanup();
        delete slot;
    }
    m_inventorySlots.clear();
    
    // 清理装备格子
    for (auto& pair : m_equipmentSlots) {
        pair.second->cleanup();
        delete pair.second;
    }
    m_equipmentSlots.clear();
    
    // 重置指针
    m_inventoryPanel = nullptr;
    m_inventoryTitle = nullptr;
    m_equipmentPanel = nullptr;
    m_equipmentTitle = nullptr;
    m_closeButton = nullptr;
    
    // 清空装备的物品
    m_equippedItems.clear();
    
    std::cout << "InventoryUI cleaned up" << std::endl;
}

void InventoryUI::update(float32 deltaTime) {
    if (!m_visible) {
        return;
    }
    
    // 更新UI元素
    for (auto& element : m_uiElements) {
        element->update(deltaTime);
    }
    
    // 更新背包格子
    updateInventorySlots();
    
    // 更新装备格子
    updateEquipmentSlots();
}

void InventoryUI::render() {
    if (!m_visible) {
        return;
    }
    
    // 渲染UI元素
    for (auto& element : m_uiElements) {
        element->render();
    }
    
    // 渲染背包格子
    for (auto& slot : m_inventorySlots) {
        slot->render();
    }
    
    // 渲染装备格子
    for (auto& pair : m_equipmentSlots) {
        pair.second->render();
    }
}

bool InventoryUI::handleInput(int32 inputType, int32 inputValue, float32 x, float32 y) {
    if (!m_visible) {
        return false;
    }
    
    // 处理关闭按钮输入
    if (m_closeButton && m_closeButton->containsPoint(x, y)) {
        hide();
        return true;
    }
    
    // 处理背包格子输入
    for (size_t i = 0; i < m_inventorySlots.size(); ++i) {
        if (m_inventorySlots[i]->containsPoint(x, y)) {
            // TODO: 处理背包格子点击
            return true;
        }
    }
    
    // 处理装备格子输入
    for (auto& pair : m_equipmentSlots) {
        if (pair.second->containsPoint(x, y)) {
            // TODO: 处理装备格子点击
            return true;
        }
    }
    
    // 处理其他UI元素输入
    for (auto& element : m_uiElements) {
        if (element->handleInput(inputType, inputValue, x, y)) {
            return true;
        }
    }
    
    return false;
}

void InventoryUI::setUIManager(UIManager* uiManager) {
    m_uiManager = uiManager;
}

UIManager* InventoryUI::getUIManager() const {
    return m_uiManager;
}

void InventoryUI::setPlayerData(PlayerData* playerData) {
    m_playerData = playerData;
}

PlayerData* InventoryUI::getPlayerData() const {
    return m_playerData;
}

void InventoryUI::show() {
    m_visible = true;
    
    // 刷新背包内容
    refreshInventory();
}

void InventoryUI::hide() {
    m_visible = false;
}

bool InventoryUI::isVisible() const {
    return m_visible;
}

void InventoryUI::setVisible(bool visible) {
    m_visible = visible;
}

void InventoryUI::refreshInventory() {
    // 刷新背包内容
    updateInventorySlots();
    updateEquipmentSlots();
}

bool InventoryUI::addItem(ItemID itemId, int32 count) {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现添加物品到背包
    return true;
}

bool InventoryUI::removeItem(ItemID itemId, int32 count) {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现从背包移除物品
    return true;
}

bool InventoryUI::useItem(ItemID itemId) {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现使用物品
    return true;
}

bool InventoryUI::equipItem(ItemID itemId) {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现装备物品
    return true;
}

bool InventoryUI::unequipItem(ItemType type) {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现卸下物品
    return true;
}

bool InventoryUI::hasItem(ItemID itemId) const {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现检查物品是否在背包中
    return false;
}

int32 InventoryUI::getItemCount(ItemID itemId) const {
    if (!m_playerData) {
        return 0;
    }
    
    // TODO: 实现获取物品数量
    return 0;
}

const std::vector<ItemID>& InventoryUI::getInventoryItems() const {
    static std::vector<ItemID> empty;
    if (!m_playerData) {
        return empty;
    }
    
    // TODO: 实现获取背包中所有物品
    return empty;
}

ItemID InventoryUI::getEquippedItem(ItemType type) const {
    auto it = m_equippedItems.find(type);
    if (it != m_equippedItems.end()) {
        return it->second;
    }
    return ItemID::NONE;
}

void InventoryUI::setEquippedItem(ItemType type, ItemID itemId) {
    m_equippedItems[type] = itemId;
}

void InventoryUI::resize(int32 width, int32 height) {
    // 设置新大小
    m_width = width;
    m_height = height;
    
    // 调整UI元素大小
    resizeUIElements(width, height);
    
    // 重新计算背包格子位置
    calculateInventorySlotPositions();
    
    // 重新计算装备格子位置
    calculateEquipmentSlotPositions();
}

int32 InventoryUI::getWidth() const {
    return m_width;
}

int32 InventoryUI::getHeight() const {
    return m_height;
}

void InventoryUI::setSize(int32 width, int32 height) {
    m_width = width;
    m_height = height;
}

float32 InventoryUI::getUIScaleFactor() const {
    return m_uiScaleFactor;
}

void InventoryUI::setUIScaleFactor(float32 scaleFactor) {
    m_uiScaleFactor = scaleFactor;
    
    // 调整UI元素缩放
    for (auto& element : m_uiElements) {
        element->setScale(scaleFactor);
    }
    
    // 调整背包格子缩放
    for (auto& slot : m_inventorySlots) {
        slot->setScale(scaleFactor);
    }
    
    // 调整装备格子缩放
    for (auto& pair : m_equipmentSlots) {
        pair.second->setScale(scaleFactor);
    }
}

void InventoryUI::adjustForDeviceType(DeviceType deviceType) {
    // 根据设备类型调整UI元素
    adjustUIElementsForDeviceType(deviceType);
}

void InventoryUI::adjustForInputType(InputType inputType) {
    // 根据输入类型调整UI元素
    adjustUIElementsForInputType(inputType);
}

void InventoryUI::setInventorySlots(int32 slots) {
    m_inventorySlots = slots;
    
    // 重新初始化背包格子
    initInventorySlots();
}

int32 InventoryUI::getInventorySlots() const {
    return m_inventorySlots;
}

void InventoryUI::setInventoryCapacity(int32 capacity) {
    m_inventoryCapacity = capacity;
}

int32 InventoryUI::getInventoryCapacity() const {
    return m_inventoryCapacity;
}

void InventoryUI::initUIElements() {
    if (!m_uiManager) {
        return;
    }
    
    // 创建背包面板
    m_inventoryPanel = m_uiManager->createPanel("inventoryPanel", m_width / 2 - 350, m_height / 2 - 250, 700, 500);
    m_uiElements.push_back(m_inventoryPanel);
    
    // 创建背包标题
    m_inventoryTitle = m_uiManager->createLabel("inventoryTitle", "背包", m_width / 2 - 300, m_height / 2 - 230, 24);
    m_uiElements.push_back(m_inventoryTitle);
    
    // 创建装备面板
    m_equipmentPanel = m_uiManager->createPanel("equipmentPanel", m_width / 2 + 100, m_height / 2 - 200, 200, 400);
    m_uiElements.push_back(m_equipmentPanel);
    
    // 创建装备标题
    m_equipmentTitle = m_uiManager->createLabel("equipmentTitle", "装备", m_width / 2 + 150, m_height / 2 - 180, 20);
    m_uiElements.push_back(m_equipmentTitle);
    
    // 创建关闭按钮
    m_closeButton = m_uiManager->createButton("closeButton", "关闭", m_width / 2 + 250, m_height / 2 - 230, 80, 40);
    m_uiElements.push_back(m_closeButton);
}

void InventoryUI::initInventorySlots() {
    if (!m_uiManager) {
        return;
    }
    
    // 清理现有背包格子
    for (auto& slot : m_inventorySlots) {
        slot->cleanup();
        delete slot;
    }
    m_inventorySlots.clear();
    
    // 创建新的背包格子
    int32 slotsPerRow = 5;
    int32 slotSize = 60;
    int32 slotPadding = 10;
    
    for (int32 i = 0; i < m_inventorySlots; ++i) {
        int32 row = i / slotsPerRow;
        int32 col = i % slotsPerRow;
        float32 x = m_width / 2 - 300 + col * (slotSize + slotPadding);
        float32 y = m_height / 2 - 200 + row * (slotSize + slotPadding);
        
        UIElement* slot = m_uiManager->createPanel("inventorySlot" + std::to_string(i), x, y, slotSize, slotSize);
        m_inventorySlots.push_back(slot);
    }
    
    // 计算背包格子位置
    calculateInventorySlotPositions();
}

void InventoryUI::initEquipmentSlots() {
    if (!m_uiManager) {
        return;
    }
    
    // 清理现有装备格子
    for (auto& pair : m_equipmentSlots) {
        pair.second->cleanup();
        delete pair.second;
    }
    m_equipmentSlots.clear();
    
    // 创建装备格子
    int32 slotSize = 50;
    int32 slotPadding = 15;
    
    // 鱼竿
    UIElement* rodSlot = m_uiManager->createPanel("rodSlot", m_width / 2 + 125, m_height / 2 - 150, slotSize, slotSize);
    m_equipmentSlots[ItemType::ROD] = rodSlot;
    
    // 鱼线
    UIElement* lineSlot = m_uiManager->createPanel("lineSlot", m_width / 2 + 125, m_height / 2 - 85, slotSize, slotSize);
    m_equipmentSlots[ItemType::LINE] = lineSlot;
    
    // 鱼钩
    UIElement* hookSlot = m_uiManager->createPanel("hookSlot", m_width / 2 + 125, m_height / 2 - 20, slotSize, slotSize);
    m_equipmentSlots[ItemType::HOOK] = hookSlot;
    
    // 鱼饵
    UIElement* baitSlot = m_uiManager->createPanel("baitSlot", m_width / 2 + 125, m_height / 2 + 45, slotSize, slotSize);
    m_equipmentSlots[ItemType::BAIT] = baitSlot;
    
    // 计算装备格子位置
    calculateEquipmentSlotPositions();
}

void InventoryUI::updateInventorySlots() {
    // 更新背包格子
    // TODO: 实现背包格子更新逻辑
}

void InventoryUI::updateEquipmentSlots() {
    // 更新装备格子
    // TODO: 实现装备格子更新逻辑
}

void InventoryUI::resizeUIElements(int32 width, int32 height) {
    // 调整背包面板
    if (m_inventoryPanel) {
        m_inventoryPanel->setPosition(width / 2 - 350, height / 2 - 250);
    }
    
    // 调整背包标题
    if (m_inventoryTitle) {
        m_inventoryTitle->setPosition(width / 2 - 300, height / 2 - 230);
    }
    
    // 调整装备面板
    if (m_equipmentPanel) {
        m_equipmentPanel->setPosition(width / 2 + 100, height / 2 - 200);
    }
    
    // 调整装备标题
    if (m_equipmentTitle) {
        m_equipmentTitle->setPosition(width / 2 + 150, height / 2 - 180);
    }
    
    // 调整关闭按钮
    if (m_closeButton) {
        m_closeButton->setPosition(width / 2 + 250, height / 2 - 230);
    }
}

void InventoryUI::adjustUIElementsForDeviceType(DeviceType deviceType) {
    // 根据设备类型调整UI元素
    switch (deviceType) {
        case DeviceType::SMARTPHONE:
            // 智能手机：增大UI元素，简化布局
            setUIScaleFactor(m_uiScaleFactor * 1.5f);
            break;
        case DeviceType::TABLET:
            // 平板电脑：中等UI元素大小
            setUIScaleFactor(m_uiScaleFactor * 1.25f);
            break;
        case DeviceType::LAPTOP:
        case DeviceType::DESKTOP:
        default:
            // 桌面设备：标准UI元素大小
            setUIScaleFactor(m_uiScaleFactor);
            break;
    }
}

void InventoryUI::adjustUIElementsForInputType(InputType inputType) {
    // 根据输入类型调整UI元素
    switch (inputType) {
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
}

void InventoryUI::calculateInventorySlotPositions() {
    // 计算背包格子位置
    int32 slotsPerRow = 5;
    int32 slotSize = 60;
    int32 slotPadding = 10;
    
    for (size_t i = 0; i < m_inventorySlots.size(); ++i) {
        int32 row = i / slotsPerRow;
        int32 col = i % slotsPerRow;
        float32 x = m_width / 2 - 300 + col * (slotSize + slotPadding);
        float32 y = m_height / 2 - 200 + row * (slotSize + slotPadding);
        
        m_inventorySlots[i]->setPosition(x, y);
    }
}

void InventoryUI::calculateEquipmentSlotPositions() {
    // 计算装备格子位置
    int32 slotSize = 50;
    int32 slotPadding = 15;
    
    // 鱼竿
    if (m_equipmentSlots.find(ItemType::ROD) != m_equipmentSlots.end()) {
        m_equipmentSlots[ItemType::ROD]->setPosition(m_width / 2 + 125, m_height / 2 - 150);
    }
    
    // 鱼线
    if (m_equipmentSlots.find(ItemType::LINE) != m_equipmentSlots.end()) {
        m_equipmentSlots[ItemType::LINE]->setPosition(m_width / 2 + 125, m_height / 2 - 85);
    }
    
    // 鱼钩
    if (m_equipmentSlots.find(ItemType::HOOK) != m_equipmentSlots.end()) {
        m_equipmentSlots[ItemType::HOOK]->setPosition(m_width / 2 + 125, m_height / 2 - 20);
    }
    
    // 鱼饵
    if (m_equipmentSlots.find(ItemType::BAIT) != m_equipmentSlots.end()) {
        m_equipmentSlots[ItemType::BAIT]->setPosition(m_width / 2 + 125, m_height / 2 + 45);
    }
}

const Item* InventoryUI::getItemInfo(ItemID itemId) const {
    // TODO: 实现获取物品信息
    return nullptr;
}

std::string InventoryUI::getEquipmentTypeName(ItemType type) const {
    switch (type) {
        case ItemType::ROD:
            return "鱼竿";
        case ItemType::LINE:
            return "鱼线";
        case ItemType::HOOK:
            return "鱼钩";
        case ItemType::BAIT:
            return "鱼饵";
        default:
            return "未知";
    }
}

void InventoryUI::getEquipmentSlotPosition(ItemType type, float32& x, float32& y) const {
    auto it = m_equipmentSlots.find(type);
    if (it != m_equipmentSlots.end()) {
        it->second->getPosition(x, y);
    } else {
        x = 0;
        y = 0;
    }
}

bool InventoryUI::handleItemClick(ItemID itemId, float32 x, float32 y) {
    // TODO: 实现处理物品点击
    return false;
}

bool InventoryUI::handleEquipmentClick(ItemType type, float32 x, float32 y) {
    // TODO: 实现处理装备点击
    return false;
}

void InventoryUI::showItemInfo(ItemID itemId) {
    // TODO: 实现显示物品信息
}

void InventoryUI::hideItemInfo() {
    // TODO: 实现隐藏物品信息
}

} // namespace FishingGame
