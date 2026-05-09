#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/ProfilePage.hpp>

#include <argon/argon.hpp>
#include <string>

using namespace geode::prelude;

class $modify(PPHook, ProfilePage) {
    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);
        if (Mod::get()->getSettingValue<bool>("supporter-badge")) {
            addBadge(xblazeapi::patreonSupporterStatus(score->m_accountID));
        }
    }

    void addBadge(xblazeapi::PatreonSupporterTier tier) {
        if (tier == xblazeapi::PatreonSupporterTier::None) return;

        auto userMenu = static_cast<CCMenu*>(m_mainLayer->getChildByIDRecursive("username-menu"));

        auto badgeSprite = CCSprite::create(
            tier == xblazeapi::PatreonSupporterTier::PlainNormalSupporter ? "silver-supporter.png"_spr : "gold-supporter.png"_spr
        );

        badgeSprite->setScale(.5f);
        auto badge = CCMenuItemSpriteExtra::create(
            badgeSprite,
            this,
            menu_selector(PPHook::onBadge)
        );

        badge->setID("supporter-badge"_spr);
        userMenu->addChild(badge);
        userMenu->updateLayout();
    }

    void onUpdate(CCObject* sender) {
        if (auto userMenu = static_cast<CCMenu*>(m_mainLayer->getChildByIDRecursive("username-menu"))) {
            userMenu->removeChildByID("supporter-badge"_spr);
        }
        ProfilePage::onUpdate(sender);
    }

    void onBadge(CCObject*) {
        FLAlertLayer::create(
            "Supporter",
            "This player has supported developer <cj>XblazeGMD</c> by joining his <cy>Patreon!</c>",
            "OK"
        )->show();
    }
};