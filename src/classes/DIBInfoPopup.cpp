#include "DIBInfoPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

DIBInfoPopup* DIBInfoPopup::create(const DemonBreakdown& breakdown) {
    auto ret = new DIBInfoPopup();
    if (ret->init(breakdown)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool DIBInfoPopup::init(const DemonBreakdown& breakdown) {
    if (!Popup::init(380.0f, 210.0f, "GJ_square02.png")) return false;

    setID("DIBInfoPopup");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_closeBtn->setVisible(false);

    setKeyboardEnabled(true);

    auto okButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(DIBInfoPopup::onClose));
    okButton->setPosition({ 190.0f, 25.0f });
    okButton->setID("ok-button");
    m_buttonMenu->addChild(okButton);

    m_demonSprites = CCArray::create();
    m_demonClassicLabels = CCArray::create();
    m_demonPlatformerLabels = CCArray::create();

    for (int i = 0; i < 5; i++) {
        auto demonSprite = CCSprite::create();
        demonSprite->setPosition({ 50.0f + i * 70.0f, 140.0f });
        demonSprite->setID(fmt::format("demon-sprite-{}", i + 1));
        m_mainLayer->addChild(demonSprite);
        m_demonSprites->addObject(demonSprite);

        auto classicLabel = CCLabelBMFont::create("", "goldFont.fnt");
        classicLabel->setScale(0.6f);
        classicLabel->setPosition({ 50.0f + i * 70.0f, 90.0f });
        classicLabel->setID(fmt::format("classic-label-{}", i + 1));
        m_mainLayer->addChild(classicLabel);
        m_demonClassicLabels->addObject(classicLabel);

        auto platformerLabel = CCLabelBMFont::create("", "goldFont.fnt");
        platformerLabel->setScale(0.6f);
        platformerLabel->setPosition({ 50.0f + i * 70.0f, 64.0f });
        platformerLabel->setColor({ 255, 200, 255 });
        platformerLabel->setID(fmt::format("platformer-label-{}", i + 1));
        m_mainLayer->addChild(platformerLabel);
        m_demonPlatformerLabels->addObject(platformerLabel);
    }

    m_completionCountClassic = breakdown.classic;
    m_completionCountPlatformer = breakdown.platformer;

    auto classicLabel = CCLabelBMFont::create(fmt::format("Classic: {}", m_completionCountClassic[0]).c_str(), "goldFont.fnt");
    classicLabel->setScale(0.7f);
    classicLabel->setID("classic-label");
    m_mainLayer->addChild(classicLabel);

    auto platformerLabel = CCLabelBMFont::create(fmt::format("Platformer: {}", m_completionCountPlatformer[0]).c_str(), "goldFont.fnt");
    platformerLabel->setScale(0.7f);
    platformerLabel->setColor({ 255, 200, 255 });
    platformerLabel->setID("platformer-label");
    m_mainLayer->addChild(platformerLabel);

    auto titleLabels = CCArray::create();
    titleLabels->addObject(classicLabel);
    titleLabels->addObject(platformerLabel);

    GameToolbox::alignItemsHorisontally(titleLabels, 30.0f, { 190.0f, 190.0f }, false);

    auto bottomRightMenu = CCMenu::create();
    bottomRightMenu->setPosition({ 368.0f, 14.0f });
    bottomRightMenu->setAnchorPoint({ 1.0f, 0.0f });
    bottomRightMenu->setContentSize({ 40.0f, 100.0f });
    bottomRightMenu->setScale(0.4f);
    bottomRightMenu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisLineAlignment(AxisAlignment::End)
            ->setGap(10.0f)
    );
    bottomRightMenu->setID("bottom-right-menu");
    m_mainLayer->addChild(bottomRightMenu, 1);

    auto gauntletLabel = CCLabelBMFont::create(fmt::format("Gauntlet: {}", breakdown.gauntlet).c_str(), "goldFont.fnt");
    gauntletLabel->setAnchorPoint({ 1.0f, 0.0f });
    gauntletLabel->setID("gauntlet-label");
    bottomRightMenu->addChild(gauntletLabel);

    auto eventLabel = CCLabelBMFont::create(fmt::format("Event: {}", breakdown.event).c_str(), "goldFont.fnt");
    eventLabel->setAnchorPoint({ 1.0f, 0.0f });
    eventLabel->setID("event-label");
    bottomRightMenu->addChild(eventLabel);

    auto weeklyLabel = CCLabelBMFont::create(fmt::format("Weekly: {}", breakdown.weekly).c_str(), "goldFont.fnt");
    weeklyLabel->setAnchorPoint({ 1.0f, 0.0f });
    weeklyLabel->setID("weekly-label");
    bottomRightMenu->addChild(weeklyLabel);

    bottomRightMenu->updateLayout();

    auto prevButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_01_001.png", 1.0f, [this](auto) { loadPage(m_page - 1); });
    prevButton->setPosition({ -20.0f, 105.0f });
    prevButton->setID("prev-button");
    m_buttonMenu->addChild(prevButton);

    auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextButtonSprite->setFlipX(true);
    auto nextButton = CCMenuItemExt::createSpriteExtra(nextButtonSprite, [this](auto) { loadPage(m_page + 1); });
    nextButton->setPosition({ 400.0f, 105.0f });
    nextButton->setID("next-button");
    m_buttonMenu->addChild(nextButton);

    loadPage(0);

    return true;
}

void DIBInfoPopup::loadPage(int page) {
    m_page = (4 + (page % 4)) % 4;

    auto sfc = CCSpriteFrameCache::get();
    for (int i = 0; i < 5; i++) {
        auto difficulty = m_page * 5 + i + 1;
        auto demonSprite = static_cast<CCSprite*>(m_demonSprites->objectAtIndex(i));
        demonSprite->setDisplayFrame(sfc->spriteFrameByName(fmt::format("DIB_{:02d}_btn2_001.png"_spr, difficulty).c_str()));
        demonSprite->setPosition(DemonsInBetween::offsetForDifficulty(difficulty, GJDifficultyName::Long) + CCPoint { 50.0f + i * 70.0f, 140.0f });
        static_cast<CCLabelBMFont*>(m_demonClassicLabels->objectAtIndex(i))->setString(
            fmt::to_string(m_completionCountClassic[difficulty]).c_str());
        static_cast<CCLabelBMFont*>(m_demonPlatformerLabels->objectAtIndex(i))->setString(
            fmt::to_string(m_completionCountPlatformer[difficulty]).c_str());
    }
}

void DIBInfoPopup::onClose(CCObject* sender) {
    setKeyboardEnabled(false);
    Popup::onClose(sender);
}

void DIBInfoPopup::keyDown(enumKeyCodes key, double timestamp) {
    switch (key) {
        case KEY_Left: case CONTROLLER_Left: return loadPage(m_page - 1);
        case KEY_Right: case CONTROLLER_Right: return loadPage(m_page + 1);
        default: return Popup::keyDown(key, timestamp);
    }
}
