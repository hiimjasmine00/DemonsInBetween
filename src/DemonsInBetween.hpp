#include <cocos2d.h>
#include <Geode/Enums.hpp>
#include <Geode/GeneratedPredeclare.hpp>
#include <Geode/utils/web.hpp>

struct LadderDemon {
    int id;
    double tier;
    double enjoyment;
    int difficulty;
};

class DemonsInBetween {
public:
    inline static std::vector<LadderDemon> gddl = {};
    inline static std::map<int, std::vector<std::string>> gddlDifficulties = {
        { 1, {} }, { 2, {} }, { 3, {} }, { 4, {} }, { 5, {} }, { 6, {} }, { 7, {} }, { 8, {} }, { 9, {} }, { 10, {} },
        { 11, {} }, { 12, {} }, { 13, {} }, { 14, {} }, { 15, {} }, { 16, {} }, { 17, {} }, { 18, {} }, { 19, {} }, { 20, {} }
    };
    inline static constexpr std::array<cocos2d::CCPoint, 21> longOffsets = {
        cocos2d::CCPoint { 0.0f, 0.0f },
        { 0.0f, -5.0f }, { 0.125f, -5.0f }, { 0.0f, -5.0f }, { 0.0f, -5.125f }, { 0.25f, -5.0f },
        { 0.125f, -4.75f }, { 0.0f, -5.0f }, { 0.0f, -4.125f }, { -0.125f, -4.125f }, { 0.0f, -4.0f },
        { -0.125f, -4.125f }, { 0.0f, -4.125f }, { 0.125f, -4.125f }, { 0.0f, -4.125f }, { 0.0f, -4.125f },
        { 0.0f, -3.625f }, { 0.0f, -3.625f }, { 0.0f, -3.5f }, { 0.0f, -3.5f }, { 0.0f, -3.5f }
    };
    inline static constexpr std::array<cocos2d::CCPoint, 21> shortOffsets = {
        cocos2d::CCPoint { 0.0f, 0.0f },
        { -0.125f, -0.25f }, { -0.125f, -0.25f }, { -0.125f, -0.25f }, { -0.125f, -0.375f }, { -0.125f, -0.25f },
        { -0.125f, -0.25f }, { -0.125f, -0.375f }, { -0.125f, 0.5f }, { -0.125f, 0.5f }, { -0.125f, 0.25f },
        { -0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.0f, 0.5f },
        { 0.0f, 1.25f }, { 0.0f, 1.25f }, { 0.0f, 1.125f }, { 0.0f, 1.125f }, { 0.0f, 1.125f }
    };

    static LadderDemon& demonForLevel(int);
    static const cocos2d::CCPoint& offsetForDifficulty(int, GJDifficultyName);
    static cocos2d::CCSprite* spriteForDifficulty(const cocos2d::CCPoint&, int, GJDifficultyName, GJFeatureState);
    static GJFeatureState stateForLevel(GJGameLevel*);
    static cocos2d::CCScene* browseScene(int);
    static GJSearchObject* searchObjectForPage(int, int);
};
