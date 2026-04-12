#include <XblazeAPI.hpp>

#include <arc/prelude.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace xblazeapi {
    arc::Future<Result<PatreonSupporterTier>> patreonSupporterStatus(int accountID, int timeout) {
        auto req = web::WebRequest()
            .userAgent("GeometryDash/2.2081 XblazeAPI/1.0.0")
            .timeout(std::chrono::seconds(timeout));

        auto res = co_await req.get(fmt::format("https://xblazegmd.vercel.app/patreon/tier?accountID={}", accountID));
        if (!res.ok()) {
            co_return Err("Failed to request API (): {}", res.code(), res.errorMessage());
        }

        if (res.json().isErr()) {
            co_return Err("Could not get response from API: {}", res.json().unwrapErr());
        }

        auto json = res.json().unwrap();

        // The usual return format is { tier: <tier-id> }
        // 'tier-id' can be casted to "PatreonSupporterTier"
        auto tierId = json["tier"].asInt();
        if (tierId.isErr()) {
            co_return Err("Could not get tier id: {}", tierId.unwrapErr());
        }

        co_return Ok(static_cast<PatreonSupporterTier>(tierId.unwrap()));
    }

    arc::Future<Result<bool>> isPatreonSupporter(int accountID, int timeout) {
        auto supporterStatus = co_await patreonSupporterStatus(accountID, timeout);
        if (supporterStatus.isErr()) {
            co_return Err("{}", supporterStatus.unwrapErr());
        }
        co_return Ok(supporterStatus.unwrap() != PatreonSupporterTier::None);
    }

    arc::Future<Result<bool>> isPatreonPlainNormalSupporter(int accountID, int timeout) {
        auto supporterStatus = co_await patreonSupporterStatus(accountID, timeout);
        if (supporterStatus.isErr()) {
            co_return Err("{}", supporterStatus.unwrapErr());
        }
        co_return Ok(supporterStatus.unwrap() == PatreonSupporterTier::PlainNormalSupporter);
    }

    arc::Future<Result<bool>> isPatreonAmazingBeautifulCrab(int accountID, int timeout) {
        auto supporterStatus = co_await patreonSupporterStatus(accountID, timeout);
        if (supporterStatus.isErr()) {
            co_return Err("{}", supporterStatus.unwrapErr());
        }
        co_return Ok(supporterStatus.unwrap() == PatreonSupporterTier::AmazingBeautifulCrab);
    }
}