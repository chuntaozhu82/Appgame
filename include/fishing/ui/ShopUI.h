#ifndef SHOP_UI_H
#define SHOP_UI_H

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
class EconomySystem;

// 商店UI类
class ShopUI {
public:
    ShopUI();
    ~ShopUI();

    // 初始化商店UI
    bool init();

    // 清理商店UI
    void cleanup();

    // 更新商店UI
    void update(float32 deltaTime);

    // 渲染商店UI
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

    // 设置经济系统
    void setEconomySystem(EconomySystem* economySystem);

    // 获取经济系统
    EconomySystem* getEconomySystem() const;

    // 显示商店UI
    void show();

    // 隐藏商店UI
    void hide();

    // 检查商店UI是否可见
    bool isVisible() const;

    // 设置商店UI可见性
    void setVisible(bool visible);

    // 刷新商店内容
    void refreshShop();

    // 添加商品到商店
    bool addShopItem(ItemID itemId, int32 price, int32 stock = 1);

    // 从商店移除商品
    bool removeShopItem(ItemID itemId);

    // 更新商品价格
    bool updateItemPrice(ItemID itemId, int32 price);

    // 更新商品库存
    bool updateItemStock(ItemID itemId, int32 stock);

    // 购买商品
    bool buyItem(ItemID itemId, int32 count = 1);

    // 出售商品
    bool sellItem(ItemID itemId, int32 count = 1);

    // 检查商品是否在商店中
    bool hasShopItem(ItemID itemId) const;

    // 获取商品价格
    int32 getItemPrice(ItemID itemId) const;

    // 获取商品库存
    int32 getItemStock(ItemID itemId) const;

    // 获取商店中所有商品
    const std::vector<ShopItem>& getShopItems() const;

    // 响应式调整大小
    void resize(int32 width, int32 height);

    // 获取商店UI宽度
    int32 getWidth() const;

    // 获取商店UI高度
    int32 getHeight() const;

    // 设置商店UI大小
    void setSize(int32 width, int32 height);

    // 获取UI缩放因子
    float32 getUIScaleFactor() const;

    // 设置UI缩放因子
    void setUIScaleFactor(float32 scaleFactor);

    // 根据设备类型调整商店UI
    void adjustForDeviceType(DeviceType deviceType);

    // 根据输入类型调整商店UI
    void adjustForInputType(InputType inputType);

    // 设置商店页面数量
    void setShopPages(int32 pages);

    // 获取商店页面数量
    int32 getShopPages() const;

    // 设置当前商店页面
    void setCurrentShopPage(int32 page);

    // 获取当前商店页面
    int32 getCurrentShopPage() const;

    // 切换到下一页
    bool nextPage();

    // 切换到上一页
    bool previousPage();

private:
    // UI管理器
    UIManager* m_uiManager;

    // 玩家数据
    PlayerData* m_playerData;

    // 经济系统
    EconomySystem* m_economySystem;

    // 商店UI可见性
    bool m_visible;

    // 商店UI大小
    int32 m_width;
    int32 m_height;

    // UI缩放因子
    float32 m_uiScaleFactor;

    // 商店页面数量
    int32 m_shopPages;

    // 当前商店页面
    int32 m_currentShopPage;

    // 商店商品
    std::vector<ShopItem> m_shopItems;

    // 商品映射（按ID）
    std::map<ItemID, ShopItem*> m_shopItemsById;

    // UI元素
    std::vector<UIElement*> m_uiElements;

    // 商店面板
    UIElement* m_shopPanel;

    // 商店标题
    UIElement* m_shopTitle;

    // 商品格子
    std::vector<UIElement*> m_shopSlots;

    // 页面导航
    UIElement* m_previousPageButton;
    UIElement* m_nextPageButton;
    UIElement* m_pageIndicator;

    // 关闭按钮
    UIElement* m_closeButton;

    // 初始化UI元素
    void initUIElements();

    // 初始化商品格子
    void initShopSlots();

    // 初始化页面导航
    void initPageNavigation();

    // 更新商品格子
    void updateShopSlots();

    // 更新页面导航
    void updatePageNavigation();

    // 响应式调整UI元素大小
    void resizeUIElements(int32 width, int32 height);

    // 根据设备类型调整UI元素
    void adjustUIElementsForDeviceType(DeviceType deviceType);

    // 根据输入类型调整UI元素
    void adjustUIElementsForInputType(InputType inputType);

    // 计算商品格子位置
    void calculateShopSlotPositions();

    // 获取物品信息
    const Item* getItemInfo(ItemID itemId) const;

    // 处理商品点击
    bool handleShopItemClick(ItemID itemId, float32 x, float32 y);

    // 显示商品信息
    void showItemInfo(ItemID itemId);

    // 隐藏商品信息
    void hideItemInfo();

    // 检查玩家是否有足够的钱
    bool hasEnoughMoney(int32 amount) const;

    // 检查背包是否有足够的空间
    bool hasEnoughInventorySpace(ItemID itemId, int32 count = 1) const;

    // 获取当前页面的商品
    std::vector<ShopItem> getCurrentPageItems() const;

    // 计算页面数量
    int32 calculatePageCount() const;
};

} // namespace FishingGame

#endif // SHOP_UI_H
