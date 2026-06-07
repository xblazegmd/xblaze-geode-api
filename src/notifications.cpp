#include <XblazeAPI.hpp>

#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

namespace xblazeapi {
    XBLAZE_DLL geode::Notification* quickNotification(geode::ZStringView msg, geode::NotificationIcon icon) {
        auto ret = Notification::create(msg, icon);
        ret->show();
        return ret;
    }

    XBLAZE_DLL geode::Notification* quickInfoNotification(geode::ZStringView msg) {
        return quickNotification(msg, NotificationIcon::Info);
    }

    XBLAZE_DLL geode::Notification* quickWarningNotification(geode::ZStringView msg) {
        return quickNotification(msg, NotificationIcon::Warning);
    }

    XBLAZE_DLL geode::Notification* quickErrorNotification(geode::ZStringView msg) {
        return quickNotification(msg, NotificationIcon::Error);
    }

    XBLAZE_DLL geode::Notification* quickSuccessNotification(geode::ZStringView msg) {
        return quickNotification(msg, NotificationIcon::Success);
    }

    XBLAZE_DLL geode::Notification* quickLoadingNotification(geode::ZStringView msg) {
        return quickNotification(msg, NotificationIcon::Loading);
    }

    XBLAZE_DLL void quickNotificationTS(geode::ZStringView msg, geode::NotificationIcon icon) {
        geode::queueInMainThread([msg, icon] {
            quickNotification(msg, icon);
        });
    }

    XBLAZE_DLL void quickInfoNotificationTS(geode::ZStringView msg) {
        quickNotificationTS(msg, NotificationIcon::Info);
    }

    XBLAZE_DLL void quickWarningNotificationTS(geode::ZStringView msg) {
        quickNotificationTS(msg, NotificationIcon::Warning);
    }

    XBLAZE_DLL void quickErrorNotificationTS(geode::ZStringView msg) {
        quickNotificationTS(msg, NotificationIcon::Error);
    }

    XBLAZE_DLL void quickSuccessNotificationTS(geode::ZStringView msg) {
        quickNotificationTS(msg, NotificationIcon::Success);
    }

    XBLAZE_DLL void quickLoadingNotificationTS(geode::ZStringView msg) {
        quickNotificationTS(msg, NotificationIcon::Loading);
    }
}