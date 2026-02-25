#ifndef INVENTORY_UI_H
#define INVENTORY_UI_H

#include "fishing/core/Types.h"
#include "fishing/core/DataStructures.h"
#include "fishing/ui/UIManager.h"
#include <string>
#include <vector>
#include <map>

namespace FishingGame {

// 前向声明
class UIManager;
class PlayerData;

// 背包UI类
class InventoryUI {
public:
    InventoryUI();
    ~InventoryUI();

    // 初始化背包UI
    bool init();

    // 清理背包UI
    void cleanup();

    // 更新背包UI
    void update(float32 deltaTime);

    // 渲染背包UI
    void render();

    // 处理输入
    bool handleInput(int32 inputType, int32 inputValue, float32 x, float32 y);

    // 设置UI管理器
    void setUIManager(UIManager* uiManager);

    // 获取UI管理器
    UIManager* getUIManager() const;

    // 设置玩家数据
    void setPlayerData(PlayerData* playerData);

    // 获取玩家数据
    PlayerData* getPlayerData() const;

    // 显示背包UI
    void show();

    // 隐藏背包UI
    void hide();

    // 检查背包UI是否可见
    bool isVisible() const;

    // 设置背包UI可见性
    void setVisible(bool visible);

    // 刷新背包内容
    void refreshInventory();

    // 添加物品到背包
    bool addItem(ItemID itemId, int32 count = 1);

    // 从背包移除物品
    bool removeItem(ItemID itemId, int32 count = 1);

    // 使用物品
    bool useItem(ItemID itemId);

    // 装备物品
    bool equipItem(ItemID itemId);

    // 卸下物品
    bool unequipItem(ItemType type);

    // 检查物品是否在背包中
    bool hasItem(ItemID itemId) const;

    // 获取物品数量
    int32 getItemCount(ItemID itemId) const;

    // 获取背包中所有物品
    const std::vector<ItemID>& getInventoryItems() const;

    // 获取装备的物品
    ItemID getEquippedItem(ItemType type) const;

    // 设置装备的物品
    void setEquippedItem(ItemType type, ItemID itemId);

    // 响应式调整大小
    void resize(int32 width, int32 height);

    // 获取背包UI宽度
    int32 getWidth() const;

    // 获取背包UI高度
    int32 getHeight() const;

    // 设置背包UI大小
    void setSize(int32 width, int32 height);

    // 获取UI缩放因子
    float32 getUIScaleFactor() const;

    // 设置UI缩放因子
    void setUIScaleFactor(float32 scaleFactor);

    // 根据设备类型调整背包UI
    void adjustForDeviceType(DeviceType deviceType);

    // 根据输入类型调整背包UI
    void adjustForInputType(InputType inputType);

    // 设置背包格子数量
    void setInventorySlots(int32 slots);

    // 获取背包格子数量
    int32 getInventorySlots() const;

    // 设置背包容量
    void setInventoryCapacity(int32 capacity);

    // 获取背包容量
    int32 getInventoryCapacity() const;

private:
    // UI管理器
    UIManager* m_uiManager;

    // 玩家数据
    PlayerData* m_playerData;

    // 背包UI可见性
    bool m_visible;

    // 背包UI大小
    int32 m_width;
    int32 m_height;

    // UI缩放因子
    float32 m_uiScaleFactor;

    // 背包格子数量
    int32 m_inventorySlots;

    // 背包容量
    int32 m_inventoryCapacity;

    // 装备的物品
    std::map<ItemType, ItemID> m_equippedItems;

    // UI元素
    std::vector<UIElement*> m_uiElements;

    // 背包面板
    UIElement* m_inventoryPanel;

    // 背包标题
    UIElement* m_inventoryTitle;

    // 背包格子
    std::vector<UIElement*> m_inventorySlots;

    // 装备面板
    UIElement* m_equipmentPanel;

    // 装备标题
    UIElement* m_equipmentTitle;

    // 装备格子
    std::map<ItemType, UIElement*> m_equipmentSlots;

    // 关闭按钮
    UIElement* m_closeButton;

    // 初始化UI元素
    void initUIElements();

    // 初始化背包格子
    void initInventorySlots();

    // 初始化装备格子
    void initEquipmentSlots();

    // 更新背包格子
    void updateInventorySlots();

    // 更新装备格子
    void updateEquipmentSlots();

    // 响应式调整UI元素大小
    void resizeUIElements(int32 width, int32 height);

    // 根据设备类型调整UI元素
    void adjustUIElementsForDeviceType(DeviceType deviceType);

    // 根据输入类型调整UI元素
    void adjustUIElementsForInputType(InputType inputType);

    // 计算背包格子位置
    void calculateInventorySlotPositions();

    // 计算装备格子位置
    void calculateEquipmentSlotPositions();

    // 获取物品信息
    const Item* getItemInfo(ItemID itemId) const;

    // 获取装备类型的显示名称
    std::string getEquipmentTypeName(ItemType type) const;

    // 获取装备类型的位置
    void getEquipmentSlotPosition(ItemType type, float32& x, float32& y) const;

    // 处理物品点击
    bool handleItemClick(ItemID itemId, float32 x, float32 y);

    // 处理装备点击
    bool handleEquipmentClick(ItemType type, float32 x, float32 y);

    // 显示物品信息
    void showItemInfo(ItemID itemId);

    // 隐藏物品信息
    void hideItemInfo();
};

} // namespace FishingGame

#endif // INVENTORY_UI_H
