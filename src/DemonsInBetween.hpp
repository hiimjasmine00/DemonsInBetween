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

template <class T, size_t N>
struct matjson::Serialize<std::array<T, N>> {
    static geode::Result<std::array<T, N>> fromJson(const matjson::Value& value) {
        if (!value.isArray()) return geode::Err("Expected array");
        std::array<T, N> arr;
        for (size_t i = 0; i < N; i++) {
            if (i < value.size()) {
                GEODE_UNWRAP_INTO(arr[i], value[i].as<T>());
            }
            else arr[i] = 0;
        }
        return geode::Ok(arr);
    }

    static matjson::Value toJson(const std::array<T, N>& arr) {
        std::vector<matjson::Value> vec;
        vec.reserve(arr.size());
        for (auto& v : arr) {
            vec.push_back(v);
        }
        return vec;
    }
};

template <>
struct matjson::Serialize<DemonBreakdown> {
    static geode::Result<DemonBreakdown> fromJson(const matjson::Value& value) {
        if (!value.isObject()) return geode::Err("Expected object");
        DemonBreakdown breakdown;
        if (auto classic = value.get<std::array<int, 21>>("classic").ok()) breakdown.classic = *classic;
        if (auto platformer = value.get<std::array<int, 21>>("platformer").ok()) breakdown.platformer = *platformer;
        if (auto weekly = value.get<int>("weekly").ok()) breakdown.weekly = *weekly;
        if (auto event = value.get<int>("event").ok()) breakdown.event = *event;
        if (auto gauntlet = value.get<int>("gauntlet").ok()) breakdown.gauntlet = *gauntlet;
        return geode::Ok(breakdown);
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
