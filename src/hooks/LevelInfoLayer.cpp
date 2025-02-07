#include "../DemonsInBetween.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class $modify(DIBLevelInfoLayer, LevelInfoLayer) {
    static void onModify(ModifyBase<ModifyDerive<DIBLevelInfoLayer, LevelInfoLayer>>& self) {
        (void)self.setHookPriorityAfterPost("LevelInfoLayer::init", "minemaker0430.gddp_integration");
    }

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (level->m_stars.value() < 10) return true;

        auto createDemon = true;

        if (getChildByID("grd-difficulty") || !Mod::get()->getSettingValue<bool>("enable-difficulties")) createDemon = false;

        auto gddpDifficulty = getChildByID("gddp-difficulty");
        if (gddpDifficulty && !Mod::get()->getSettingValue<bool>("gddp-integration-override")) createDemon = false;
        else if (gddpDifficulty) gddpDifficulty->setVisible(false);

        auto levelID = level->m_levelID.value();
        auto& demon = DemonsInBetween::demonForLevel(levelID);
        if (demon.id == 0 || demon.difficulty == 0) return true;

        auto demonInfoButton = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName(fmt::format("DIB_{:02d}_001.png"_spr, demon.difficulty).c_str()),
            this, menu_selector(DIBLevelInfoLayer::onDemonInfo)
        );
        demonInfoButton->setID("demon-info-button"_spr);
        auto leftSideMenu = getChildByID("left-side-menu");
        leftSideMenu->addChild(demonInfoButton);
        leftSideMenu->updateLayout();

        if (!createDemon) return true;

        addChild(DemonsInBetween::spriteForDifficulty(m_difficultySprite, demon.difficulty, GJDifficultyName::Long, DemonsInBetween::stateForLevel(m_level)), 3);
        m_difficultySprite->setOpacity(0);

        return true;
    }

    void onDemonInfo(CCObject* sender) {
        auto& demon = DemonsInBetween::demonForLevel(m_level->m_levelID.value());
        if (demon.id == 0 || demon.difficulty == 0) return;

        auto difficulty = "Unknown Demon";
        switch (demon.difficulty) {
            case 1: difficulty = "Free Demon"; break;
            case 2: difficulty = "Peaceful Demon"; break;
            case 3: difficulty = "Simple Demon"; break;
            case 4: difficulty = "Easy Demon"; break;
            case 5: difficulty = "Casual Demon"; break;
            case 6: difficulty = "Mild Demon"; break;
            case 7: difficulty = "Medium Demon"; break;
            case 8: difficulty = "Normal Demon"; break;
            case 9: difficulty = "Moderate Demon"; break;
            case 10: difficulty = "Tricky Demon"; break;
            case 11: difficulty = "Hard Demon"; break;
            case 12: difficulty = "Harder Demon"; break;
            case 13: difficulty = "Tough Demon"; break;
            case 14: difficulty = "Wild Demon"; break;
            case 15: difficulty = "Insane Demon"; break;
            case 16: difficulty = "Cruel Demon"; break;
            case 17: difficulty = "Crazy Demon"; break;
            case 18: difficulty = "Bizarre Demon"; break;
            case 19: difficulty = "Brutal Demon"; break;
            case 20: difficulty = "Extreme Demon"; break;
        }

        auto originalDifficulty = "Unknown Demon";
        switch (m_level->m_demonDifficulty) {
            case 0: originalDifficulty = "Hard Demon"; break;
            case 3: originalDifficulty = "Easy Demon"; break;
            case 4: originalDifficulty = "Medium Demon"; break;
            case 5: originalDifficulty = "Insane Demon"; break;
            case 6: originalDifficulty = "Extreme Demon"; break;
        }

        FLAlertLayer::create("Demon Info", fmt::format(
            "<cy>{}</c>\n"
            "<cg>Tier</c>: {}\n"
            "<cl>Enjoyment</c>: {}\n"
            "<cp>Difficulty</c>: {}\n"
            "<co>Original Difficulty</c>: {}",
            std::string(m_level->m_levelName),
            demon.tier,
            demon.enjoyment >= 0.0 ? fmt::format("{}", demon.enjoyment) : "N/A",
            difficulty,
            originalDifficulty
        ), "OK")->show();
    }
};
