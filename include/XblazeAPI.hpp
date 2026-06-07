#pragma once

#include <Geode/Result.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/utils/web.hpp>

#include <arc/future/Future.hpp>
#include <matjson.hpp>

#include <initializer_list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#ifdef GEODE_IS_WINDOWS
    #ifdef XBLAZEAPI_EXPORTING
        #define XBLAZE_DLL __declspec(dllexport)
    #else
        #define XBLAZE_DLL __declspec(dllimport)
    #endif
#else
    #define XBLAZE_DLL __attribute__((visibility("default")))
#endif

#define XBLAZE_UNWRAP_INTO_FUTURE(var, ...) GEODE_UNWRAP_INTO_BASE(co_return, var, __VA_ARGS__)

namespace xblazeapi {
    // gd server stuff
    inline const std::string BOOMLINGS = "https://www.boomlings.com/database/";
    inline const std::string SECRET = "Wmfd2893gb7";

    using ServerResponse = geode::Result<std::string, int>;
    using VoidCallback = geode::Function<void()>;

    /**
     * Make a request to the Geometry Dash servers.
     * @param endpoint The endpoint the request will be sent to
     * @param body The request body
     * @return `Ok` if request was successful, `Err` if it failed
     *
     * NOTE: The `Err` value is an integer that can either be an http error
     * code ('xxx'), a gd server error (negative number) or 0 if anything else
     */
    XBLAZE_DLL arc::Future<ServerResponse> requestGDServers(
        std::string_view endpoint,
        std::string_view body,
        int timeout = 10
    );

    /**
     * Fix the spaghetti mess rob's server response is.
     *
     * NOTE: This will only work with the "key:val" pairs (other separators like "~" can
     * be specified). If the response is, let's say, multiple objects separated by a "|", you
     * will need to separate them yourself.
     */
    XBLAZE_DLL std::unordered_map<std::string, std::string> formatResponse(
        std::string_view response,
        std::string sep = ":"
    );

    // Other web request stuff

    XBLAZE_DLL std::string buildBodyString(std::initializer_list<std::pair<std::string, std::string>> body);

    /**
     * Do we have internet?
     */
    XBLAZE_DLL arc::Future<bool> doWeHaveInternet(const std::string& url = "http://connectivitycheck.gstatic.com/generate_204");

    // Proper confirm popup

    /**
     * Create a quick Yes/No popup.
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesCb The callback for when the user clicks "Yes"
     * @param noCb The callback for when the user clicks "No"
     * @param flip Reverse the Yes and No buttons
     */
    XBLAZE_DLL void confirmYesNo(
        geode::ZStringView title,
        geode::ZStringView msg,
        VoidCallback yesCb,
        VoidCallback noCb = [] {},
        bool reverse = false
    );

    /**
     * Create a quick Yes/No popup.
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesBtn The text for the "Yes" button (obviously defaults to "Yes")
     * @param noBtn The text for the "No" button (obviously defaults to "No")
     * @param yesCb The callback for when the user clicks "Yes"
     * @param noCb The callback for when the user clicks "No"
     * @param flip Reverse the Yes and No buttons
     */
    XBLAZE_DLL void confirmYesNo(
        geode::ZStringView title,
        geode::ZStringView msg,
        geode::ZStringView yesBtn,
        geode::ZStringView noBtn,
        VoidCallback yesCb,
        VoidCallback noCb = [] {},
        bool reverse = false
    );

    /**
     * Create a quick notification
     * @warning Not thread-safe, for async use cases use `quickNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickNotification(geode::ZStringView msg, geode::NotificationIcon icon = geode::NotificationIcon::None);

    /**
     * Create a quick info notification
     * @warning Not thread-safe, for async use cases use `quickInfoNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickInfoNotification(geode::ZStringView msg);

    /**
     * Create a quick warning notification
     * @warning Not thread-safe, for async use cases use `quickWarningNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickWarningNotification(geode::ZStringView msg);

    /**
     * Create a quick error notification
     * @warning Not thread-safe, for async use cases use `quickErrorNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickErrorNotification(geode::ZStringView msg);

    /**
     * Create a quick success notification
     * @warning Not thread-safe, for async use cases use `quickSuccessNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickSuccessNotification(geode::ZStringView msg);

    /**
     * Create a quick loading notification
     * @warning Not thread-safe, for async use cases use `quickLoadingNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickLoadingNotification(geode::ZStringView msg);

    // Notifications (thread-safe)

    /**
     * Create a quick notification (thread-salfe)
     */
    XBLAZE_DLL void quickNotificationTS(geode::ZStringView msg, geode::NotificationIcon icon = geode::NotificationIcon::None);

    /**
     * Create a quick info notification (thread-safe)
     */
    XBLAZE_DLL void quickInfoNotificationTS(geode::ZStringView msg);

    /**
     * Create a quick warning notification (thread-safe)
     */
    XBLAZE_DLL void quickWarningNotificationTS(geode::ZStringView msg);

    /**
     * Create a quick error notification (thread-safe)
     */
    XBLAZE_DLL void quickErrorNotificationTS(geode::ZStringView msg);

    /**
     * Create a quick success notification (thread-safe)
     */
    XBLAZE_DLL void quickSuccessNotificationTS(geode::ZStringView msg);

    /**
     * Create a quick loading notification (thread-safe)
     */
    XBLAZE_DLL void quickLoadingNotificationTS(geode::ZStringView msg);

    // Random stuff

    /**
     * Sleep for __ seconds
     */
    XBLAZE_DLL arc::Future<> sleepSecs(int s);

    /**
     * Sleep for __ milliseconds
     */
    XBLAZE_DLL arc::Future<> sleepMillis(int m);
}