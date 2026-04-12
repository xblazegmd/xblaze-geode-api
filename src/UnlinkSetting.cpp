#include "UnlinkSetting.hpp"

#include <Geode/Geode.hpp>

#include <argon/argon.hpp>
#include <memory>

using namespace geode::prelude;

Result<std::shared_ptr<SettingV3>> UnlinkSetting::parse(
    const std::string &key,
    const std::string &modID,
    const matjson::Value &json
) {
    auto res = std::make_shared<UnlinkSetting>();
    auto root = checkJson(json, "RemoveSetting");

    res->init(key, modID, root);
    res->parseNameAndDescription(root);
    res->parseEnableIf(root);

    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

SettingNodeV3* UnlinkSetting::createNode(float width) {
    return UnlinkSettingNode::create(
        std::static_pointer_cast<UnlinkSetting>(shared_from_this()),
        width
    );
}

UnlinkSettingNode* UnlinkSettingNode::create(std::shared_ptr<UnlinkSetting> setting, float width) {
    auto ret = new UnlinkSettingNode();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool UnlinkSettingNode::init(std::shared_ptr<UnlinkSetting> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) return false;
    this->getNameLabel()->setVisible(false);

    auto btnSprite = ButtonSprite::create(
        "Unlink Patreon Account",
        "goldFont.fnt",
        "GJ_button_01.png",
        .8f
    );
    btnSprite->setScale(.7f);
    auto btn = CCMenuItemSpriteExtra::create(btnSprite, this, menu_selector(UnlinkSettingNode::onUnlink));

    auto btnMenu = this->getButtonMenu();

    btnMenu->addChildAtPosition(btn, Anchor::Center);
    btnMenu->setContentWidth(width);
    btnMenu->setPositionX(width);
    btnMenu->updateLayout();

    this->updateState(nullptr);

    return true;
}

void UnlinkSettingNode::onUnlink(CCObject*) {
    geode::createQuickPopup(
        "Are you sure?",
        "This will unlink your <co>Patreon account</c> from your <cl>Geometry Dash account</c> making your supporter badge dissapear",
        "No", "Yes",
        [this](auto, bool btn2) {
            if (btn2) {
                auto unlinkingNot = Notification::create("Unlinking Patreon account...", NotificationIcon::Loading, 15);
                unlinkingNot->show();

                auto accountData = argon::getGameAccountData();
                m_unlinkTask.spawn(
                    "unlinkAccount",
                    unlinkAccount(accountData.accountId, std::move(accountData)),
                    [unlinkingNot](Result<> res) {
                        unlinkingNot->cancel();
                        if (res.isErr()) {
                            Notification::create(
                                fmt::format("Failed to unlink Patreon: {}", res.unwrapErr()),
                                NotificationIcon::Error
                            )->show();
                        } else {
                            Notification::create("Successfully unlinked Patreon account!", NotificationIcon::Success)->show();
                        }
                    }
                );
            }
        }
    );
}

arc::Future<Result<>> UnlinkSettingNode::unlinkAccount(int accountID, argon::AccountData accData) {
    auto tokenRes = co_await argon::startAuth(std::move(accData));
    if (tokenRes.isErr()) {
        argon::clearToken(accountID);
        co_return Err("Argon auth failed: {}", tokenRes.unwrapErr());
    }

    auto token = std::move(tokenRes).unwrap();

    auto req = web::WebRequest()
        .userAgent("GeometryDash/2.2081 XblazeAPI/1.0.0")
        .header("Authorization", token)
        .timeout(std::chrono::seconds(10));

    auto res = co_await req.send("DELETE", fmt::format("https://xblazegmd.vercel.app/patreon/remove?accountID={}", accountID));
    if (!res.ok()) {
        co_return Err("{} ({})", res.errorMessage(), res.code());
    }

    co_return Ok();
}

$execute {
    (void)Mod::get()->registerCustomSettingType("remove-btn", &UnlinkSetting::parse);
}