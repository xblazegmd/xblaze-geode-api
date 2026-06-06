#include <XblazeAPI.hpp>

#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

namespace xblazeapi {
    XBLAZE_DLL geode::Notification* quickNotification(const std::string& msg, geode::NotificationIcon icon) {
        auto ret = Notification::create(msg, icon);
        ret->show();
        return ret;
    }

    XBLAZE_DLL geode::Notification* quickInfoNotification(const std::string& msg) {
        return quickNotification(msg, NotificationIcon::Info);
    }

    XBLAZE_DLL geode::Notification* quickWarningNotification(const std::string& msg) {
        return quickNotification(msg, NotificationIcon::Warning);
    }

    XBLAZE_DLL geode::Notification* quickErrorNotification(const std::string& msg) {
        return quickNotification(msg, NotificationIcon::Error);
    }

    XBLAZE_DLL geode::Notification* quickSuccessNotification(const std::string& msg) {
        return quickNotification(msg, NotificationIcon::Success);
    }

    XBLAZE_DLL geode::Notification* quickLoadingNotification(const std::string& msg) {
        return quickNotification(msg, NotificationIcon::Loading);
    }

    XBLAZE_DLL void quickNotificationTS(const std::string& msg, geode::NotificationIcon icon) {
        geode::queueInMainThread([msg, icon] {
            quickNotification(msg, icon);
        });
    }

    XBLAZE_DLL void quickInfoNotificationTS(const std::string& msg) {
        quickNotificationTS(msg, NotificationIcon::Info);
    }

    XBLAZE_DLL void quickWarningNotificationTS(const std::string& msg) {
        quickNotificationTS(msg, NotificationIcon::Warning);
    }

    XBLAZE_DLL void quickErrorNotificationTS(const std::string& msg) {
        quickNotificationTS(msg, NotificationIcon::Error);
    }

    XBLAZE_DLL void quickSuccessNotificationTS(const std::string& msg) {
        quickNotificationTS(msg, NotificationIcon::Success);
    }

    XBLAZE_DLL void quickLoadingNotificationTS(const std::string& msg) {
        quickNotificationTS(msg, NotificationIcon::Loading);
    }
}