#include "DIBInfoPopup.hpp"
#include "../DemonsInBetween.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

#define GDDL_URL "https://docs.google.com/spreadsheets/d/1qKlWKpDkOpU1ZF6V6xGfutDY2NvcA8MNPnsv6GBkKPQ/gviz/tq?tqx=out:csv&sheet=GDDL"

DIBInfoPopup* DIBInfoPopup::create() {
    auto ret = new DIBInfoPopup();
    if (ret->initAnchored(380.0f, 210.0f, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool DIBInfoPopup::setup() {
    setID("DIBInfoPopup");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_closeBtn->setVisible(false);

    setKeyboardEnabled(true);

    auto okButton = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("OK", 0, 0, 1.0f, false, "goldFont.fnt", "GJ_button_01.png", 0.0f),
        this, menu_selector(DIBInfoPopup::onClose)
    );
    okButton->setPosition({ 190.0f, 25.0f });
    okButton->setID("ok-button");
    m_buttonMenu->addChild(okButton);

    m_demonSprites = CCArray::create();
    m_demonSprites->retain();

    m_demonClassicLabels = CCArray::create();
    m_demonClassicLabels->retain();

    m_demonPlatformerLabels = CCArray::create();
    m_demonPlatformerLabels->retain();

    for (int i = 0; i < 5; i++) {
        auto demonSprite = CCSprite::create();
        demonSprite->setPosition({ 50.0f + i * 70.0f, 140.0f });
        demonSprite->setID(fmt::format("demon-sprite-{}", i + 1).c_str());
        m_mainLayer->addChild(demonSprite);
        m_demonSprites->addObject(demonSprite);

        auto classicLabel = CCLabelBMFont::create("", "goldFont.fnt");
        classicLabel->setScale(0.6f);
        classicLabel->setPosition({ 50.0f + i * 70.0f, 90.0f });
        classicLabel->setID(fmt::format("classic-label-{}", i + 1).c_str());
        m_mainLayer->addChild(classicLabel);
        m_demonClassicLabels->addObject(classicLabel);

        auto platformerLabel = CCLabelBMFont::create("", "goldFont.fnt");
        platformerLabel->setScale(0.6f);
        platformerLabel->setPosition({ 50.0f + i * 70.0f, 64.0f });
        platformerLabel->setColor({ 255, 200, 255 });
        platformerLabel->setID(fmt::format("platformer-label-{}", i + 1).c_str());
        m_mainLayer->addChild(platformerLabel);
        m_demonPlatformerLabels->addObject(platformerLabel);
    }

    auto glm = GameLevelManager::get();
    auto gsm = GameStatsManager::get();
     for (auto [onlineID, level] : CCDictionaryExt<std::string, GJGameLevel*>(glm->m_onlineLevels)) {
        if (level->m_stars.value() < 10 || level->m_normalPercent.value() < 100 || !gsm->hasCompletedLevel(level)) continue;

        auto& completionCount = level->m_levelLength == 5 ? m_completionCountPlatformer : m_completionCountClassic;
        completionCount[0]++;

        auto& demon = DemonsInBetween::demonForLevel(level->m_levelID.value());
        if (demon.id == 0 || demon.difficulty == 0) switch (level->m_demonDifficulty) {
            case 3: completionCount[4]++; break;
            case 4: completionCount[7]++; break;
            case 0: completionCount[11]++; break;
            case 5: completionCount[15]++; break;
            case 6: completionCount[20]++; break;
        }
        else completionCount[demon.difficulty - 1]++;
    }

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

    auto weeklyCompleted = 0;
    auto eventCompleted = 0;
    auto gauntletCompleted = 0;

    for (auto [dailyID, dailyLevel] : CCDictionaryExt<std::string, GJGameLevel*>(glm->m_dailyLevels)) {
        if (dailyLevel->m_stars.value() < 10 || dailyLevel->m_normalPercent.value() < 100 || !gsm->hasCompletedLevel(dailyLevel)) continue;
        if (dailyLevel->m_dailyID > 200000) eventCompleted++;
        else if (dailyLevel->m_dailyID > 100000) weeklyCompleted++;
    }

    for (auto [levelID, gauntletLevel] : CCDictionaryExt<std::string, GJGameLevel*>(glm->m_gauntletLevels)) {
        if (gauntletLevel->m_stars.value() < 10 || gauntletLevel->m_normalPercent.value() < 100 || !gsm->hasCompletedLevel(gauntletLevel)) continue;
        gauntletCompleted++;
    }

    auto gauntletLabel = CCLabelBMFont::create(fmt::format("Gauntlet: {}", gauntletCompleted).c_str(), "goldFont.fnt");
    gauntletLabel->setAnchorPoint({ 1.0f, 0.0f });
    gauntletLabel->setID("gauntlet-label");
    bottomRightMenu->addChild(gauntletLabel);

    auto eventLabel = CCLabelBMFont::create(fmt::format("Event: {}", eventCompleted).c_str(), "goldFont.fnt");
    eventLabel->setAnchorPoint({ 1.0f, 0.0f });
    eventLabel->setID("event-label");
    bottomRightMenu->addChild(eventLabel);

    auto weeklyLabel = CCLabelBMFont::create(fmt::format("Weekly: {}", weeklyCompleted).c_str(), "goldFont.fnt");
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
        demonSprite->setPosition(CCPoint { 50.0f + i * 70.0f, 140.0f } + DemonsInBetween::offsetForDifficulty(difficulty, GJDifficultyName::Long));
        static_cast<CCLabelBMFont*>(m_demonClassicLabels->objectAtIndex(i))->setString(fmt::format("{}", m_completionCountClassic[difficulty]).c_str());
        static_cast<CCLabelBMFont*>(m_demonPlatformerLabels->objectAtIndex(i))->setString(fmt::format("{}", m_completionCountPlatformer[difficulty]).c_str());
    }
}

void DIBInfoPopup::onClose(CCObject* sender) {
    setKeyboardEnabled(false);
    Popup<>::onClose(sender);
}

void DIBInfoPopup::keyDown(enumKeyCodes key) {
    switch (key) {
        case KEY_Left: case CONTROLLER_Left: return loadPage(m_page - 1);
        case KEY_Right: case CONTROLLER_Right: return loadPage(m_page + 1);
        default: return Popup<>::keyDown(key);
    }
}

DIBInfoPopup::~DIBInfoPopup() {
    CC_SAFE_RELEASE(m_demonSprites);
    CC_SAFE_RELEASE(m_demonClassicLabels);
    CC_SAFE_RELEASE(m_demonPlatformerLabels);
}
