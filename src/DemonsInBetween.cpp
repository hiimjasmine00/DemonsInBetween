#include "DemonsInBetween.hpp"
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/web.hpp>
#include <ranges>

using namespace geode::prelude;

std::map<int, LadderDemon> DemonsInBetween::gddl;
std::map<int, std::vector<std::string>> DemonsInBetween::gddlDifficulties = {
    { 1, {} }, { 2, {} }, { 3, {} }, { 4, {} }, { 5, {} }, { 6, {} }, { 7, {} }, { 8, {} }, { 9, {} }, { 10, {} },
    { 11, {} }, { 12, {} }, { 13, {} }, { 14, {} }, { 15, {} }, { 16, {} }, { 17, {} }, { 18, {} }, { 19, {} }, { 20, {} }
};

constexpr const char* url = "https://docs.google.com/spreadsheets/d/1qKlWKpDkOpU1ZF6V6xGfutDY2NvcA8MNPnsv6GBkKPQ/gviz/tq?tqx=out:csv&sheet=GDDL";

$execute {
    new EventListener(+[](GameEvent*) {
        web::WebRequest().get(url).listen([](web::WebResponse* res) {
            if (!res->ok()) return;

            constexpr std::array difficulties = {
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 12, 13, 14, 14, 15, 15,
                16, 17, 18, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20
            };
            constexpr std::array mainLevels = { 0, 14, 18, 20 };

            auto lines = string::split(std::string(std::from_range, res->data()), "\n");
            auto& header = lines[0];
            auto keys = string::split(header.substr(1, header.size() - 2), "\",\"");
            for (auto& line : lines | std::views::drop(1)) {
                auto values = string::split(line.substr(1, line.size() - 2), "\",\"");
                LadderDemon demon;
                for (size_t j = 0; j < keys.size() && j < values.size(); j++) {
                    auto& key = keys[j];
                    auto& value = values[j];
                    if (key == "ID") {
                        std::from_chars(value.data(), value.data() + value.size(), demon.id);
                        if (demon.id < mainLevels.size()) demon.id = mainLevels[demon.id];
                        if (demon.id < 1) break;
                    }
                    else if (key == "Tier") {
                        #ifdef __cpp_lib_to_chars
                        std::from_chars(value.data(), value.data() + value.size(), demon.tier);
                        #else
                        if (auto num = numFromString<double>(value).ok()) demon.tier = *num;
                        #endif
                        int roundedTier = round(demon.tier);
                        demon.difficulty = roundedTier < difficulties.size() ? difficulties[roundedTier] : 0;
                        if (demon.difficulty > 0) DemonsInBetween::gddlDifficulties[demon.difficulty].push_back(fmt::to_string(demon.id));
                        else break;
                    }
                    else if (key == "Enjoyment") {
                        #ifdef __cpp_lib_to_chars
                        std::from_chars(value.data(), value.data() + value.size(), demon.enjoyment);
                        #else
                        if (auto num = numFromString<double>(value).ok()) demon.enjoyment = *num;
                        #endif
                    }
                }
                if (demon.id > 0 && demon.difficulty > 0) DemonsInBetween::gddl.emplace(demon.id, demon);
            }
        });
    }, GameEventFilter(GameEventType::Loaded));
}

LadderDemon* DemonsInBetween::demonForLevel(int levelID) {
    auto demon = gddl.find(levelID);
    return demon != gddl.end() ? &demon->second : nullptr;
}

CCPoint DemonsInBetween::offsetForDifficulty(int difficulty, GJDifficultyName name) {
    constexpr std::array<CCPoint, 21> longOffsets = {
        CCPoint { 0.0f, 0.0f },
        { 0.0f, -5.0f }, { 0.125f, -5.0f }, { 0.0f, -5.0f }, { 0.0f, -5.125f }, { 0.25f, -5.0f },
        { 0.125f, -4.75f }, { 0.0f, -5.0f }, { 0.0f, -4.125f }, { -0.125f, -4.125f }, { 0.0f, -4.0f },
        { -0.125f, -4.125f }, { 0.0f, -4.125f }, { 0.125f, -4.125f }, { 0.0f, -4.125f }, { 0.0f, -4.125f },
        { 0.0f, -3.625f }, { 0.0f, -3.625f }, { 0.0f, -3.5f }, { 0.0f, -3.5f }, { 0.0f, -3.5f }
    };
    constexpr std::array<CCPoint, 21> shortOffsets = {
        CCPoint { 0.0f, 0.0f },
        { -0.125f, -0.25f }, { -0.125f, -0.25f }, { -0.125f, -0.25f }, { -0.125f, -0.375f }, { -0.125f, -0.25f },
        { -0.125f, -0.25f }, { -0.125f, -0.375f }, { -0.125f, 0.5f }, { -0.125f, 0.5f }, { -0.125f, 0.25f },
        { -0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.0f, 0.5f },
        { 0.0f, 1.25f }, { 0.0f, 1.25f }, { 0.0f, 1.125f }, { 0.0f, 1.125f }, { 0.0f, 1.125f }
    };

    auto& offsets = name == GJDifficultyName::Long ? longOffsets : shortOffsets;
    return difficulty < offsets.size() ? offsets[difficulty] : offsets[0];
}

CCSprite* DemonsInBetween::spriteForDifficulty(const CCPoint& position, int difficulty, GJDifficultyName name, GJFeatureState state) {
    auto sprite = CCSprite::createWithSpriteFrameName(fmt::format(
        "DIB_{:02d}{}_btn{}_001.png"_spr,
        difficulty,
        state == GJFeatureState::Legendary ? "_4" : state == GJFeatureState::Mythic ? "_5" : "",
        name == GJDifficultyName::Long ? "2" : ""
    ).c_str());
    sprite->setPosition(position + offsetForDifficulty(difficulty, name));
    sprite->setID("between-difficulty-sprite"_spr);
    return sprite;
}

GJFeatureState DemonsInBetween::stateForLevel(GJGameLevel* level) {
    auto state = level->m_featured ? (GJFeatureState)(level->m_isEpic + 1) : GJFeatureState::None;
    auto mod = Mod::get();
    if (state == GJFeatureState::Legendary && !mod->getSettingValue<bool>("enable-legendary")) state = GJFeatureState::None;
    else if (state == GJFeatureState::Mythic && !mod->getSettingValue<bool>("enable-mythic")) state = GJFeatureState::None;
    return state;
}

GJSearchObject* DemonsInBetween::searchObjectForPage(int difficulty, int page) {
    if (difficulty <= 0) return nullptr;

    auto& levels = gddlDifficulties[difficulty];
    auto searchObject = GJSearchObject::create(SearchType::MapPackOnClick);
    #ifdef GEODE_IS_ANDROID
    std::string searchQuery = searchObject->m_searchQuery;
    #else
    auto& searchQuery = searchObject->m_searchQuery;
    #endif
    auto end = std::min(levels.end(), levels.begin() + (page + 1) * 10);
    for (auto it = levels.begin() + page * 10; it != end; ++it) {
        if (!searchQuery.empty()) searchQuery += ',';
        searchQuery += *it;
    }
    GEODE_ANDROID(searchObject->m_searchQuery = searchQuery;)
    return searchObject;
}

DemonBreakdown DemonsInBetween::createBreakdown() {
    DemonBreakdown breakdown;

    auto glm = GameLevelManager::get();
    auto gsm = GameStatsManager::get();
    for (auto [_, level] : CCDictionaryExt<std::string, GJGameLevel*>(glm->m_onlineLevels)) {
        if (level->m_stars.value() < 10 || level->m_normalPercent.value() < 100 || !gsm->hasCompletedLevel(level)) continue;

        auto& completionCount = level->m_levelLength == 5 ? breakdown.platformer : breakdown.classic;
        completionCount[0]++;

        auto demon = DemonsInBetween::demonForLevel(level->m_levelID.value());
        if (demon) completionCount[demon->difficulty]++;
        else {
            switch (level->m_demonDifficulty) {
                case 3: completionCount[4]++; break;
                case 4: completionCount[7]++; break;
                case 0: completionCount[11]++; break;
                case 5: completionCount[15]++; break;
                case 6: completionCount[20]++; break;
            }
        }
    }

    for (auto [_, level] : CCDictionaryExt<std::string, GJGameLevel*>(glm->m_dailyLevels)) {
        if (level->m_stars.value() < 10 || level->m_normalPercent.value() < 100 || !gsm->hasCompletedLevel(level)) continue;
        auto dailyID = level->m_dailyID.value();
        if (dailyID > 200000) breakdown.event++;
        else if (dailyID > 100000) breakdown.weekly++;
    }

    for (auto [_, level] : CCDictionaryExt<std::string, GJGameLevel*>(glm->m_gauntletLevels)) {
        if (level->m_stars.value() < 10 || level->m_normalPercent.value() < 100 || !gsm->hasCompletedLevel(level)) continue;
        breakdown.gauntlet++;
    }

    return breakdown;
}

template <class T, size_t N>
struct matjson::Serialize<std::array<T, N>> {
    static Result<std::array<T, N>> fromJson(const matjson::Value& value) {
        if (!value.isArray()) return Err("not an array");
        std::array<T, N> arr{};
        for (size_t i = 0; i < N && i < value.size(); i++) {
            GEODE_UNWRAP_INTO(arr[i], value[i].as<T>());
        }
        return Ok(arr);
    }

    static matjson::Value toJson(const std::array<T, N>& arr) {
        std::vector<matjson::Value> vec;
        vec.reserve(N);
        for (auto& v : arr) {
            vec.push_back(v);
        }
        return vec;
    }
};

Result<DemonBreakdown> matjson::Serialize<DemonBreakdown>::fromJson(const matjson::Value& value) {
    if (!value.isObject()) return Err("Expected object");
    DemonBreakdown breakdown;
    if (auto classic = value.get<std::array<int, 21>>("classic").ok()) breakdown.classic = *classic;
    if (auto platformer = value.get<std::array<int, 21>>("platformer").ok()) breakdown.platformer = *platformer;
    if (auto weekly = value.get<int>("weekly").ok()) breakdown.weekly = *weekly;
    if (auto event = value.get<int>("event").ok()) breakdown.event = *event;
    if (auto gauntlet = value.get<int>("gauntlet").ok()) breakdown.gauntlet = *gauntlet;
    return Ok(breakdown);
}

matjson::Value matjson::Serialize<DemonBreakdown>::toJson(const DemonBreakdown& breakdown) {
    return matjson::makeObject({
        { "classic", breakdown.classic },
        { "platformer", breakdown.platformer },
        { "weekly", breakdown.weekly },
        { "event", breakdown.event },
        { "gauntlet", breakdown.gauntlet }
    });
}
