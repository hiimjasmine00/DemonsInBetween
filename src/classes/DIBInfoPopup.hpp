#include "../DemonsInBetween.hpp"
#include <Geode/ui/Popup.hpp>

class DIBInfoPopup : public geode::Popup<const DemonBreakdown&> {
protected:
    std::array<int, 21> m_completionCountClassic;
    std::array<int, 21> m_completionCountPlatformer;
    geode::Ref<cocos2d::CCArray> m_demonSprites;
    geode::Ref<cocos2d::CCArray> m_demonClassicLabels;
    geode::Ref<cocos2d::CCArray> m_demonPlatformerLabels;
    int m_page;

    bool setup(const DemonBreakdown&) override;
    void setupDemonInfo();
    void loadPage(int);
    void onClose(CCObject*) override;
    void keyDown(cocos2d::enumKeyCodes) override;
public:
    static DIBInfoPopup* create(const DemonBreakdown&);
};
