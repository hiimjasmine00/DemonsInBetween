#include "DIBSearchPopup.hpp"
#include "../DemonsInBetween.hpp"
#include <Geode/loader/Mod.hpp>
#include <jasmine/nodes.hpp>

using namespace geode::prelude;
using namespace jasmine::nodes;

DIBSearchPopup* DIBSearchPopup::create() {
    auto ret = new DIBSearchPopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool DIBSearchPopup::init() {
    if (!Popup::init(350.0f, 280.0f)) return false;

    setID("DIBSearchPopup");
    setTitle("Quick Search");
    m_title->setID("quick-search-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    auto table = TableNode::create(5, 4, 350.0f, 240.0f, "search-button-row");
    table->setPosition({ 175.0f, 130.0f });
    table->setID("search-buttons");
    m_mainLayer->addChild(table);

    for (int i = 1; i < 21; i++) {
        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(fmt::format("DIB_{:02d}_btn2_001.png"_spr, i).c_str()),
            this, menu_selector(DIBSearchPopup::onSearch)
        );
        button->setTag(i);
        button->setID(fmt::format("search-button-{}", i));
        table->addButton(button);
    }

    table->updateAllLayouts();

    return true;
}

void DIBSearchPopup::onSearch(CCObject* sender) {
    if (auto scene = DemonsInBetween::browseScene(sender->getTag())) {
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
    }
}
