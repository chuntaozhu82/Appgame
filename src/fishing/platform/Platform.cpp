#include "fishing/platform/Platform.h"

// 只包含默认平台实现，因为我们目前只实现了默认平台
#include "fishing/platform/default/DefaultPlatform.h"

namespace FishingGame {

// 全局平台实例
Platform* g_platform = nullptr;

// 平台工厂类实现
Platform* PlatformFactory::createPlatform() {
    // 目前只使用默认平台实现
    return new DefaultPlatform();
}

} // namespace FishingGame
