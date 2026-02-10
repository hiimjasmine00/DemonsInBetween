#include <Geode/ui/Popup.hpp>

class DIBSearchPopup : public geode::Popup {
protected:
    bool init() override;

    void onSearch(cocos2d::CCObject*);
public:
    static DIBSearchPopup* create();
};
