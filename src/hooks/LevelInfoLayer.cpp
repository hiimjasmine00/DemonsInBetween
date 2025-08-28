#include "../DemonsInBetween.hpp"
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class $modify(DIBLevelInfoLayer, LevelInfoLayer) {
    inline static constexpr std::array difficulties = {
        "Unknown Demon", "Free Demon", "Peaceful Demon", "Simple Demon", "Easy Demon", "Casual Demon", "Mild Demon",
        "Medium Demon", "Normal Demon", "Moderate Demon", "Tricky Demon", "Hard Demon", "Harder Demon", "Tough Demon",
        "Wild Demon", "Insane Demon", "Cruel Demon", "Crazy Demon", "Bizarre Demon", "Brutal Demon", "Extreme Demon"
    };
    inline static constexpr std::array originalDifficulties = {
        "Hard Demon", "Unknown Demon", "Unknown Demon", "Easy Demon", "Medium Demon", "Insane Demon", "Extreme Demon"
    };

    static void onModify(ModifyBase<ModifyDerive<DIBLevelInfoLayer, LevelInfoLayer>>& self) {
        (void)self.setHookPriorityAfterPost("LevelInfoLayer::init", "itzkiba.grandpa_demon");
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
        auto demon = DemonsInBetween::demonForLevel(levelID);
        if (!demon) return true;

        auto demonInfoButton = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName(fmt::format("DIB_{:02d}_001.png"_spr, demon->difficulty).c_str()),
            this, menu_selector(DIBLevelInfoLayer::onDemonInfo)
        );
        demonInfoButton->setID("demon-info-button"_spr);
        auto leftSideMenu = getChildByID("left-side-menu");
        leftSideMenu->addChild(demonInfoButton);
        leftSideMenu->updateLayout();

        if (createDemon) {
            addChild(DemonsInBetween::spriteForDifficulty(m_difficultySprite->getPosition(),
                demon->difficulty, GJDifficultyName::Long, DemonsInBetween::stateForLevel(m_level)), 3);
            m_difficultySprite->setOpacity(0);
        }

        return true;
    }

    void onDemonInfo(CCObject* sender) {
        auto demon = DemonsInBetween::demonForLevel(m_level->m_levelID.value());
        if (!demon) return;

        FLAlertLayer::create("Demon Info", fmt::format(
            "<cy>{}</c>\n"
            "<cg>Tier</c>: {:.2f}\n"
            "<cl>Enjoyment</c>: {}\n"
            "<cp>Difficulty</c>: {}\n"
            "<co>Original Difficulty</c>: {}",
            GEODE_ANDROID(std::string)(m_level->m_levelName),
            demon->tier,
            demon->enjoyment >= 0.0 ? fmt::format("{:.2f}", demon->enjoyment) : "N/A",
            demon->difficulty < difficulties.size() ? difficulties[demon->difficulty] : "Unknown Demon",
            m_level->m_demonDifficulty < originalDifficulties.size() ? originalDifficulties[m_level->m_demonDifficulty] : "Unknown Demon"
        ), "OK")->show();
    }
};
