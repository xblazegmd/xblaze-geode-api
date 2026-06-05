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

    void confirmYesNo(
        std::string title,
        std::string msg,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb,
        bool reverse
    ) {
        confirmYesNo(title, msg, "Yes", "No", std::move(yesCb), std::move(noCb), reverse);
    }

    void confirmYesNo(
        std::string title,
        std::string msg,
        std::string yesBtn,
        std::string noBtn,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb,
        bool reverse
    ) {
        std::string btn1 = reverse ? std::move(yesBtn) : std::move(noBtn);
        std::string btn2 = reverse ? std::move(noBtn) : std::move(yesBtn);

        geode::createQuickPopup(
            title.c_str(),
            msg.c_str(),
            btn1.c_str(), btn2.c_str(),
            [yesCb = std::move(yesCb), noCb = std::move(noCb), reverse](auto, bool btn2) mutable {
                bool yes = reverse ? !btn2 : btn2;
                if (yes) {
                    yesCb();
                } else {
                    noCb();
                }
            }
        );
    }

    void confirmYesNoSync(
        std::string title,
        std::string msg,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb
    ) {
        confirmYesNo(title, msg, std::move(yesCb), std::move(noCb));
    }

    void confirmYesNoSync(
        std::string title,
        std::string msg,
        std::string yesBtn,
        std::string noBtn,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb
    ) {
        confirmYesNo(title, msg, yesBtn, noBtn, std::move(yesCb), std::move(noCb));
    }
}