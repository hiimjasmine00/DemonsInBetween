#pragma once
#include <Geode/loader/Event.hpp>
#include <map>

struct LadderDemon {
    int id = 0;
    double tier = 0.0;
    double enjoyment = 0.0;
    int difficulty = 0;
};

class DemonsInBetweenLoadedEvent : public geode::Event<DemonsInBetweenLoadedEvent, bool(const std::map<int, LadderDemon>&)> {
public:
    using Event::Event;
};
