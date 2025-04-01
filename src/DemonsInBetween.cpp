#include "DemonsInBetween.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

LadderDemon& DemonsInBetween::demonForLevel(int levelID) {
    static LadderDemon empty = { 0, 0.0, 0.0, 0 };
    auto demon = std::ranges::find_if(gddl, [levelID](const LadderDemon& d) { return d.id == levelID; });
    return demon != gddl.end() ? *demon : empty;
}

const CCPoint& DemonsInBetween::offsetForDifficulty(int difficulty, GJDifficultyName name) {
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
    if (state == GJFeatureState::Legendary && !Mod::get()->getSettingValue<bool>("enable-legendary")) state = GJFeatureState::None;
    else if (state == GJFeatureState::Mythic && !Mod::get()->getSettingValue<bool>("enable-mythic")) state = GJFeatureState::None;
    return state;
}

GJSearchObject* DemonsInBetween::searchObjectForPage(int difficulty, int page) {
    if (difficulty <= 0) return nullptr;

    auto& levels = DemonsInBetween::gddlDifficulties[difficulty];
    return GJSearchObject::create(SearchType::MapPackOnClick, string::join(
        std::vector<std::string>(levels.begin() + page * 10, levels.begin() + std::min((int)levels.size(), (page + 1) * 10)), ","));
}
