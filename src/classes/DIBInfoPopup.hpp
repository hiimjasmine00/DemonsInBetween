#include "../DemonsInBetween.hpp"
#include <Geode/ui/Popup.hpp>

class DIBInfoPopup : public geode::Popup<DemonBreakdown> {
protected:
    std::vector<int> m_completionCountClassic;
    std::vector<int> m_completionCountPlatformer;
    geode::Ref<cocos2d::CCArray> m_demonSprites;
    geode::Ref<cocos2d::CCArray> m_demonClassicLabels;
    geode::Ref<cocos2d::CCArray> m_demonPlatformerLabels;
    int m_page;

    bool setup(DemonBreakdown) override;
    void setupDemonInfo();
    void loadPage(int);
    void onClose(CCObject*) override;
    void keyDown(cocos2d::enumKeyCodes) override;
public:
    static DIBInfoPopup* create(DemonBreakdown);
};
