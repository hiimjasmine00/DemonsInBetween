#include <Geode/ui/Popup.hpp>

class DIBSearchPopup : public geode::Popup {
protected:
    bool init() override;
    void onSearch(cocos2d::CCObject* sender);
public:
    static DIBSearchPopup* create();
};
