#include "DemonsInBetween.hpp"
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

LadderDemon& DemonsInBetween::demonForLevel(int levelID) {
    static LadderDemon empty = { 0, 0.0, 0.0, 0 };
    auto demon = ranges::indexOf(GDDL, [levelID](const LadderDemon& d) { return d.id == levelID; });
    return !demon.has_value() ? empty : GDDL[demon.value()];
}

CCPoint DemonsInBetween::offsetForDifficulty(int difficulty, GJDifficultyName name) {
    switch (difficulty) {
        case 1: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 2: return name == GJDifficultyName::Long ? CCPoint { 0.125f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 3: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 4: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.125f } : CCPoint { -0.125f, -0.375f };
        case 5: return name == GJDifficultyName::Long ? CCPoint { 0.25f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 6: return name == GJDifficultyName::Long ? CCPoint { 0.125f, -4.75f } : CCPoint { -0.125f, -0.25f };
        case 7: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.0f } : CCPoint { -0.125f, -0.375f };
        case 8: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { -0.125f, 0.5f };
        case 9: return name == GJDifficultyName::Long ? CCPoint { -0.125f, -4.125f } : CCPoint { -0.125f, 0.5f };
        case 10: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.0f } : CCPoint { -0.125f, 0.25f };
        case 11: return name == GJDifficultyName::Long ? CCPoint { -0.125f, -4.125f } : CCPoint { -0.125f, 0.5f };
        case 12: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { 0.125f, 0.5f };
        case 13: return name == GJDifficultyName::Long ? CCPoint { 0.125f, -4.125f } : CCPoint { 0.125f, 0.5f };
        case 14: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { 0.125f, 0.5f };
        case 15: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { 0.0f, 0.5f };
        case 16: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.625f } : CCPoint { 0.0f, 1.25f };
        case 17: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.625f } : CCPoint { 0.0f, 1.25f };
        case 18: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.5f } : CCPoint { 0.0f, 1.125f };
        case 19: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.5f } : CCPoint { 0.0f, 1.125f };
        case 20: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.5f } : CCPoint { 0.0f, 1.125f };
        default: return CCPoint { 0.0f, 0.0f };
    }
}

CCSprite* DemonsInBetween::spriteForDifficulty(GJDifficultySprite* difficultySprite, int difficulty, GJDifficultyName name, GJFeatureState state) {
    auto sprite = CCSprite::createWithSpriteFrameName(fmt::format(
        "DIB_{:02d}{}_btn{}_001.png"_spr,
        difficulty,
        state == GJFeatureState::Legendary ? "_4" : state == GJFeatureState::Mythic ? "_5" : "",
        name == GJDifficultyName::Long ? "2" : ""
    ).c_str());
    sprite->setPosition(difficultySprite->getPosition() + offsetForDifficulty(difficulty, name));
    sprite->setID("between-difficulty-sprite"_spr);
    return sprite;
}

GJFeatureState DemonsInBetween::stateForLevel(GJGameLevel* level) {
    auto state = level->m_featured ? (GJFeatureState)(level->m_isEpic + 1) : GJFeatureState::None;
    if (state == GJFeatureState::Legendary && !Mod::get()->getSettingValue<bool>("enable-legendary")) state = GJFeatureState::None;
    else if (state == GJFeatureState::Mythic && !Mod::get()->getSettingValue<bool>("enable-mythic")) state = GJFeatureState::None;
    return state;
}

GJSearchObject* DemonsInBetween::searchObjectForPage(int page) {
    auto& levels = DemonsInBetween::GDDL_DIFFICULTIES[DemonsInBetween::DIFFICULTY];
    SEARCH_SIZE = levels.size();
    MAX_PAGE = (SEARCH_SIZE - 1) / 10;
    return GJSearchObject::create(SearchType::MapPackOnClick, string::join(
        std::vector<std::string>(levels.begin() + page * 10, levels.begin() + std::min(SEARCH_SIZE, (page + 1) * 10)), ","));
}
