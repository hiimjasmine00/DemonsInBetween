#include "../DemonsInBetween.hpp"
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelPage.hpp>

using namespace geode::prelude;

class $modify(DIBLevelPage, LevelPage) {
    struct Fields {
        CCSprite* m_demonSprite = nullptr;
    };

    static void onModify(ModifyBase<ModifyDerive<DIBLevelPage, LevelPage>>& self) {
        (void)self.setHookPriorityAfterPost("LevelPage::updateDynamicPage", "firee.overchargedlevels");
    }

    void updateDynamicPage(GJGameLevel* level) {
        LevelPage::updateDynamicPage(level);

        auto f = m_fields.self();
        if (f->m_demonSprite) {
            f->m_demonSprite->removeFromParent();
            f->m_demonSprite = nullptr;
            m_difficultySprite->setVisible(true);
        }

        auto levelID = level->m_levelID.value();
        if (levelID < 1 || GameStatsManager::get()->getStat("8") < level->m_requiredCoins) return;

        auto demon = DemonsInBetween::demonForLevel(levelID);
        if (!demon) return;

        auto overchargedMod = Loader::get()->getLoadedMod("firee.overchargedlevels");
        auto overcharged = overchargedMod && overchargedMod->getSettingValue<bool>("enabled");
        f->m_demonSprite = CCSprite::createWithSpriteFrameName(
            fmt::format("DIB_{:02d}{}_001.png"_spr, demon->difficulty, overcharged ? "_btn" : "").c_str());
        f->m_demonSprite->setPosition(m_difficultySprite->getPosition() +
            (overcharged ? DemonsInBetween::offsetForDifficulty(demon->difficulty, GJDifficultyName::Short) * 0.9f : CCPoint { 0.0f, 0.0f }));
        f->m_demonSprite->setScale(1.1f - overcharged * 0.2f);
        f->m_demonSprite->setID("between-difficulty-sprite"_spr);
        m_levelDisplay->addChild(f->m_demonSprite);
        m_difficultySprite->setVisible(false);
    }
};
