#include "DIBSearchPopup.hpp"
#include "TableNode.hpp"
#include "../DemonsInBetween.hpp"
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
    setID("DIBSearchPopup");
    setTitle("Quick Search");
    m_title->setID("quick-search-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    auto table = TableNode::create(5, 4);
    table->setContentSize({ 350.0f, 240.0f });
    table->setColumnLayout(ColumnLayout::create()->setAxisReverse(true));
    table->setRowLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    table->setRowHeight(60.0f);
    table->setRowPrefix("search-button-row");
    table->setPosition({ 175.0f, 130.0f });
    table->setID("search-buttons");
    m_mainLayer->addChild(table);

    for (int i = 1; i < 21; i++) {
        auto button = CCMenuItemExt::createSpriteExtraWithFrameName(fmt::format("DIB_{:02d}_btn2_001.png"_spr, i), 1.0f, [i](auto) {
            if (auto scene = DemonsInBetween::browseScene(i)) CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
        });
        button->setID(fmt::format("search-button-{}", i));
        table->addButton(button);
    }

    table->updateAllLayouts();

    return true;
}
