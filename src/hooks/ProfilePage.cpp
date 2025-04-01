#include "../classes/DIBInfoPopup.hpp"
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(DIBProfilePage, ProfilePage) {
    static void onModify(ModifyBase<ModifyDerive<DIBProfilePage, ProfilePage>>& self) {
        (void)self.getHook("ProfilePage::onStatInfo").map([](Hook* hook) {
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("enable-demon-breakdown"));

            listenForSettingChanges("enable-demon-breakdown", [hook](bool value) {
                (void)(value ? hook->enable().mapErr([](const std::string& err) {
                    return log::error("Failed to enable ProfilePage::onStatInfo hook: {}", err), err;
                }) : hook->disable().mapErr([](const std::string& err) {
                    return log::error("Failed to disable ProfilePage::onStatInfo hook: {}", err), err;
                }));
            }, mod);

            return hook;
        }).mapErr([](const std::string& err) {
            return log::error("Failed to get ProfilePage::onStatInfo hook: {}", err), err;
        });
    }

    void onStatInfo(CCObject* sender) {
        if (m_ownProfile && sender->getTag() == 3) DIBInfoPopup::create()->show();
        else ProfilePage::onStatInfo(sender);
    }
};
