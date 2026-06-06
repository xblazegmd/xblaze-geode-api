#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>

#include <initializer_list>
#include <utility>

using namespace geode::prelude;

namespace xblazeapi {
    std::string buildBodyString(std::initializer_list<std::pair<std::string, std::string>> body) {
        fmt::memory_buffer out;

        for (const auto& [k, v] : body)
            fmt::format_to(std::back_inserter(out), "{}={}&", k, v);

        return fmt::to_string(out);
    }

    arc::Future<bool> doWeHaveInternet(const std::string& url) {
        #ifdef GEODE_IS_MOBILE
            // I have no idea whether this is thread-safe or not, not risking it lol
            auto check = co_await async::waitForMainThread([] {
                return GameToolbox::doWeHaveInternet();
            });
            co_return check;
        #else
            auto check = co_await web::WebRequest()
                .userAgent("GeometryDash/2.2081")
                .timeout(std::chrono::seconds(10))
                .get(url);
            co_return check.ok();
        #endif
    }
}