#include "../DemonsInBetween.hpp"
#include <Geode/modify/GameLevelManager.hpp>
#define USER_DATA_API_EVENTS
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>
#include <jasmine/hook.hpp>
#include <jasmine/setting.hpp>

using namespace geode::prelude;

class $modify(DIBGameLevelManager, GameLevelManager) {
    static void onModify(ModifyBase<ModifyDerive<DIBGameLevelManager, GameLevelManager>>& self) {
        auto enableBreakdown = jasmine::setting::get<bool>("enable-demon-breakdown");
        auto sendBreakdown = jasmine::setting::get<bool>("send-demon-breakdown");

        auto hook = jasmine::hook::get(self.m_hooks, "GameLevelManager::updateUserScore", enableBreakdown->getValue() && sendBreakdown->getValue());
        if (hook) {
            SettingChangedEventV3(GEODE_MOD_ID, "enable-demon-breakdown").listen([sendBreakdown, hook](std::shared_ptr<SettingV3> setting) {
                jasmine::hook::toggle(hook, std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue() && sendBreakdown->getValue());
            }).leak();

            SettingChangedEventV3(GEODE_MOD_ID, "send-demon-breakdown").listen([enableBreakdown, hook](std::shared_ptr<SettingV3> setting) {
                jasmine::hook::toggle(hook, std::static_pointer_cast<BoolSettingV3>(std::move(setting))->getValue() && enableBreakdown->getValue());
            }).leak();
        }
    }

    void updateUserScore() {
        GameLevelManager::updateUserScore();
        if (isDLActive("user_score")) user_data::upload(DemonsInBetween::createBreakdown());
    }
};
