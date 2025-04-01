#include "../DemonsInBetween.hpp"
#include <Geode/binding/SetIDPopup.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <random>

using namespace geode::prelude;
class $modify(DIBLevelBrowserLayer, LevelBrowserLayer) {
    struct Fields {
        int m_difficulty;
        int m_searchSize;
        int m_maxPage;
        int m_currentPage;
    };

    static void onModify(ModifyBase<ModifyDerive<DIBLevelBrowserLayer, LevelBrowserLayer>>& self) {
        (void)self.setHookPriorityAfterPost("LevelBrowserLayer::init", "cvolton.betterinfo");
        (void)self.setHookPriorityAfterPost("LevelBrowserLayer::setupPageInfo", "cvolton.betterinfo");
        (void)self.setHookPriorityAfterPost("LevelBrowserLayer::loadPage", "cvolton.betterinfo");
    }

    bool init(GJSearchObject* searchObject) {
        if (!LevelBrowserLayer::init(searchObject)) return false;

        if (m_fields->m_difficulty <= 0) return true;

        if (auto pageMenu = getChildByID("page-menu")) {
            if (auto randomButton = static_cast<CCMenuItem*>(pageMenu->getChildByID("cvolton.betterinfo/random-button")))
                randomButton->m_pfnSelector = menu_selector(DIBLevelBrowserLayer::onBetterInfoRandom);
            if (auto lastButton = static_cast<CCMenuItem*>(pageMenu->getChildByID("cvolton.betterinfo/last-button")))
                lastButton->m_pfnSelector = menu_selector(DIBLevelBrowserLayer::onBetterInfoLast);
        }
        if (auto searchMenu = getChildByID("search-menu")) {
            if (auto firstButton = static_cast<CCMenuItem*>(searchMenu->getChildByID("cvolton.betterinfo/first-button")))
                firstButton->m_pfnSelector = menu_selector(DIBLevelBrowserLayer::onBetterInfoFirst);
        }

        updatePageButtons();

        return true;
    }

    void onBetterInfoRandom(CCObject* sender) {
        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        static std::mt19937 mt(std::random_device{}());

        loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, f->m_currentPage = std::uniform_int_distribution<int>(0, f->m_maxPage)(mt)));
    }

    void onBetterInfoLast(CCObject* sender) {
        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, f->m_currentPage = f->m_maxPage));
    }

    void onBetterInfoFirst(CCObject* sender) {
        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, f->m_currentPage = 0));
    }

    void updatePageButtons() {
        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        if (m_leftArrow) m_leftArrow->setVisible(f->m_currentPage > 0);
        if (m_rightArrow) m_rightArrow->setVisible(f->m_currentPage < f->m_maxPage);
        if (m_pageBtn) m_pageBtn->setVisible(f->m_maxPage > 10);
        if (m_refreshBtn) m_refreshBtn->setVisible(false);

        if (auto pageMenu = getChildByID("page-menu")) {
            if (auto randomButton = pageMenu->getChildByID("cvolton.betterinfo/random-button"))
                randomButton->setVisible(f->m_maxPage > 10);
            if (auto lastButton = pageMenu->getChildByID("cvolton.betterinfo/last-button"))
                lastButton->setVisible(f->m_currentPage < f->m_maxPage);
        }
        if (auto searchMenu = getChildByID("search-menu")) {
            if (auto firstButton = searchMenu->getChildByID("cvolton.betterinfo/first-button"))
                firstButton->setVisible(f->m_currentPage > 0);
        }
    }

    void loadPage(GJSearchObject* searchObject) {
        LevelBrowserLayer::loadPage(searchObject);

        if (m_fields->m_difficulty <= 0) return;

        updatePageButtons();
    }

    void loadLevelsFinished(CCArray* levels, const char* key, int type) override {
        LevelBrowserLayer::loadLevelsFinished(levels, key, type);

        if (m_fields->m_difficulty <= 0) return;

        updatePageButtons();
    }

    void setupPageInfo(gd::string pageInfo, const char* key) override {
        LevelBrowserLayer::setupPageInfo(pageInfo, key);

        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        m_countText->setString(fmt::format("{} to {} of {}",
            f->m_currentPage * 10 + 1, std::min(f->m_searchSize, (f->m_currentPage + 1) * 10), f->m_searchSize).c_str());
        m_countText->limitLabelWidth(100.0f, 0.6f, 0.0f);
        m_pageText->setString(std::to_string(f->m_currentPage + 1).c_str());
        m_pageText->limitLabelWidth(32.0f, 0.8f, 0.0f);

        updatePageButtons();
    }

    void setIDPopupClosed(SetIDPopup* popup, int page) override {
        LevelBrowserLayer::setIDPopupClosed(popup, page);

        auto f = m_fields.self();
        if (f->m_difficulty <= 0 || popup->getTag() == 4) return;

        loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, f->m_currentPage = std::min(page - 1, f->m_maxPage)));
    }

    void onGoToPage(CCObject* sender) {
        LevelBrowserLayer::onGoToPage(sender);

        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        if (auto popup = CCScene::get()->getChildByType<SetIDPopup>(0)) {
            popup->m_value = f->m_currentPage + 1;
            popup->updateTextInputLabel();
        }
    }

    void onNextPage(CCObject* sender) {
        LevelBrowserLayer::onNextPage(sender);

        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, ++f->m_currentPage));
    }

    void onPrevPage(CCObject* sender) {
        LevelBrowserLayer::onPrevPage(sender);

        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, --f->m_currentPage));
    }
#ifdef GEODE_IS_MACOS // In the Geometry Dash binary for macOS, onNextPage and onPrevPage are inlined into keyDown
    void keyDown(enumKeyCodes key) override {
        LevelBrowserLayer::keyDown(key);

        auto f = m_fields.self();
        if (f->m_difficulty <= 0) return;

        switch (key) {
            case KEY_Left: case CONTROLLER_Left:
                if (m_leftArrow && m_leftArrow->isVisible() && m_leftArrow->isEnabled())
                    loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, --f->m_currentPage));
                break;
            case KEY_Right: case CONTROLLER_Right:
                if (m_rightArrow && m_rightArrow->isVisible() && m_rightArrow->isEnabled())
                    loadPage(DemonsInBetween::searchObjectForPage(f->m_difficulty, ++f->m_currentPage));
                break;
            default:
                break;
        }
    }
#endif
};

// Devilish activities
CCScene* DemonsInBetween::browseScene(int difficulty) {
    auto scene = CCScene::create();
    auto layer = static_cast<DIBLevelBrowserLayer*>(new LevelBrowserLayer());
    auto f = layer->m_fields.self();
    f->m_difficulty = difficulty;
    auto& levels = DemonsInBetween::gddlDifficulties[difficulty];
    f->m_searchSize = levels.size();
    f->m_maxPage = (levels.size() - 1) / 10;
    if (layer->init(DemonsInBetween::searchObjectForPage(difficulty, 0))) {
        scene->addChild(static_cast<CCNode*>(layer->autorelease()));
        return scene;
    }
    delete layer;
    return nullptr;
}
