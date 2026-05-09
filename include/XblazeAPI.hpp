#pragma once

#include <Geode/Result.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/utils/web.hpp>

#include <arc/future/Future.hpp>
#include <matjson.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

#ifdef GEODE_IS_WINDOWS
    #ifdef XBLAZEAPI_EXPORTING
        #define XBLAZE_DLL __declspec(dllexport)
    #else
        #define XBLAZE_DLL __declspec(dllimport)
    #endif
#else
    #define XBLAZE_DLL __attribute__((visibility("default")))
#endif

namespace xblazeapi {
    // gd server stuff
    constexpr std::string_view BOOMLINGS = "http://www.boomlings.com/database/";
    constexpr std::string_view SECRET = "Wmfd2893gb7";

    /**
     * Make a request to the Geometry Dash servers.
     * @param endpoint The endpoint the request will be sent to
     * @param body The request body
     * @return `Ok` if request was successful, `Err` if it failed
     *
     * NOTE: `Err` returns an `int`. If it's negative it's an error code returned by the
     * servers themselves, if it's a "xxx" like thing it's an HTTP status code, and if it's
     * "571116" that means `res.string().isErr()` is true
     */
    XBLAZE_DLL arc::Future<geode::Result<std::string, int>> requestGDServers(
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

    // Proper confirm popup

    /**
     * Create a quick Yes/No popup.
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesBtn The text for the "Yes" button (obviously defaults to "Yes")
     * @param noBtn The text for the "No" button (obviously defaults to "No")
     *
     * NOTE: This function will only properly work
     * inside a coroutine, for non-coroutines use `confirmYesNoSync`
     */
    XBLAZE_DLL arc::Future<bool> confirmYesNo(
        std::string title,
        std::string msg,
        std::string yesBtn = "Yes",
        std::string noBtn = "No"
    );

    /**
     * Create a quick Yes/No popup.
     * This function is meant for when you're not in a coroutine
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesCb The callback for when the user clicks "Yes"
     * @param noCb The callback for when the user clicks "No"
     *
     * NOTE: For coroutines use `confirmYesNo`
     */
    XBLAZE_DLL void confirmYesNoSync(
        std::string title,
        std::string msg,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb = [] {}
    );

    /**
     * Create a quick Yes/No popup.
     * This function is meant for when you're not in a coroutine
     * @param title The title for the popup
     * @param msg The message for the popup
     * @param yesBtn The text for the "Yes" button (obviously defaults to "Yes")
     * @param noBtn The text for the "No" button (obviously defaults to "No")
     * @param yesCb The callback for when the user clicks "Yes"
     * @param noCb The callback for when the user clicks "No"
     *
     * NOTE: For coroutines use `confirmYesNo`
     */
    XBLAZE_DLL void confirmYesNoSync(
        std::string title,
        std::string msg,
        std::string yesBtn,
        std::string noBtn,
        geode::Function<void()> yesCb,
        geode::Function<void()> noCb = [] {}
    );

    // Random stuff

    /**
     * Sleep for __ seconds
     */
    XBLAZE_DLL arc::Future<> sleepSecs(int s);

    /**
     * Sleep for __ milliseconds
     */
    XBLAZE_DLL arc::Future<> sleepMillis(int m);

    /**
     * Create a quick error notification (not thread-safe, use `quickErrorNotificationTS` instead)
     */
    XBLAZE_DLL geode::Notification* quickErrorNotification(const std::string& msg);

    /**
     * Create a quick error notification (thread-safe)
     */
    XBLAZE_DLL void quickErrorNotificationTS(const std::string& msg);

    /**
     * Do we have internet?
     */
    XBLAZE_DLL arc::Future<bool> doWeHaveInternet();

    /**
     * Do we have internet?
     */
    XBLAZE_DLL arc::Future<bool> doWeHaveInternet(const std::string& checkUrl);

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