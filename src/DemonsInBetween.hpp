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

class DemonsInBetween {
public:
    static std::map<int, LadderDemon> gddl;
    static std::map<int, std::vector<std::string>> gddlDifficulties;

    static LadderDemon* demonForLevel(int);
    static cocos2d::CCPoint offsetForDifficulty(int, GJDifficultyName);
    static cocos2d::CCSprite* spriteForDifficulty(const cocos2d::CCPoint&, int, GJDifficultyName, GJFeatureState);
    static GJFeatureState stateForLevel(GJGameLevel*);
    static cocos2d::CCScene* browseScene(int);
    static GJSearchObject* searchObjectForPage(int, int);
    static DemonBreakdown createBreakdown();
};
