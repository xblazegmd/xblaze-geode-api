#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>

#include <initializer_list>
#include <string>
#include <utility>

using namespace geode::prelude;

namespace xblazeapi {
    std::string buildBodyString(std::initializer_list<std::pair<std::string, std::string>> body) {
        std::vector<std::string> ret;
        for (const auto& [k, v] : body) {
            ret.push_back(fmt::format("{}={}", k, v));
        }
        return string::join(ret, "&");
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