#include "../classes/DIBInfoPopup.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/ProfilePage.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

using namespace geode::prelude;

class $modify(DIBProfilePage, ProfilePage) {
    static void onModify(ModifyBase<ModifyDerive<DIBProfilePage, ProfilePage>>& self) {
        if (auto it = self.m_hooks.find("ProfilePage::onStatInfo"); it != self.m_hooks.end()) {
            auto hook = it->second.get();
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("enable-demon-breakdown"));
            hook->setPriority(Priority::Replace);

            listenForSettingChangesV3<bool>("enable-demon-breakdown", [hook](bool value) {
                if (auto err = hook->toggle(value).err()) {
                    log::error("Failed to toggle ProfilePage::onStatInfo hook: {}", *err);
                }
            }, mod);
        }
    }

    void onStatInfo(CCObject* sender) {
        if (sender->getTag() == 3 && (m_ownProfile || !user_data::downloading(m_score))) {
            if (m_ownProfile) {
                return DIBInfoPopup::create(DemonsInBetween::createBreakdown())->show();
            }
            else if (auto data = user_data::get<DemonBreakdown>(m_score)) {
                return DIBInfoPopup::create(data.unwrap())->show();
            }
        }

        ProfilePage::onStatInfo(sender);
    }
};
