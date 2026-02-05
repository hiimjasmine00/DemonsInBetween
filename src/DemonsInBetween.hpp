#include <cocos2d.h>
#include <Geode/Enums.hpp>
#include <Geode/GeneratedPredeclare.hpp>
#include <matjson.hpp>

struct LadderDemon {
    int id = 0;
    double tier = 0.0;
    double enjoyment = 0.0;
    int difficulty = 0;
};

struct DemonBreakdown {
    std::array<int, 21> classic = {};
    std::array<int, 21> platformer = {};
    int weekly = 0;
    int event = 0;
    int gauntlet = 0;
};

template <>
struct matjson::Serialize<DemonBreakdown> {
    static geode::Result<DemonBreakdown> fromJson(const matjson::Value&);
    static matjson::Value toJson(const DemonBreakdown&);
};

namespace DemonsInBetween {
    extern std::map<int, LadderDemon> gddl;
    extern std::map<int, std::vector<std::string>> gddlDifficulties;

    LadderDemon* demonForLevel(int);
    cocos2d::CCPoint offsetForDifficulty(int, GJDifficultyName);
    cocos2d::CCSprite* spriteForDifficulty(const cocos2d::CCPoint&, int, GJDifficultyName, GJFeatureState);
    GJFeatureState stateForLevel(GJGameLevel*);
    cocos2d::CCScene* browseScene(int);
    GJSearchObject* searchObjectForPage(int, int);
    DemonBreakdown createBreakdown();
}
