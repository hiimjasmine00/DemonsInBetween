#include "../DemonsInBetween.hpp"
#include <Geode/modify/GameLevelManager.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

using namespace geode::prelude;

class $modify(DIBGameLevelManager, GameLevelManager) {
    static void onModify(ModifyBase<ModifyDerive<DIBGameLevelManager, GameLevelManager>>& self) {
        self.getHook("GameLevelManager::onUpdateUserScoreCompleted").inspect([](Hook* hook) {
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("enable-demon-breakdown") && mod->getSettingValue<bool>("send-demon-breakdown"));

            listenForAllSettingChangesV3([hook](std::shared_ptr<SettingV3> setting) {
                auto mod = Mod::get();
                auto key = setting->getKey();
                auto enableDemonBreakdown = key == "enable-demon-breakdown"
                    ? std::static_pointer_cast<BoolSettingV3>(setting)->getValue()
                    : mod->getSettingValue<bool>("enable-demon-breakdown");
                auto sendDemonBreakdown = key == "send-demon-breakdown"
                    ? std::static_pointer_cast<BoolSettingV3>(setting)->getValue()
                    : mod->getSettingValue<bool>("send-demon-breakdown");

                hook->toggle(enableDemonBreakdown && sendDemonBreakdown).inspectErr([](const std::string& err) {
                    log::error("Failed to toggle GameLevelManager::onUpdateUserScoreCompleted hook: {}", err);
                });
            }, mod);
        }).inspectErr([](const std::string& err) {
            log::error("Failed to get GameLevelManager::onUpdateUserScoreCompleted hook: {}", err);
        });
    }

    void onUpdateUserScoreCompleted(gd::string response, gd::string tag) {
        auto sent = m_downloadObjects->objectForKey(tag) != nullptr;
        GameLevelManager::onUpdateUserScoreCompleted(response, tag);
        if (sent && response != "-1") user_data::upload(DemonsInBetween::createBreakdown());
    }
};
