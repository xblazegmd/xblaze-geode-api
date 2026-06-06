# Xblaze's Geode API
## 2.0.0
- Remove Patreon badges and deprecate Patreon APIs (no one's even a member on my patreon so it's useless)
- Make `xblazeapi::BOOMLINGS` and `xblazeapi::SECRET` `std::string` instead of `std::string_view`
- Make `confirmYesNo` synchronous and deprecate async `confirmYesNo` and `confirmYesNoSync`
- Add "reverse" option in `confirmYesNo`
- Add more quick notification utils

## 1.2.0
- Change badge fetching system to one that doesn't overwhelm my website (lol)
- Add web request utils

## 1.1.0
- Switch API endpoint to `xblaze.netlify.app`

## 1.0.1
- Fix unnecesary amount of requests to load Patreon badges

## 1.0.0
- Initial Release