#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>
#include <asp/iter/Split.hpp>

#include <string_view>
#include <unordered_map>

using namespace geode::prelude;

namespace xblazeapi {
    arc::Future<ServerResponse> requestGDServers(
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

        auto ret = res.string();

        if (ret.isErr()) {
            log::error("Could not get response from endpoint '{}': {}", endpoint, ret.unwrapErr());
            co_return Err(0);
        }
        auto unwrapped = ret.unwrap();
        auto num = utils::numFromString<int>(unwrapped);
        if (num.isOk() && num.unwrap() < 0) {
            co_return Err(num.unwrap());
        }

        co_return Ok(unwrapped);
    }

    std::unordered_map<std::string, std::string> formatResponse(std::string_view response, char sep) {
        std::unordered_map<std::string, std::string> map;
        map.reserve(static_cast<size_t>(std::count(response.begin(), response.end(), sep) * 2));

        size_t pos = 0;
        while (true) {
            const size_t keyPos = response.find(sep, pos);
            if (keyPos == response.npos) break;

            size_t valPos = response.find(sep, keyPos + 1);
            if (valPos == response.npos) valPos = response.size();

            std::string_view key = response.substr(pos, keyPos - pos);
            std::string_view val = response.substr(keyPos + 1, valPos - (keyPos + 1));
            
            map.insert({ key, val });
            pos = valPos + 1;
        }

        return map;
    }

    std::unordered_map<std::string, std::string> formatResponse(std::string_view response, std::string_view sep) {
        auto split = asp::iter::split(response, sep).arrayChunks<2>();
        return std::unordered_map<std::string, std::string>(split.begin(), split.end());
    }
}