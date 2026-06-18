#include "../DemonsInBetween.hpp"
#include <Geode/ui/Popup.hpp>

class DIBInfoPopup : public geode::Popup {
protected:
    std::array<int, 21> m_completionCountClassic;
    std::array<int, 21> m_completionCountPlatformer;
    std::array<cocos2d::CCSprite*, 5> m_demonSprites;
    std::array<cocos2d::CCLabelBMFont*, 5> m_demonClassicLabels;
    std::array<cocos2d::CCLabelBMFont*, 5> m_demonPlatformerLabels;
    int m_page;

    bool init(const DemonBreakdown& breakdown);
    void onPrevPage(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void loadPage(int page);
    void onClose(cocos2d::CCObject* sender) override;
    void keyDown(cocos2d::enumKeyCodes key, double timestamp) override;
public:
    static DIBInfoPopup* create(const DemonBreakdown& breakdown);
};
