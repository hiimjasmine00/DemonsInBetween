#include "../DemonsInBetween.hpp"
#include <Geode/modify/GameLevelManager.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

using namespace geode::prelude;

class $modify(DIBGameLevelManager, GameLevelManager) {
    static void onModify(ModifyBase<ModifyDerive<DIBGameLevelManager, GameLevelManager>>& self) {
        self.getHook("GameLevelManager::onUpdateUserScoreFinished").inspect([](Hook* hook) {
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("send-demon-breakdown"));

            listenForSettingChangesV3<bool>("send-demon-breakdown", [hook](bool value) {
                hook->toggle(value).inspectErr([](const std::string& err) {
                    log::error("Failed to toggle GameLevelManager::onUpdateUserScoreFinished hook: {}", err);
                });
            }, mod);
        }).inspectErr([](const std::string& err) {
            log::error("Failed to get GameLevelManager::onUpdateUserScoreFinished hook: {}", err);
        });
    }

    void onUpdateUserScoreCompleted(gd::string response, gd::string tag) {
        auto sent = m_downloadObjects->objectForKey(tag) != nullptr;
        GameLevelManager::onUpdateUserScoreCompleted(response, tag);
        if (sent && response != "-1") user_data::upload(DemonsInBetween::createBreakdown());
    }
};
