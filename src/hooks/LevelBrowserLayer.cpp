#include "../DemonsInBetween.hpp"
#include <Geode/binding/SetIDPopup.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <random>

using namespace geode::prelude;
class $modify(DIBLevelBrowserLayer, LevelBrowserLayer) {
    struct Fields {
        int m_currentPage;
    };

    static void onModify(ModifyBase<ModifyDerive<DIBLevelBrowserLayer, LevelBrowserLayer>>& self) {
        (void)self.setHookPriorityAfterPost("LevelBrowserLayer::init", "cvolton.betterinfo");
        (void)self.setHookPriorityAfterPost("LevelBrowserLayer::setupPageInfo", "cvolton.betterinfo");
        (void)self.setHookPriorityAfterPost("LevelBrowserLayer::loadPage", "cvolton.betterinfo");
    }

    bool init(GJSearchObject* searchObject) {
        if (!LevelBrowserLayer::init(searchObject)) return false;

        if (!DemonsInBetween::SEARCHING) return true;

        if (auto pageMenu = getChildByID("page-menu")) {
            if (auto randomButton = static_cast<CCMenuItemSpriteExtra*>(pageMenu->getChildByID("cvolton.betterinfo/random-button")))
                randomButton->m_pfnSelector = menu_selector(DIBLevelBrowserLayer::onBetterInfoRandom);
            if (auto lastButton = static_cast<CCMenuItemSpriteExtra*>(pageMenu->getChildByID("cvolton.betterinfo/last-button")))
                lastButton->m_pfnSelector = menu_selector(DIBLevelBrowserLayer::onBetterInfoLast);
        }
        if (auto searchMenu = getChildByID("search-menu")) {
            if (auto firstButton = static_cast<CCMenuItemSpriteExtra*>(searchMenu->getChildByID("cvolton.betterinfo/first-button")))
                firstButton->m_pfnSelector = menu_selector(DIBLevelBrowserLayer::onBetterInfoFirst);
        }

        updatePageButtons();

        return true;
    }

    void onBetterInfoRandom(CCObject* sender) {
        if (!DemonsInBetween::SEARCHING) return;

        static std::mt19937 mt(std::random_device{}());
        
        loadPage(DemonsInBetween::searchObjectForPage(m_fields->m_currentPage = std::uniform_int_distribution<int>(0, DemonsInBetween::MAX_PAGE)(mt)));
    }

    void onBetterInfoLast(CCObject* sender) {
        if (!DemonsInBetween::SEARCHING) return;
        
        loadPage(DemonsInBetween::searchObjectForPage(m_fields->m_currentPage = DemonsInBetween::MAX_PAGE));
    }

    void onBetterInfoFirst(CCObject* sender) {
        if (!DemonsInBetween::SEARCHING) return;
        
        loadPage(DemonsInBetween::searchObjectForPage(m_fields->m_currentPage = 0));
    }

    void updatePageButtons() {
        if (!DemonsInBetween::SEARCHING) return;

        auto f = m_fields.self();

        if (m_leftArrow) m_leftArrow->setVisible(f->m_currentPage > 0);
        if (m_rightArrow) m_rightArrow->setVisible(f->m_currentPage < DemonsInBetween::MAX_PAGE);
        if (m_pageBtn) m_pageBtn->setVisible(DemonsInBetween::SEARCH_SIZE > 10);
        if (m_refreshBtn) m_refreshBtn->setVisible(false);

        if (auto pageMenu = getChildByID("page-menu")) {
            if (auto randomButton = pageMenu->getChildByID("cvolton.betterinfo/random-button"))
                randomButton->setVisible(DemonsInBetween::SEARCH_SIZE > 10);
            if (auto lastButton = pageMenu->getChildByID("cvolton.betterinfo/last-button"))
                lastButton->setVisible(f->m_currentPage < DemonsInBetween::MAX_PAGE);
        }
        if (auto searchMenu = getChildByID("search-menu")) {
            if (auto firstButton = searchMenu->getChildByID("cvolton.betterinfo/first-button"))
                firstButton->setVisible(f->m_currentPage > 0);
        }
    }

    void loadPage(GJSearchObject* searchObject) {
        LevelBrowserLayer::loadPage(searchObject);

        if (!DemonsInBetween::SEARCHING) return;

        updatePageButtons();
    }

    void loadLevelsFinished(CCArray* levels, const char* key, int type) override {
        LevelBrowserLayer::loadLevelsFinished(levels, key, type);

        if (!DemonsInBetween::SEARCHING) return;

        updatePageButtons();
    }

    void setupPageInfo(gd::string pageInfo, const char* key) override {
        LevelBrowserLayer::setupPageInfo(pageInfo, key);

        if (!DemonsInBetween::SEARCHING) return;

        auto f = m_fields.self();
        m_countText->setString(fmt::format("{} to {} of {}",
            f->m_currentPage * 10 + 1, std::min(DemonsInBetween::SEARCH_SIZE, (f->m_currentPage + 1) * 10), DemonsInBetween::SEARCH_SIZE).c_str());
        m_countText->limitLabelWidth(100.0f, 0.6f, 0.0f);
        m_pageText->setString(std::to_string(f->m_currentPage + 1).c_str());
        m_pageText->limitLabelWidth(32.0f, 0.8f, 0.0f);

        updatePageButtons();
    }

    void onBack(CCObject* sender) override {
        LevelBrowserLayer::onBack(sender);

        if (!DemonsInBetween::SEARCHING) return;

        DemonsInBetween::DIFFICULTY = 0;
        DemonsInBetween::SEARCHING = false;
        DemonsInBetween::SEARCH_SIZE = 0;
        DemonsInBetween::MAX_PAGE = 0;
    }

    void setIDPopupClosed(SetIDPopup* popup, int page) override {
        LevelBrowserLayer::setIDPopupClosed(popup, page);

        if (!DemonsInBetween::SEARCHING || popup->getTag() == 4) return;

        loadPage(DemonsInBetween::searchObjectForPage(m_fields->m_currentPage = std::min(page - 1, DemonsInBetween::MAX_PAGE)));
    }

    void onGoToPage(CCObject* sender) {
        LevelBrowserLayer::onGoToPage(sender);

        if (!DemonsInBetween::SEARCHING) return;

        if (auto popup = CCScene::get()->getChildByType<SetIDPopup>(0)) {
            popup->m_value = m_fields->m_currentPage + 1;
            popup->updateTextInputLabel();
        }
    }

    void onNextPage(CCObject* sender) {
        LevelBrowserLayer::onNextPage(sender);

        if (!DemonsInBetween::SEARCHING) return;

        loadPage(DemonsInBetween::searchObjectForPage(++m_fields->m_currentPage));
    }

    void onPrevPage(CCObject* sender) {
        LevelBrowserLayer::onPrevPage(sender);

        if (!DemonsInBetween::SEARCHING) return;

        loadPage(DemonsInBetween::searchObjectForPage(--m_fields->m_currentPage));
    }
#ifdef GEODE_IS_MACOS // In the Geometry Dash binary for macOS, onNextPage and onPrevPage are inlined into keyDown
    void keyDown(enumKeyCodes key) override {
        LevelBrowserLayer::keyDown(key);

        if (!DemonsInBetween::SEARCHING) return;

        switch (key) {
            case KEY_Left: case CONTROLLER_Left:
                if (m_leftArrow && m_leftArrow->isVisible() && m_leftArrow->isEnabled())
                    loadPage(DemonsInBetween::searchObjectForPage(--m_fields->m_currentPage));
                break;
            case KEY_Right: case CONTROLLER_Right:
                if (m_rightArrow && m_rightArrow->isVisible() && m_rightArrow->isEnabled())
                    loadPage(DemonsInBetween::searchObjectForPage(++m_fields->m_currentPage));
                break;
            default:
                break;
        }
    }
#endif
};
