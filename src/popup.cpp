#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>

#include <string>
#include <utility>

using namespace geode::prelude;

namespace xblazeapi {
    arc::Future<bool> confirmYesNo(
        std::string title,
        std::string msg,
        std::string yesBtn,
        std::string noBtn
    ) {
        bool ret = false;
        arc::Notify notify;

        geode::queueInMainThread([title, msg, yesBtn, noBtn, &ret, notify] {
            geode::createQuickPopup(
                title.c_str(),
                msg.c_str(),
                noBtn.c_str(), yesBtn.c_str(),
                [&ret, notify](auto, bool btn2) {
                    ret = btn2;
                    notify.notifyAll();
                }
            );
        });

        co_await notify.notified();
        co_return ret;
    }

    void confirmYesNoSync(
        std::string title,
        std::string msg,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb
    ) {
        confirmYesNoSync(title, msg, "Yes", "No", std::move(yesCb), std::move(noCb));
    }

    void confirmYesNoSync(
        std::string title,
        std::string msg,
        std::string yesBtn,
        std::string noBtn,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb
    ) {
        geode::createQuickPopup(
            title.c_str(),
            msg.c_str(),
            noBtn.c_str(), yesBtn.c_str(),
            [yesCb = std::move(yesCb), noCb = std::move(noCb)](auto, bool btn2) mutable {
                if (btn2) {
                    yesCb();
                } else {
                    noCb();
                }
            }
        );
    }
}