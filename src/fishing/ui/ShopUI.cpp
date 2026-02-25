#include "fishing/ui/ShopUI.h"
#include "fishing/ui/UIManager.h"
#include <iostream>

namespace FishingGame {

ShopUI::ShopUI()
    : m_uiManager(nullptr),
      m_playerData(nullptr),
      m_economySystem(nullptr),
      m_visible(false),
      m_width(1920),
      m_height(1080),
      m_uiScaleFactor(1.0f),
      m_shopPages(1),
      m_currentShopPage(1),
      m_shopPanel(nullptr),
      m_shopTitle(nullptr),
      m_previousPageButton(nullptr),
      m_nextPageButton(nullptr),
      m_pageIndicator(nullptr),
      m_closeButton(nullptr)
{
}

ShopUI::~ShopUI() {
    cleanup();
}

bool ShopUI::init() {
    // 初始化UI元素
    initUIElements();
    
    // 初始化商品格子
    initShopSlots();
    
    // 初始化页面导航
    initPageNavigation();
    
    std::cout << "ShopUI initialized" << std::endl;
    return true;
}

void ShopUI::cleanup() {
    // 清理UI元素
    for (auto& element : m_uiElements) {
        element->cleanup();
        delete element;
    }
    m_uiElements.clear();
    
    // 清理商品格子
    for (auto& slot : m_shopSlots) {
        slot->cleanup();
        delete slot;
    }
    m_shopSlots.clear();
    
    // 重置指针
    m_shopPanel = nullptr;
    m_shopTitle = nullptr;
    m_previousPageButton = nullptr;
    m_nextPageButton = nullptr;
    m_pageIndicator = nullptr;
    m_closeButton = nullptr;
    
    // 清空商品
    m_shopItems.clear();
    m_shopItemsById.clear();
    
    std::cout << "ShopUI cleaned up" << std::endl;
}

void ShopUI::update(float32 deltaTime) {
    if (!m_visible) {
        return;
    }
    
    // 更新UI元素
    for (auto& element : m_uiElements) {
        element->update(deltaTime);
    }
    
    // 更新商品格子
    updateShopSlots();
    
    // 更新页面导航
    updatePageNavigation();
}

void ShopUI::render() {
    if (!m_visible) {
        return;
    }
    
    // 渲染UI元素
    for (auto& element : m_uiElements) {
        element->render();
    }
    
    // 渲染商品格子
    for (auto& slot : m_shopSlots) {
        slot->render();
    }
}

bool ShopUI::handleInput(int32 inputType, int32 inputValue, float32 x, float32 y) {
    if (!m_visible) {
        return false;
    }
    
    // 处理关闭按钮输入
    if (m_closeButton && m_closeButton->containsPoint(x, y)) {
        hide();
        return true;
    }
    
    // 处理上一页按钮输入
    if (m_previousPageButton && m_previousPageButton->containsPoint(x, y)) {
        previousPage();
        return true;
    }
    
    // 处理下一页按钮输入
    if (m_nextPageButton && m_nextPageButton->containsPoint(x, y)) {
        nextPage();
        return true;
    }
    
    // 处理商品格子输入
    for (size_t i = 0; i < m_shopSlots.size(); ++i) {
        if (m_shopSlots[i]->containsPoint(x, y)) {
            // TODO: 处理商品格子点击
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

void ShopUI::setUIManager(UIManager* uiManager) {
    m_uiManager = uiManager;
}

UIManager* ShopUI::getUIManager() const {
    return m_uiManager;
}

void ShopUI::setPlayerData(PlayerData* playerData) {
    m_playerData = playerData;
}

PlayerData* ShopUI::getPlayerData() const {
    return m_playerData;
}

void ShopUI::setEconomySystem(EconomySystem* economySystem) {
    m_economySystem = economySystem;
}

EconomySystem* ShopUI::getEconomySystem() const {
    return m_economySystem;
}

void ShopUI::show() {
    m_visible = true;
    
    // 刷新商店内容
    refreshShop();
}

void ShopUI::hide() {
    m_visible = false;
}

bool ShopUI::isVisible() const {
    return m_visible;
}

void ShopUI::setVisible(bool visible) {
    m_visible = visible;
}

void ShopUI::refreshShop() {
    // 刷新商店内容
    updateShopSlots();
    updatePageNavigation();
}

bool ShopUI::addShopItem(ItemID itemId, int32 price, int32 stock) {
    // 检查商品是否已存在
    if (hasShopItem(itemId)) {
        return false;
    }
    
    // 创建新商品
    ShopItem item;
    item.itemId = itemId;
    item.price = price;
    item.stock = stock;
    
    // 添加到商店
    m_shopItems.push_back(item);
    m_shopItemsById[itemId] = &m_shopItems.back();
    
    // 更新页面数量
    m_shopPages = calculatePageCount();
    
    return true;
}

bool ShopUI::removeShopItem(ItemID itemId) {
    // 检查商品是否存在
    auto it = m_shopItemsById.find(itemId);
    if (it == m_shopItemsById.end()) {
        return false;
    }
    
    // 从商品列表中移除
    auto itemIt = std::find_if(m_shopItems.begin(), m_shopItems.end(), [itemId](const ShopItem& item) {
        return item.itemId == itemId;
    });
    if (itemIt != m_shopItems.end()) {
        m_shopItems.erase(itemIt);
    }
    
    // 从映射中移除
    m_shopItemsById.erase(it);
    
    // 更新页面数量
    m_shopPages = calculatePageCount();
    
    return true;
}

bool ShopUI::updateItemPrice(ItemID itemId, int32 price) {
    // 检查商品是否存在
    auto it = m_shopItemsById.find(itemId);
    if (it == m_shopItemsById.end()) {
        return false;
    }
    
    // 更新价格
    it->second->price = price;
    return true;
}

bool ShopUI::updateItemStock(ItemID itemId, int32 stock) {
    // 检查商品是否存在
    auto it = m_shopItemsById.find(itemId);
    if (it == m_shopItemsById.end()) {
        return false;
    }
    
    // 更新库存
    it->second->stock = stock;
    return true;
}

bool ShopUI::buyItem(ItemID itemId, int32 count) {
    // 检查商品是否存在
    auto it = m_shopItemsById.find(itemId);
    if (it == m_shopItemsById.end()) {
        return false;
    }
    
    // 检查库存是否足够
    if (it->second->stock < count) {
        return false;
    }
    
    // 检查玩家金钱是否足够
    int32 totalPrice = it->second->price * count;
    if (!hasEnoughMoney(totalPrice)) {
        return false;
    }
    
    // TODO: 实现购买逻辑
    
    return true;
}

bool ShopUI::sellItem(ItemID itemId, int32 count) {
    // TODO: 实现出售逻辑
    return false;
}

bool ShopUI::hasShopItem(ItemID itemId) const {
    return m_shopItemsById.find(itemId) != m_shopItemsById.end();
}

int32 ShopUI::getItemPrice(ItemID itemId) const {
    auto it = m_shopItemsById.find(itemId);
    if (it != m_shopItemsById.end()) {
        return it->second->price;
    }
    return 0;
}

int32 ShopUI::getItemStock(ItemID itemId) const {
    auto it = m_shopItemsById.find(itemId);
    if (it != m_shopItemsById.end()) {
        return it->second->stock;
    }
    return 0;
}

const std::vector<ShopItem>& ShopUI::getShopItems() const {
    return m_shopItems;
}

void ShopUI::resize(int32 width, int32 height) {
    // 设置新大小
    m_width = width;
    m_height = height;
    
    // 调整UI元素大小
    resizeUIElements(width, height);
    
    // 重新计算商品格子位置
    calculateShopSlotPositions();
}

int32 ShopUI::getWidth() const {
    return m_width;
}

int32 ShopUI::getHeight() const {
    return m_height;
}

void ShopUI::setSize(int32 width, int32 height) {
    m_width = width;
    m_height = height;
}

float32 ShopUI::getUIScaleFactor() const {
    return m_uiScaleFactor;
}

void ShopUI::setUIScaleFactor(float32 scaleFactor) {
    m_uiScaleFactor = scaleFactor;
    
    // 调整UI元素缩放
    for (auto& element : m_uiElements) {
        element->setScale(scaleFactor);
    }
    
    // 调整商品格子缩放
    for (auto& slot : m_shopSlots) {
        slot->setScale(scaleFactor);
    }
}

void ShopUI::adjustForDeviceType(DeviceType deviceType) {
    // 根据设备类型调整UI元素
    adjustUIElementsForDeviceType(deviceType);
}

void ShopUI::adjustForInputType(InputType inputType) {
    // 根据输入类型调整UI元素
    adjustUIElementsForInputType(inputType);
}

void ShopUI::setShopPages(int32 pages) {
    m_shopPages = pages;
}

int32 ShopUI::getShopPages() const {
    return m_shopPages;
}

void ShopUI::setCurrentShopPage(int32 page) {
    m_currentShopPage = page;
    
    // 确保页面在有效范围内
    if (m_currentShopPage < 1) {
        m_currentShopPage = 1;
    }
    if (m_currentShopPage > m_shopPages) {
        m_currentShopPage = m_shopPages;
    }
    
    // 更新商品格子
    updateShopSlots();
}

int32 ShopUI::getCurrentShopPage() const {
    return m_currentShopPage;
}

bool ShopUI::nextPage() {
    if (m_currentShopPage < m_shopPages) {
        setCurrentShopPage(m_currentShopPage + 1);
        return true;
    }
    return false;
}

bool ShopUI::previousPage() {
    if (m_currentShopPage > 1) {
        setCurrentShopPage(m_currentShopPage - 1);
        return true;
    }
    return false;
}

void ShopUI::initUIElements() {
    if (!m_uiManager) {
        return;
    }
    
    // 创建商店面板
    m_shopPanel = m_uiManager->createPanel("shopPanel", m_width / 2 - 400, m_height / 2 - 300, 800, 600);
    m_uiElements.push_back(m_shopPanel);
    
    // 创建商店标题
    m_shopTitle = m_uiManager->createLabel("shopTitle", "商店", m_width / 2 - 50, m_height / 2 - 280, 28);
    m_uiElements.push_back(m_shopTitle);
    
    // 创建关闭按钮
    m_closeButton = m_uiManager->createButton("closeButton", "关闭", m_width / 2 + 300, m_height / 2 - 280, 80, 40);
    m_uiElements.push_back(m_closeButton);
}

void ShopUI::initShopSlots() {
    if (!m_uiManager) {
        return;
    }
    
    // 清理现有商品格子
    for (auto& slot : m_shopSlots) {
        slot->cleanup();
        delete slot;
    }
    m_shopSlots.clear();
    
    // 创建新的商品格子
    int32 slotsPerRow = 4;
    int32 slotsPerPage = 12;
    int32 slotSize = 80;
    int32 slotPadding = 15;
    
    for (int32 i = 0; i < slotsPerPage; ++i) {
        int32 row = i / slotsPerRow;
        int32 col = i % slotsPerRow;
        float32 x = m_width / 2 - 350 + col * (slotSize + slotPadding);
        float32 y = m_height / 2 - 200 + row * (slotSize + slotPadding);
        
        UIElement* slot = m_uiManager->createPanel("shopSlot" + std::to_string(i), x, y, slotSize, slotSize);
        m_shopSlots.push_back(slot);
    }
    
    // 计算商品格子位置
    calculateShopSlotPositions();
}

void ShopUI::initPageNavigation() {
    if (!m_uiManager) {
        return;
    }
    
    // 创建上一页按钮
    m_previousPageButton = m_uiManager->createButton("previousPageButton", "上一页", m_width / 2 - 100, m_height / 2 + 200, 100, 40);
    m_uiElements.push_back(m_previousPageButton);
    
    // 创建下一页按钮
    m_nextPageButton = m_uiManager->createButton("nextPageButton", "下一页", m_width / 2 + 10, m_height / 2 + 200, 100, 40);
    m_uiElements.push_back(m_nextPageButton);
    
    // 创建页面指示器
    m_pageIndicator = m_uiManager->createLabel("pageIndicator", "1/1", m_width / 2 - 20, m_height / 2 + 210, 16);
    m_uiElements.push_back(m_pageIndicator);
}

void ShopUI::updateShopSlots() {
    // 获取当前页面的商品
    std::vector<ShopItem> currentPageItems = getCurrentPageItems();
    
    // 更新商品格子
    for (size_t i = 0; i < m_shopSlots.size(); ++i) {
        if (i < currentPageItems.size()) {
            // TODO: 更新商品格子显示
        } else {
            // 清空商品格子
            // TODO: 清空商品格子显示
        }
    }
}

void ShopUI::updatePageNavigation() {
    // 更新页面指示器
    if (m_pageIndicator) {
        // TODO: 更新页面指示器文本
    }
    
    // 启用/禁用页面导航按钮
    if (m_previousPageButton) {
        // TODO: 根据当前页面启用/禁用上一页按钮
    }
    if (m_nextPageButton) {
        // TODO: 根据当前页面启用/禁用下一页按钮
    }
}

void ShopUI::resizeUIElements(int32 width, int32 height) {
    // 调整商店面板
    if (m_shopPanel) {
        m_shopPanel->setPosition(width / 2 - 400, height / 2 - 300);
    }
    
    // 调整商店标题
    if (m_shopTitle) {
        m_shopTitle->setPosition(width / 2 - 50, height / 2 - 280);
    }
    
    // 调整关闭按钮
    if (m_closeButton) {
        m_closeButton->setPosition(width / 2 + 300, height / 2 - 280);
    }
    
    // 调整上一页按钮
    if (m_previousPageButton) {
        m_previousPageButton->setPosition(width / 2 - 100, height / 2 + 200);
    }
    
    // 调整下一页按钮
    if (m_nextPageButton) {
        m_nextPageButton->setPosition(width / 2 + 10, height / 2 + 200);
    }
    
    // 调整页面指示器
    if (m_pageIndicator) {
        m_pageIndicator->setPosition(width / 2 - 20, height / 2 + 210);
    }
}

void ShopUI::adjustUIElementsForDeviceType(DeviceType deviceType) {
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

void ShopUI::adjustUIElementsForInputType(InputType inputType) {
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

void ShopUI::calculateShopSlotPositions() {
    // 计算商品格子位置
    int32 slotsPerRow = 4;
    int32 slotSize = 80;
    int32 slotPadding = 15;
    
    for (size_t i = 0; i < m_shopSlots.size(); ++i) {
        int32 row = i / slotsPerRow;
        int32 col = i % slotsPerRow;
        float32 x = m_width / 2 - 350 + col * (slotSize + slotPadding);
        float32 y = m_height / 2 - 200 + row * (slotSize + slotPadding);
        
        m_shopSlots[i]->setPosition(x, y);
    }
}

const Item* ShopUI::getItemInfo(ItemID itemId) const {
    // TODO: 实现获取物品信息
    return nullptr;
}

bool ShopUI::handleShopItemClick(ItemID itemId, float32 x, float32 y) {
    // TODO: 实现处理商品点击
    return false;
}

void ShopUI::showItemInfo(ItemID itemId) {
    // TODO: 实现显示物品信息
}

void ShopUI::hideItemInfo() {
    // TODO: 实现隐藏物品信息
}

bool ShopUI::hasEnoughMoney(int32 amount) const {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现检查玩家金钱是否足够
    return false;
}

bool ShopUI::hasEnoughInventorySpace(ItemID itemId, int32 count) const {
    if (!m_playerData) {
        return false;
    }
    
    // TODO: 实现检查背包空间是否足够
    return false;
}

std::vector<ShopItem> ShopUI::getCurrentPageItems() const {
    std::vector<ShopItem> items;
    int32 itemsPerPage = 12;
    int32 startIndex = (m_currentShopPage - 1) * itemsPerPage;
    int32 endIndex = startIndex + itemsPerPage;
    
    for (int32 i = startIndex; i < endIndex && i < static_cast<int32>(m_shopItems.size()); ++i) {
        items.push_back(m_shopItems[i]);
    }
    
    return items;
}

int32 ShopUI::calculatePageCount() const {
    int32 itemsPerPage = 12;
    return (m_shopItems.size() + itemsPerPage - 1) / itemsPerPage;
}

} // namespace FishingGame
