#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>

#include <string>
#include <utility>

using namespace geode::prelude;

namespace xblazeapi {
    void confirmYesNo(
        geode::ZStringView title,
        geode::ZStringView msg,

        VoidCallback yesCb,
        VoidCallback noCb,

        bool reverse
    ) {
        confirmYesNo(title, msg, "Yes", "No", std::move(yesCb), std::move(noCb), reverse);
    }

    void confirmYesNo(
        geode::ZStringView title,
        geode::ZStringView msg,
        geode::ZStringView yesBtn,
        geode::ZStringView noBtn,

        VoidCallback yesCb,
        VoidCallback noCb,

        bool reverse
    ) {
        geode::ZStringView btn1 = reverse ? yesBtn : noBtn;
        geode::ZStringView btn2 = reverse ? noBtn : yesBtn;

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
}