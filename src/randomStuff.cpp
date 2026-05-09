#include <XblazeAPI.hpp>

#include <arc/prelude.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace xblazeapi {
    arc::Future<> sleepSecs(int s) {
        co_await arc::sleep(asp::Duration::fromSecs(s));
    }

    arc::Future<> sleepMillis(int m) {
        co_await arc::sleep(asp::Duration::fromMillis(m));
    }

    geode::Notification* quickErrorNotification(const std::string& msg) {
        auto ret = Notification::create(msg, NotificationIcon::Error);
        ret->show();
        return ret;
    }

    void quickErrorNotificationTS(const std::string& msg) {
        geode::queueInMainThread([msg] {
            Notification::create(msg, NotificationIcon::Error)->show();
        });
    }

    arc::Future<bool> doWeHaveInternet() {
        return doWeHaveInternet("http://connectivitycheck.gstatic.com/generate_204");
    }

    arc::Future<bool> doWeHaveInternet(const std::string& url) {
        auto check = co_await web::WebRequest()
            .userAgent("GeometryDash/2.2081")
            .timeout(std::chrono::seconds(10))
            .get(url);
        co_return check.ok();
    }
}