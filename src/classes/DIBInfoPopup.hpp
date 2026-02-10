#include "../DemonsInBetween.hpp"
#include <Geode/ui/Popup.hpp>

class DIBInfoPopup : public geode::Popup {
protected:
    std::array<int, 21> m_completionCountClassic;
    std::array<int, 21> m_completionCountPlatformer;
    std::vector<cocos2d::CCSprite*> m_demonSprites;
    std::vector<cocos2d::CCLabelBMFont*> m_demonClassicLabels;
    std::vector<cocos2d::CCLabelBMFont*> m_demonPlatformerLabels;
    int m_page;

    bool init(const DemonBreakdown&);
    void onPrevPage(cocos2d::CCObject*);
    void onNextPage(cocos2d::CCObject*);
    void loadPage(int);
    void onClose(cocos2d::CCObject*) override;
    void keyDown(cocos2d::enumKeyCodes, double) override;
public:
    static DIBInfoPopup* create(const DemonBreakdown&);
};
