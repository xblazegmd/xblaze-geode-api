#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

using namespace geode::prelude;

namespace xblazeapi {
    arc::Future<geode::Result<std::string, int>> requestGDServers(
        std::string_view endpoint,
        std::string_view body,
        int timeout
    ) {
        auto req = web::WebRequest()
            .userAgent("")
            .bodyString(body)
            .timeout(std::chrono::seconds(timeout));

        auto res = co_await req.post(fmt::format("{}{}", BOOMLINGS, endpoint));
        if (!res.ok()) {
            log::error("Failed to request endpoint '{}' ({}): {}", endpoint, res.code(), res.errorMessage());
            co_return Err(res.code());
        }

        if (res.string().isErr()) {
            log::error("Could not get response from endpoint '{}': {}", endpoint, res.string().unwrapErr());
            co_return Err(571116);
        }

        auto ret = res.string().unwrap();
        auto num = utils::numFromString<int>(ret);
        if (num.isOk() && num.unwrap() < 0) {
            co_return Err(num.unwrap());
        }

        co_return Ok(ret);
    }

    std::unordered_map<std::string, std::string> formatResponse(std::string_view response, std::string sep) {
        auto pieces = string::split(response, sep);
        std::unordered_map<std::string, std::string> ret;

        for (int i = 0; i < pieces.size(); i += 2) {
            ret[pieces[i]] = pieces[i + 1];
        }

        return ret;
    }
}