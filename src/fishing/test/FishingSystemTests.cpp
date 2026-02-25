#include "fishing/test/TestFramework.h"
#include "fishing/systems/FishingSystem.h"
#include "fishing/core/DataStructures.h"

using namespace FishingGame;

TEST_SUITE(FishingSystem) {

TEST(FishingSystem, CreateFishingSystem) {
    FishingSystem system;
    ASSERT_TRUE(true);
}

TEST(FishingSystem, InitializeFishingSystem) {
    FishingSystem system;
    bool result = system.init();
    ASSERT_TRUE(result);
    
    system.cleanup();
}

TEST(FishingSystem, GetInitialState) {
    FishingSystem system;
    system.init();
    
    FishingState state = system.getState();
    ASSERT_EQ(FishingState::IDLE, state);
    
    system.cleanup();
}

TEST(FishingSystem, CastLine) {
    FishingSystem system;
    system.init();
    
    bool result = system.castLine(100.0f, 0.0f);
    ASSERT_TRUE(result);
    
    FishingState state = system.getState();
    ASSERT_TRUE(state == FishingState::CASTING || state == FishingState::WAITING);
    
    system.cleanup();
}

TEST(FishingSystem, ReelIn) {
    FishingSystem system;
    system.init();
    
    system.castLine(100.0f, 0.0f);
    
    bool result = system.reelIn();
    
    FishingState state = system.getState();
    ASSERT_TRUE(state == FishingState::REELING || 
                state == FishingState::CAUGHT || 
                state == FishingState::FAILED ||
                state == FishingState::IDLE);
    
    system.cleanup();
}

TEST(FishingSystem, Update) {
    FishingSystem system;
    system.init();
    
    system.castLine(100.0f, 0.0f);
    
    system.update(0.016f);
    
    FishingState state = system.getState();
    ASSERT_TRUE(state != FishingState::IDLE || state == FishingState::WAITING);
    
    system.cleanup();
}

TEST(FishingSystem, GetReelingProgress) {
    FishingSystem system;
    system.init();
    
    float progress = system.getReelingProgress();
    ASSERT_TRUE(progress >= 0.0f && progress <= 1.0f);
    
    system.cleanup();
}

TEST(FishingSystem, GetLineTension) {
    FishingSystem system;
    system.init();
    
    float tension = system.getLineTension();
    ASSERT_TRUE(tension >= 0.0f && tension <= 1.0f);
    
    system.cleanup();
}

TEST(FishingSystem, HasCaughtFish) {
    FishingSystem system;
    system.init();
    
    bool hasFish = system.hasCaughtFish();
    ASSERT_FALSE(hasFish);
    
    system.cleanup();
}

TEST(FishingSystem, SetFishingSpot) {
    FishingSystem system;
    system.init();
    
    FishingSpot spot;
    spot.id = 1;
    spot.name = "Test Spot";
    spot.position = Vector2f{100.0f, 200.0f};
    
    system.setFishingSpot(spot);
    
    system.cleanup();
}

TEST(FishingSystem, SetEquippedRod) {
    FishingSystem system;
    system.init();
    
    Item rod;
    rod.id = ItemID::ROD_BASIC;
    rod.name = "Basic Rod";
    rod.type = ItemType::ROD;
    
    system.setEquippedRod(rod);
    
    system.cleanup();
}

TEST(FishingSystem, SetEquippedLine) {
    FishingSystem system;
    system.init();
    
    Item line;
    line.id = ItemID::LINE_BASIC;
    line.name = "Basic Line";
    line.type = ItemType::LINE;
    
    system.setEquippedLine(line);
    
    system.cleanup();
}

TEST(FishingSystem, SetEquippedHook) {
    FishingSystem system;
    system.init();
    
    Item hook;
    hook.id = ItemID::HOOK_BASIC;
    hook.name = "Basic Hook";
    hook.type = ItemType::HOOK;
    
    system.setEquippedHook(hook);
    
    system.cleanup();
}

TEST(FishingSystem, SetEquippedBait) {
    FishingSystem system;
    system.init();
    
    Item bait;
    bait.id = ItemID::BAIT_WORM;
    bait.name = "Worm";
    bait.type = ItemType::BAIT;
    
    system.setEquippedBait(bait);
    
    system.cleanup();
}

TEST(FishingSystem, StateTransitions) {
    FishingSystem system;
    system.init();
    
    ASSERT_EQ(FishingState::IDLE, system.getState());
    
    system.castLine(100.0f, 0.0f);
    FishingState afterCast = system.getState();
    ASSERT_TRUE(afterCast == FishingState::CASTING || afterCast == FishingState::WAITING);
    
    system.cleanup();
}

}
