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
    inline static std::vector<LadderDemon> GDDL = {};
    inline static std::map<int, std::vector<std::string>> GDDL_DIFFICULTIES = {
        { 0, {} }, { 1, {} }, { 2, {} }, { 3, {} }, { 4, {} }, { 5, {} }, { 6, {} }, { 7, {} }, { 8, {} }, { 9, {} }, { 10, {} },
        { 11, {} }, { 12, {} }, { 13, {} }, { 14, {} }, { 15, {} }, { 16, {} }, { 17, {} }, { 18, {} }, { 19, {} }, { 20, {} }
    };
    inline static int MAX_PAGE = 0;
    inline static int SEARCH_SIZE = 0;
    inline static int DIFFICULTY = 0;
    inline static bool SEARCHING = false;

    static LadderDemon& demonForLevel(int);
    static cocos2d::CCPoint offsetForDifficulty(int, GJDifficultyName);
    static cocos2d::CCSprite* spriteForDifficulty(GJDifficultySprite*, int, GJDifficultyName, GJFeatureState);
    static GJFeatureState stateForLevel(GJGameLevel*);
    static GJSearchObject* searchObjectForPage(int);
};
