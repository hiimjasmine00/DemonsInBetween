#include "../classes/DIBInfoPopup.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/ProfilePage.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>
#include <jasmine/hook.hpp>

using namespace geode::prelude;

class $modify(DIBProfilePage, ProfilePage) {
    static void onModify(ModifyBase<ModifyDerive<DIBProfilePage, ProfilePage>>& self) {
        jasmine::hook::modify(self.m_hooks, "ProfilePage::onStatInfo", "enable-demon-breakdown");
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
