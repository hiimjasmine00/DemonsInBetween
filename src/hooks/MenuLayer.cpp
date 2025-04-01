#include "../DemonsInBetween.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

#define GDDL_URL "https://docs.google.com/spreadsheets/d/1qKlWKpDkOpU1ZF6V6xGfutDY2NvcA8MNPnsv6GBkKPQ/gviz/tq?tqx=out:csv&sheet=GDDL"

class $modify(DIBMenuLayer, MenuLayer) {
    inline static constexpr std::array difficulties = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 12, 13, 14, 14, 15, 15,
        16, 17, 18, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20
    };
    inline static constexpr std::array mainLevels = { 0, 14, 18, 20 };
    inline static bool triedLoading;

    bool init() {
        if (!MenuLayer::init()) return false;

        if (triedLoading) return true;

        static std::optional<web::WebTask> task = std::nullopt;
        task = web::WebRequest().get(GDDL_URL).map([](web::WebResponse* res) {
            if (!res->ok()) return task = std::nullopt, *res;

            auto data = res->string().unwrapOr("");
            auto lines = string::split(data, "\n");
            auto keys = string::split(lines[0].substr(1, lines[0].size() - 2), "\",\"");
            for (size_t i = 1; i < lines.size(); i++) {
                auto values = string::split(lines[i].substr(1, lines[i].size() - 2), "\",\"");
                LadderDemon demon = { 0, 0.0, 0.0, 0 };
                for (size_t j = 0; j < keys.size(); j++) {
                    auto key = keys[j];
                    auto value = values[j];
                    if (key == "ID") {
                        auto id = numFromString<int>(value).unwrapOr(-1);
                        demon.id = id < mainLevels.size() ? mainLevels[id] : id;
                    }
                    else if (key == "Tier") {
                        demon.tier = numFromString<double>(value).unwrapOr(0.0);
                        auto roundedTier = (int)round(demon.tier);
                        demon.difficulty = roundedTier < difficulties.size() ? difficulties[roundedTier] : 0;
                        if (demon.difficulty > 0) DemonsInBetween::gddlDifficulties[demon.difficulty].push_back(std::to_string(demon.id));
                    }
                    else if (key == "Enjoyment") demon.enjoyment = numFromString<double>(value).unwrapOr(0.0);
                }

                if (demon.difficulty > 0) DemonsInBetween::gddl.push_back(demon);
            }

            return task = std::nullopt, *res;
        });

        triedLoading = true;

        return true;
    }
};
