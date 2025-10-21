#include "../DemonsInBetween.hpp"
#include <Geode/modify/GameLevelManager.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

using namespace geode::prelude;

class $modify(DIBGameLevelManager, GameLevelManager) {
    static void onModify(ModifyBase<ModifyDerive<DIBGameLevelManager, GameLevelManager>>& self) {
        if (auto it = self.m_hooks.find("GameLevelManager::updateUserScore"); it != self.m_hooks.end()) {
            auto hook = it->second.get();
            auto mod = Mod::get();
            hook->setAutoEnable(mod->getSettingValue<bool>("enable-demon-breakdown") && mod->getSettingValue<bool>("send-demon-breakdown"));

            listenForAllSettingChangesV3([hook](std::shared_ptr<SettingV3> setting) {
                auto key = setting->getKey();
                auto isEnable = key == "enable-demon-breakdown";
                auto isSend = key == "send-demon-breakdown";
                if (!isEnable && !isSend) return;

                auto mod = Mod::get();
                auto enableDemonBreakdown = isEnable
                    ? std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue()
                    : mod->getSettingValue<bool>("enable-demon-breakdown");
                auto sendDemonBreakdown = isSend
                    ? std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue()
                    : mod->getSettingValue<bool>("send-demon-breakdown");

                if (auto err = hook->toggle(enableDemonBreakdown && sendDemonBreakdown).err()) {
                    log::error("Failed to toggle GameLevelManager::updateUserScore hook: {}", *err);
                }
            }, mod);
        }
    }

    void updateUserScore() {
        GameLevelManager::updateUserScore();
        if (isDLActive("user_score")) user_data::upload(DemonsInBetween::createBreakdown());
    }
};
