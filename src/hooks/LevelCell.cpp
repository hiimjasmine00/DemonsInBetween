#include "../DemonsInBetween.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

class $modify(DIBLevelCell, LevelCell) {
    static void onModify(ModifyBase<ModifyDerive<DIBLevelCell, LevelCell>>& self) {
        (void)self.setHookPriorityAfterPost("LevelCell::loadFromLevel", "itzkiba.grandpa_demon");
        (void)self.setHookPriorityAfterPost("LevelCell::loadFromLevel", "minemaker0430.gddp_integration");

        self.getHook("LevelCell::loadFromLevel").inspect([](Hook* hook) {
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("enable-difficulties"));

            listenForSettingChangesV3<bool>("enable-difficulties", [hook](bool value) {
                hook->toggle(value).inspectErr([](const std::string& err) {
                    log::error("Failed to toggle LevelCell::loadFromLevel hook: {}", err);
                });
            }, mod);
        }).inspectErr([](const std::string& err) {
            log::error("Failed to get LevelCell::loadFromLevel hook: {}", err);
        });
    }

    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        if (level->m_stars.value() < 10) return;

        auto difficultyContainer = m_mainLayer->getChildByID("difficulty-container");
        if (!difficultyContainer) difficultyContainer = m_mainLayer->getChildByID("grd-demon-icon-layer");
        if (!difficultyContainer) return;

        auto difficultySprite = static_cast<CCNodeRGBA*>(difficultyContainer->getChildByID("difficulty-sprite"));
        if (!difficultySprite || !difficultySprite->isVisible()) return; // If invisible, we're just going to assume it's Grandpa Demon

        auto gddpDifficulty = difficultyContainer->getChildByID("gddp-difficulty");
        if (gddpDifficulty && !Mod::get()->getSettingValue<bool>("gddp-integration-override")) return;
        else if (gddpDifficulty) gddpDifficulty->setVisible(false);

        if (auto demon = DemonsInBetween::demonForLevel(level->m_levelID.value())) {
            difficultyContainer->addChild(DemonsInBetween::spriteForDifficulty(difficultySprite->getPosition(),
                demon->difficulty, GJDifficultyName::Short, DemonsInBetween::stateForLevel(m_level)), 3);
            difficultySprite->setOpacity(0);
        }
    }
};
