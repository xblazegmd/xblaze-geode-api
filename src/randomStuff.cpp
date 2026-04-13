#include <XblazeAPI.hpp>

#include <arc/prelude.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace xblazeapi {
    arc::Future<> sleepSecs(int s) {
        co_await arc::sleep(asp::Duration::fromSecs(s));
    }

    arc::Future<> sleepMillis(int m) {
        co_await arc::sleep(asp::Duration::fromMillis(m));
    }
}