#include "DIBSearchPopup.hpp"
#include "TableNode.hpp"
#include "../DemonsInBetween.hpp"
#include <Geode/binding/LevelBrowserLayer.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

DIBSearchPopup* DIBSearchPopup::create() {
    auto ret = new DIBSearchPopup();
    if (ret->initAnchored(350.0f, 280.0f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool DIBSearchPopup::setup() {
    setTitle("Quick Search");
    m_noElasticity = true;

    auto table = TableNode::create(5, 4);
    table->setContentSize({ 350.0f, 240.0f });
    table->setColumnLayout(ColumnLayout::create()->setAxisReverse(true));
    table->setRowLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    table->setRowHeight(60.0f);
    table->setPosition({ 175.0f, 130.0f });
    m_mainLayer->addChild(table);

    for (int i = 1; i < 21; i++) {
        table->addButton(CCMenuItemExt::createSpriteExtraWithFrameName(fmt::format("DIB_{:02d}_btn2_001.png"_spr, i).c_str(), 1.0f, [this, i](auto) {
            if (m_isBusy) return;
            m_isBusy = true;
            DemonsInBetween::DIFFICULTY = i;
            DemonsInBetween::SEARCHING = true;
            DemonsInBetween::searchObjectForPage(std::move(m_listener), 0, false, [this](GJSearchObject* obj) {
                CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, LevelBrowserLayer::scene(obj)));
                m_isBusy = false;
            });
        }));
    }

    table->updateAllLayouts();

    return true;
}
