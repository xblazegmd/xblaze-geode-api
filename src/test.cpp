#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>
#include <asp/time/Instant.hpp>

$on_game(Loaded) {
    auto now = asp::time::Instant::now();
    for (size_t i = 0; i < 1000; i++) {
      xblazeapi::formatResponse("9:10:10:39431612:12:0:13:21:14:4125578:17::43:3:25::18:2:19:7730:42:0:45:20000:15:3:30:0:31:0:28:5 years:29:1 year:35:557117:36:0_733_0_0_0_0_574_716_0_0_352_78_729_0_42_0_833_68_0_347_0_38_240_205_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0:37:3:38:1:39:2:46:7729:47:13773:40:0:27:AwMABAYDBw==", ':');
    }
    auto time = now.elapsed();
    geode::log::info("formatResponse: {}", time);
}