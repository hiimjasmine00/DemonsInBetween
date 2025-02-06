#include "../classes/DIBInfoPopup.hpp"
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(DIBProfilePage, ProfilePage) {
    void onStatInfo(CCObject* sender) {
        if (m_ownProfile && sender->getTag() == 3) DIBInfoPopup::create()->show();
        else ProfilePage::onStatInfo(sender);
    }
};
