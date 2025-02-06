#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>

class DIBSearchPopup : public geode::Popup<> {
protected:
    geode::EventListener<geode::utils::web::WebTask> m_listener;
    bool m_isBusy;

    bool setup() override;
public:
    static DIBSearchPopup* create();
};
