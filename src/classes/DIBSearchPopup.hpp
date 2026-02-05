#include <Geode/ui/Popup.hpp>

class DIBSearchPopup : public geode::Popup {
protected:
    bool init() override;
public:
    static DIBSearchPopup* create();
};
