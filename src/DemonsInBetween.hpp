#include <cocos2d.h>
#include <Geode/Enums.hpp>
#include <Geode/GeneratedPredeclare.hpp>
#include <matjson/std.hpp>

struct LadderDemon {
    int id = 0;
    double tier = 0.0;
    double enjoyment = 0.0;
    int difficulty = 0;
};

struct DemonBreakdown {
    std::vector<int> classic;
    std::vector<int> platformer;
    int weekly = 0;
    int event = 0;
    int gauntlet = 0;
};

template <>
struct matjson::Serialize<DemonBreakdown> {
    static geode::Result<DemonBreakdown> fromJson(const matjson::Value& value) {
        if (!value.isObject()) return geode::Err("Expected object");
        DemonBreakdown breakdown;
        GEODE_UNWRAP_INTO_IF_OK(breakdown.classic, value.get("classic").andThen([](const matjson::Value& v) { return v.as<std::vector<int>>(); }));
        GEODE_UNWRAP_INTO_IF_OK(breakdown.platformer, value.get("platformer").andThen([](const matjson::Value& v) { return v.as<std::vector<int>>(); }));
        GEODE_UNWRAP_INTO_IF_OK(breakdown.weekly, value.get("weekly").andThen([](const matjson::Value& v) { return v.asInt(); }));
        GEODE_UNWRAP_INTO_IF_OK(breakdown.event, value.get("event").andThen([](const matjson::Value& v) { return v.asInt(); }));
        GEODE_UNWRAP_INTO_IF_OK(breakdown.gauntlet, value.get("gauntlet").andThen([](const matjson::Value& v) { return v.asInt(); }));
        return geode::Ok(std::move(breakdown));
    }

    static matjson::Value toJson(const DemonBreakdown& breakdown) {
        return matjson::makeObject({
            { "classic", breakdown.classic },
            { "platformer", breakdown.platformer },
            { "weekly", breakdown.weekly },
            { "event", breakdown.event },
            { "gauntlet", breakdown.gauntlet }
        });
    }
};

class DemonsInBetween {
public:
    inline static std::vector<LadderDemon> gddl = {};
    inline static std::map<int, std::vector<std::string>> gddlDifficulties = {
        { 1, {} }, { 2, {} }, { 3, {} }, { 4, {} }, { 5, {} }, { 6, {} }, { 7, {} }, { 8, {} }, { 9, {} }, { 10, {} },
        { 11, {} }, { 12, {} }, { 13, {} }, { 14, {} }, { 15, {} }, { 16, {} }, { 17, {} }, { 18, {} }, { 19, {} }, { 20, {} }
    };

    static LadderDemon* demonForLevel(int);
    static cocos2d::CCPoint offsetForDifficulty(int, GJDifficultyName);
    static cocos2d::CCSprite* spriteForDifficulty(const cocos2d::CCPoint&, int, GJDifficultyName, GJFeatureState);
    static GJFeatureState stateForLevel(GJGameLevel*);
    static cocos2d::CCScene* browseScene(int);
    static GJSearchObject* searchObjectForPage(int, int);
    static DemonBreakdown createBreakdown();
};
