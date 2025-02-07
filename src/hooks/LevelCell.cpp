#include "../DemonsInBetween.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

class $modify(DIBLevelCell, LevelCell) {
    static void onModify(ModifyBase<ModifyDerive<DIBLevelCell, LevelCell>>& self) {
        auto hookRes = self.getHook("LevelCell::loadFromLevel");
        if (hookRes.isErr()) return log::error("Failed to get LevelCell::loadFromLevel hook: {}", hookRes.unwrapErr());

        auto hook = hookRes.unwrap();
        hook->setAutoEnable(Mod::get()->getSettingValue<bool>("enable-difficulties"));

        listenForSettingChanges("enable-difficulties", [hook](bool value) {
            auto changeRes = value ? hook->enable() : hook->disable();
            if (changeRes.isErr()) log::error("Failed to {} LevelCell::loadFromLevel hook: {}", value ? "enable" : "disable", changeRes.unwrapErr());
        });
    }

    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        if (level->m_stars.value() < 10) return;

        auto difficultyContainer = m_mainLayer->getChildByID("difficulty-container");
        if (!difficultyContainer) difficultyContainer = m_mainLayer->getChildByID("grd-demon-icon-layer");
        if (!difficultyContainer) return;

        auto difficultySprite = static_cast<GJDifficultySprite*>(difficultyContainer->getChildByID("difficulty-sprite"));
        if (!difficultySprite || !difficultySprite->isVisible()) return; // If invisible, we're just going to assume it's Grandpa Demon

        auto gddpDifficulty = difficultyContainer->getChildByID("gddp-difficulty");
        if (gddpDifficulty && !Mod::get()->getSettingValue<bool>("gddp-integration-override")) return;
        else if (gddpDifficulty) gddpDifficulty->setVisible(false);

        auto levelID = level->m_levelID.value();
        auto& demon = DemonsInBetween::demonForLevel(levelID);
        if (demon.id == 0 && demon.difficulty == 0) return;

        difficultyContainer->addChild(DemonsInBetween::spriteForDifficulty(difficultySprite,
            demon.difficulty, GJDifficultyName::Short, DemonsInBetween::stateForLevel(m_level)), 3);
        difficultySprite->setOpacity(0);
    }
};
