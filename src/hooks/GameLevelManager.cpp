#include "../DemonsInBetween.hpp"
#include <Geode/modify/GameLevelManager.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>
#include <jasmine/hook.hpp>
#include <jasmine/setting.hpp>

using namespace geode::prelude;

class $modify(DIBGameLevelManager, GameLevelManager) {
    static void onModify(ModifyBase<ModifyDerive<DIBGameLevelManager, GameLevelManager>>& self) {
        auto hook = jasmine::hook::get(self.m_hooks, "GameLevelManager::updateUserScore",
            jasmine::setting::getValue<bool>("enable-demon-breakdown") && jasmine::setting::getValue<bool>("send-demon-breakdown"));
        if (hook) {
            SettingChangedEventV3().listen([hook](std::shared_ptr<SettingV3> setting) {
                if (setting->getModID() != GEODE_MOD_ID) return;

                auto key = setting->getKey();
                auto isEnable = key == "enable-demon-breakdown";
                auto isSend = key == "send-demon-breakdown";
                if (!isEnable && !isSend) return;

                auto enableDemonBreakdown = isEnable
                    ? std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue()
                    : jasmine::setting::getValue<bool>("enable-demon-breakdown");
                auto sendDemonBreakdown = isSend
                    ? std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue()
                    : jasmine::setting::getValue<bool>("send-demon-breakdown");

                jasmine::hook::toggle(hook, enableDemonBreakdown && sendDemonBreakdown);
            }).leak();
        }
    }

    void updateUserScore() {
        GameLevelManager::updateUserScore();
        if (isDLActive("user_score")) user_data::upload(DemonsInBetween::createBreakdown());
    }
};
