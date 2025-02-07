#include <Geode/ui/Popup.hpp>

class DIBSearchPopup : public geode::Popup<> {
protected:
    bool setup() override;
public:
    static DIBSearchPopup* create();
};
