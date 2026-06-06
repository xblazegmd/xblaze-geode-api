<img src="logo.png" width="80" alt="the mod's logo" />

# Xblaze's Geode API
A collection of the most random utilities ever for my Geode mods

## Installation
Add this mod as a dependency on your `mod.json`
```json
"dependencies": {
    "xblazegmd.geode-api": ">=1.0.0"
}
```

Now you can include it like this
```cpp
#include <xblazegmd.geode-api/include/XblazeAPI.hpp>
```

## Utils
### Server Utils
This API adds a bunch of utilities to make working with the GD servers less of a headache

First there's some variables defined that contain data you may find useful
```cpp
// URL to the GD servers (keep in mind it ends with "/")
constexpr std::string_view BOOMLINGS = "http://www.boomlings.com/database/";

// The common secret
constexpr std::string_view SECRET = "Wmfd2893gb7";
```

There's also this nice function to make a request to the GD servers:
```cpp
auto res = co_await xblazeapi::requestGDServers("gjServerEndpoint.php", "request=body");
```

But even this function cannot save you of the pain of having to manually format the absolute mess of a response the GD servers give you... but what abt another function?
```cpp
// key:val:key:val to 'unordered_map'
auto formatted = xblazeapi::formatResponse(res.unwrap());

// Change separator
auto formatted = xblazeapi::formatResponse(res.unwrap(), "~");
```

### Web Utils
Web requests are nice and easy. However there's a lot of stuff you need to write just to make a simple web request. So hopefully these utilities help you with some of the steps
 
First of all there's this nice cross-platform helper function to check if you have internet:
```cpp
auto connected = co_await xblazeapi::doWeHaveInternet();
if (!connected) {
    // do stuff...
}
```

This function will check the internet connection with `GameToolbox::doWeHaveInternet` on mobile, and with a simple web request on PC.

The default URL to make a request to is `http://connectivitycheck.gstatic.com/generate_204`, however you can specify whichever one you wish:
```cpp
auto customURL = co_await xblazeapi::doWeHaveInternet("https://www.google.com"); // Check the connection with Google
```

Building POST request bodies can be a tedious process if you're using `bodyString`. While you could use other methods, if you REALLY need to use `bodyString`, there is a nice helper function to help you make it in a less tedious way:
```cpp
xblazeapi::buildBodyString({
    { "this", "that" },
    { "number", 1 },
    { "subscribe", "toMyYt" }
});
```

### Confirm popups
Have you ever wanted to just have an easier way to make a simple "Yes/No" popup? Sure, `geode::createQuickPopup` is good enough, but what abt a more *convenient* way?
```cpp
// Yes/No
auto res = xblazeapi::confirmYesNo(
    "Title", "Message",
    [] {
        // Callback if user clicked "Yes"
    },
    [] {
        // Callback if user clicked "No"
    }
);

// Confirm/Cancel
auto res = xblazeapi::confirmYesNo(
    "Title", "Message",
    "Confirm", "Cancel",
    [] { /* yes* */ }, [] { /* no */ }
);
```

The buttons will by default show up as "No, Yes" with yes being the second button. You can reverse this by calling the function like this
```cpp
auto res = xblazeapi::confirmYesNo(
    "Title", "Message",
    [] { /* yes* */ }, [] { /* no */ },
    true // reverse
);
```

### Quick Notifications
Geode's `Notification` is very useful for showing status updates for whatever you need to the user. By default to make a notification you define it like:
```cpp
Notification::create("Contents", NotificationIcon::Info)->show();
```

That is a ton of code. So I made a few utility functions to help with this
```cpp
xblazeapi::quickInfoNotification("Information"); // NotificationIcon::Info
xblazeapi::quickWarningNotification("Beware!"); // NotificationIcon::Warning
xblazeapi::quickErrorNotification("Oh no!"); // NotificationIcon::Error
xblazeapi::quickSuccessNotification("We did it!"); // NotificationIcon::Sucess
xblazeapi::quickLoadingNotification("Loading..."); // NotificationIcon::Loading
xblazeapi::quickNotification("No icon!"); // NotificationIcon::None
```

The functions listed above are not thread-safe, i.e. this would crash:
```cpp
async::spawn(xblazeapi::quickNotification("Risky")); // Crash!
```

If you *really* need to show the notification on a separate thread you can use the thread-safe version of the utilities:
```cpp
async::spawn(xblazeapi::quickNotificationTS("thread-safe! (no not this s**t)"));
```

### Random stuff
```cpp
// Simpler sleep functions
co_await xblazeapi::sleepSecs(10);
co_await xblazeapi::sleepMillis(100);

// GEODE_UNWRAP_INTO but for futures
XBLAZE_UNWRAP_INTO_FUTURE(int var, riskyFunction());
```