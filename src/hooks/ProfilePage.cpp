#include "../classes/DIBInfoPopup.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/ProfilePage.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

using namespace geode::prelude;

class $modify(DIBProfilePage, ProfilePage) {
    static void onModify(ModifyBase<ModifyDerive<DIBProfilePage, ProfilePage>>& self) {
        self.getHook("ProfilePage::onStatInfo").inspect([](Hook* hook) {
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("enable-demon-breakdown"));
            hook->setPriority(Priority::Replace);

            listenForSettingChangesV3<bool>("enable-demon-breakdown", [hook](bool value) {
                hook->toggle(value).inspectErr([](const std::string& err) {
                    log::error("Failed to toggle ProfilePage::onStatInfo hook: {}", err);
                });
            }, mod);
        }).inspectErr([](const std::string& err) {
            log::error("Failed to get ProfilePage::onStatInfo hook: {}", err);
        });
    }

    void onStatInfo(CCObject* sender) {
        if (sender->getTag() == 3 && (m_ownProfile || !user_data::downloading(m_score))) {
            if (m_ownProfile) {
                return DIBInfoPopup::create(DemonsInBetween::createBreakdown())->show();
            }
            else if (auto data = user_data::get<DemonBreakdown>(m_score)) {
                return DIBInfoPopup::create(std::move(data).unwrap())->show();
            }
        }

        ProfilePage::onStatInfo(sender);
    }
};
