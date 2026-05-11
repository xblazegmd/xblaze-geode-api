#include <XblazeAPI.hpp>

#include <arc/prelude.hpp>
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/StringMap.hpp>

#include <string>
#include <vector>

using namespace geode::prelude;

static std::vector<utils::StringMap<std::string>> members = {};

$execute {
    async::spawn(
        web::WebRequest()
            .timeout(std::chrono::seconds(10))
            .get("https://raw.githubusercontent.com/xblazegmd/website/refs/heads/members/members.min.json"),
        [](web::WebResponse res) {
            if (!res.ok()) {
                log::error("Failed to get patreon members: {}", res.code());
                return;
            }

            if (res.json().isErr()) {
                log::error("Coult not get JSON data: {}", res.json().unwrapErr());
                return;
            }

            auto list = res.json().unwrap().asArray();
            if (list.isErr()) {
                log::error("Coult not get JSON data: {}", list.unwrapErr());
                return;
            }

            for (const auto& member : list.unwrap()) {
                utils::StringMap<std::string> m;
                m["accountID"] = member["accountID"].asString().unwrapOr("0");
                m["tier"] = member["tier"].asString().unwrapOr("none");
                members.push_back(m);
            }
        }
    );
}

namespace xblazeapi {
    PatreonSupporterTier patreonSupporterStatus(int accountID) {
        for (auto& member : members) {
            if (member["accountID"] != utils::numToString(accountID)) continue;
            auto tier = member["tier"];
            if (tier == "none") {
                return PatreonSupporterTier::None;
            } else if (tier == "normal") {
                return PatreonSupporterTier::PlainNormalSupporter;
            } else if (tier == "carb") {
                return PatreonSupporterTier::AmazingBeautifulCrab;
            }
        }
        return PatreonSupporterTier::None;
    }

    inline bool isPatreonSupporter(int accountID) {
        return patreonSupporterStatus(accountID) != PatreonSupporterTier::None;
    }

    inline bool isPatreonPlainNormalSupporter(int accountID) {
        return patreonSupporterStatus(accountID) == PatreonSupporterTier::PlainNormalSupporter;
    }

    inline bool isPatreonAmazingBeautifulCrab(int accountID) {
        return patreonSupporterStatus(accountID) == PatreonSupporterTier::AmazingBeautifulCrab;
    }
}