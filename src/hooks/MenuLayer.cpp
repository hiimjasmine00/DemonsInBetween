#include "../DemonsInBetween.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

#define GDDL_URL "https://docs.google.com/spreadsheets/d/1qKlWKpDkOpU1ZF6V6xGfutDY2NvcA8MNPnsv6GBkKPQ/gviz/tq?tqx=out:csv&sheet=GDDL"

class $modify(DIBMenuLayer, MenuLayer) {
    struct Fields {
        inline static bool TRIED_LOADING;
    };

    bool init() {
        if (!MenuLayer::init()) return false;

        if (Fields::TRIED_LOADING) return true;

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
                        demon.id = numFromString<int>(value).unwrapOr(-1);
                        switch (demon.id) {
                            case 1: demon.id = 14; break;
                            case 2: demon.id = 18; break;
                            case 3: demon.id = 20; break;
                        }
                    }
                    else if (key == "Tier") {
                        demon.tier = numFromString<double>(value).unwrapOr(0.0);
                        switch ((int)round(demon.tier)) {
                            case 1: demon.difficulty = 1; break;
                            case 2: demon.difficulty = 2; break;
                            case 3: demon.difficulty = 3; break;
                            case 4: demon.difficulty = 4; break;
                            case 5: demon.difficulty = 5; break;
                            case 6: demon.difficulty = 6; break;
                            case 7: demon.difficulty = 7; break;
                            case 8: demon.difficulty = 8; break;
                            case 9: demon.difficulty = 9; break;
                            case 10: demon.difficulty = 10; break;
                            case 11: case 12: demon.difficulty = 11; break;
                            case 13: demon.difficulty = 12; break;
                            case 14: demon.difficulty = 13; break;
                            case 15: case 16: demon.difficulty = 14; break;
                            case 17: case 18: demon.difficulty = 15; break;
                            case 19: demon.difficulty = 16; break;
                            case 20: demon.difficulty = 17; break;
                            case 21: demon.difficulty = 18; break;
                            case 22: demon.difficulty = 19; break;
                            case 23: case 24: case 25: case 26: case 27: case 28: case 29:
                            case 30: case 31: case 32: case 33: case 34: case 35: demon.difficulty = 20; break;
                            default: demon.difficulty = 0; break;
                        }

                        if (demon.difficulty > 0) DemonsInBetween::GDDL_DIFFICULTIES[demon.difficulty].push_back(std::to_string(demon.id));
                    }
                    else if (key == "Enjoyment") demon.enjoyment = numFromString<double>(value).unwrapOr(0.0);
                }

                if (demon.difficulty > 0) DemonsInBetween::GDDL.push_back(demon);
            }

            return task = std::nullopt, *res;
        });

        Fields::TRIED_LOADING = true;

        return true;
    }
};
