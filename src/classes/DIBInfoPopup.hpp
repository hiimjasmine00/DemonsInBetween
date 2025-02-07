#include <Geode/ui/Popup.hpp>

class DIBInfoPopup : public geode::Popup<> {
protected:
    cocos2d::CCArray* m_demonSprites;
    cocos2d::CCArray* m_demonClassicLabels;
    cocos2d::CCArray* m_demonPlatformerLabels;
    int m_page;
    std::vector<int> m_completionCountClassic = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<int> m_completionCountPlatformer = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    bool setup() override;
    void setupDemonInfo();
    void loadPage(int);
    void onClose(CCObject*) override;
    void keyDown(cocos2d::enumKeyCodes) override;
public:
    static DIBInfoPopup* create();

    ~DIBInfoPopup() override;
};
