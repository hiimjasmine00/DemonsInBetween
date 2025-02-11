#include "../classes/DIBInfoPopup.hpp"
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(DIBProfilePage, ProfilePage) {
    static void onModify(ModifyBase<ModifyDerive<DIBProfilePage, ProfilePage>>& self) {
        auto hookRes = self.getHook("ProfilePage::onStatInfo");
        if (hookRes.isErr()) return log::error("Failed to get ProfilePage::onStatInfo hook: {}", hookRes.unwrapErr());

        auto hook = hookRes.unwrap();
        hook->setAutoEnable(true);

        listenForSettingChanges("enable-demon-breakdown", [hook](bool value) {
            auto changeRes = value ? hook->enable() : hook->disable();
            if (changeRes.isErr()) log::error("Failed to {} ProfilePage::onStatInfo hook: {}", value ? "enable" : "disable", changeRes.unwrapErr());
        });
    }

    void onStatInfo(CCObject* sender) {
        if (m_ownProfile && sender->getTag() == 3) DIBInfoPopup::create()->show();
        else ProfilePage::onStatInfo(sender);
    }
};
