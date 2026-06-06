#pragma once

#include <Geode/Result.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/utils/web.hpp>

#include <arc/future/Future.hpp>
#include <matjson.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

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
    constexpr std::string_view BOOMLINGS = "http://www.boomlings.com/database/";
    constexpr std::string_view SECRET = "Wmfd2893gb7";

    using ServerResponse = geode::Result<std::string, int>;

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

    XBLAZE_DLL std::string buildBodyString(std::vector<std::pair<std::string, std::string>> body);

    /**
     * Do we have internet?
     */
    XBLAZE_DLL arc::Future<bool> doWeHaveInternet(const std::string& url = "http://connectivitycheck.gstatic.com/generate_204");

    // Proper confirm popup

    /**
     * Create a quick Yes/No popup.
     * @warning The async version of confirmYesNo() is deprecated and will be removed in v2.1.0
     *
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesBtn The text for the "Yes" button (obviously defaults to "Yes")
     * @param noBtn The text for the "No" button (obviously defaults to "No")
     *
     * NOTE: This function will only properly work
     * inside a coroutine, for non-coroutines use `confirmYesNoSync`
     */
    [[deprecated("The async version of 'confirmYesNo' is deprecated. This will be removed in v2.1.0")]]
    XBLAZE_DLL arc::Future<bool> confirmYesNo(
        std::string title,
        std::string msg,
        std::string yesBtn = "Yes",
        std::string noBtn = "No"
    );

    /**
     * Create a quick Yes/No popup.
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesCb The callback for when the user clicks "Yes"
     * @param noCb The callback for when the user clicks "No"
     * @param flip Reverse the Yes and No buttons
     */
    XBLAZE_DLL void confirmYesNo(
        std::string title,
        std::string msg,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb = [] {},
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
        std::string title,
        std::string msg,
        std::string yesBtn,
        std::string noBtn,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb = [] {},
        bool reverse = false
    );

    /**
     * Create a quick Yes/No popup.
     * This function is meant for when you're not in a coroutine.
     * @warning `confirmYesNoSync` is deprecated and will be removed in v2.1.0. Please use `confirmYesNo` instead
     *
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesCb The callback for when the user clicks "Yes"
     * @param noCb The callback for when the user clicks "No"
     */
    [[deprecated("Please use 'confirmYesNo' instead. This will be removed in v2.1.0")]]
    XBLAZE_DLL void confirmYesNoSync(
        std::string title,
        std::string msg,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb = [] {}
    );

    /**
     * Create a quick Yes/No popup.
     * This function is meant for when you're not in a coroutine.
     * @warning `confirmYesNoSync` is deprecated and will be removed in v2.1.0. Please use `confirmYesNo` instead
     *
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesBtn The text for the "Yes" button (obviously defaults to "Yes")
     * @param noBtn The text for the "No" button (obviously defaults to "No")
     * @param yesCb The callback for when the user clicks "Yes"
     */
    [[deprecated("Please use 'confirmYesNo' instead. This will be removed in v2.1.0")]]
    XBLAZE_DLL void confirmYesNoSync(
        std::string title,
        std::string msg,
        std::string yesBtn,
        std::string noBtn,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb = [] {}
    );

    // Notifications

    /**
     * Create a quick notification
     * @warning Not thread-safe, for async use cases use `quickNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickNotification(const std::string& msg, geode::NotificationIcon icon = geode::NotificationIcon::None);

    /**
     * Create a quick info notification
     * @warning Not thread-safe, for async use cases use `quickInfoNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickInfoNotification(const std::string& msg);

    /**
     * Create a quick warning notification
     * @warning Not thread-safe, for async use cases use `quickWarningNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickWarningNotification(const std::string& msg);

    /**
     * Create a quick error notification
     * @warning Not thread-safe, for async use cases use `quickErrorNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickErrorNotification(const std::string& msg);

    /**
     * Create a quick success notification
     * @warning Not thread-safe, for async use cases use `quickSuccessNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickSuccessNotification(const std::string& msg);

    /**
     * Create a quick loading notification
     * @warning Not thread-safe, for async use cases use `quickLoadingNotificationTS` instead
     */
    XBLAZE_DLL geode::Notification* quickLoadingNotification(const std::string& msg);

    // Notifications (thread-safe)

    /**
     * Create a quick notification (thread-salfe)
     */
    XBLAZE_DLL void quickNotificationTS(const std::string& msg, geode::NotificationIcon icon = geode::NotificationIcon::None);

    /**
     * Create a quick info notification (thread-safe)
     */
    XBLAZE_DLL void quickInfoNotificationTS(const std::string& msg);

    /**
     * Create a quick warning notification (thread-safe)
     */
    XBLAZE_DLL void quickWarningNotificationTS(const std::string& msg);

    /**
     * Create a quick error notification (thread-safe)
     */
    XBLAZE_DLL void quickErrorNotificationTS(const std::string& msg);

    /**
     * Create a quick success notification (thread-safe)
     */
    XBLAZE_DLL void quickSuccessNotificationTS(const std::string& msg);

    /**
     * Create a quick loading notification (thread-safe)
     */
    XBLAZE_DLL void quickLoadingNotificationTS(const std::string& msg);

    // Random stuff

    /**
     * Sleep for __ seconds
     */
    XBLAZE_DLL arc::Future<> sleepSecs(int s);

    /**
     * Sleep for __ milliseconds
     */
    XBLAZE_DLL arc::Future<> sleepMillis(int m);

    // Patreon's stuff

    /**
     * The tier that the user has on XblazeGMD's Patreon
     */
    enum class XBLAZE_DLL PatreonSupporterTier {
        None = 0,
        PlainNormalSupporter = 1,
        AmazingBeautifulCrab = 2
    };

    /**
     * Check if the current user is supporting XblazeGMD on Patreon
     */
    XBLAZE_DLL PatreonSupporterTier patreonSupporterStatus(int accountID);

    /**
     * Check if the user has any tier on XblazeGMD's Patreon.
     * This will only return `true` if the user is on a paid tier
     */
    XBLAZE_DLL inline bool isPatreonSupporter(int accountID);

    /**
     * Check if the user has the 'Plain Normal Supporter Tier' on XblazeGMD's Patreon
     */
    XBLAZE_DLL inline bool isPatreonPlainNormalSupporter(int accountID);

    /**
     * Check if the user has the 'Amazing Beautiful Crab Tier' on XblazeGMD's Patreon
     */
    XBLAZE_DLL inline bool isPatreonAmazingBeautifulCrab(int accountID);
}