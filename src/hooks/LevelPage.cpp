#include "../DemonsInBetween.hpp"
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelPage.hpp>

using namespace geode::prelude;

class $modify(DIBLevelPage, LevelPage) {
    static void onModify(ModifyBase<ModifyDerive<DIBLevelPage, LevelPage>>& self) {
        (void)self.setHookPriorityAfterPost("LevelPage::updateDynamicPage", "firee.overchargedlevels");
    }

    void updateDynamicPage(GJGameLevel* level) {
        LevelPage::updateDynamicPage(level);

        if (auto betweenDifficulty = m_levelDisplay->getChildByID("between-difficulty-sprite"_spr)) {
            betweenDifficulty->removeFromParent();
            m_difficultySprite->setVisible(true);
        }

        auto levelID = level->m_levelID.value();
        if (levelID < 1 || GameStatsManager::get()->getStat("8") < level->m_requiredCoins) return;

        auto& demon = DemonsInBetween::demonForLevel(levelID);
        if (demon.id == 0 || demon.difficulty == 0) return;

        auto overcharged = Loader::get()->getLoadedMod("firee.overchargedlevels");
        auto overchargedEnabled = overcharged && overcharged->getSettingValue<bool>("enabled");
        auto demonSprite = CCSprite::createWithSpriteFrameName(fmt::format("DIB_{:02d}{}_001.png"_spr, demon.difficulty, overchargedEnabled ? "_btn" : "").c_str());
        demonSprite->setPosition(m_difficultySprite->getPosition() +
            (overchargedEnabled ? DemonsInBetween::offsetForDifficulty(demon.difficulty, GJDifficultyName::Short) * 0.9f : CCPoint { 0.0f, 0.0f }));
        demonSprite->setScale(overchargedEnabled ? 0.9f : 1.1f);
        demonSprite->setID("between-difficulty-sprite"_spr);
        m_levelDisplay->addChild(demonSprite);
        m_difficultySprite->setVisible(false);
    }
};
