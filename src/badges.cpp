#include <XblazeAPI.hpp>

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/ProfilePage.hpp>

#include <argon/argon.hpp>
#include <string>

using namespace geode::prelude;

class $modify(PPHook, ProfilePage) {
    struct Fields {
        async::TaskHolder<Result<xblazeapi::PatreonSupporterTier>> m_patreonTask;
        bool m_loaded = false;
    };

    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);
        if (Mod::get()->getSettingValue<bool>("supporter-badge")) {
            loadBadge(score->m_accountID);
        }
    }

    void loadBadge(int accountID) {
        if (m_fields->m_loaded) {
            addBadge(stringToTier(Mod::get()->getSavedValue<std::string>("patreon-tier")));
            return;
        }

        m_fields->m_patreonTask.spawn(
            "Patreon Tier Fetcher",
            xblazeapi::patreonSupporterStatus(accountID),
            [this](Result<xblazeapi::PatreonSupporterTier> tier) {
                if (tier.isErr()) {
                    log::error("Could not get patreon tier: {}", tier.unwrapErr());
                    return;
                }

                Mod::get()->setSavedValue("patreon-tier", tierToString(tier.unwrap()));
                addBadge(tier.unwrap());
                m_fields->m_loaded = true;
            }
        );
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

    void onBadge(CCObject*) {
        FLAlertLayer::create(
            "Supporter",
            "This player has supported developer <cj>XblazeGMD</c> by joining his <cy>Patreon!</c>",
            "OK"
        )->show();
    }

    void onUpdate(CCObject* sender) {
        m_fields->m_loaded = false;
        ProfilePage::onUpdate(sender);
    }

    std::string tierToString(xblazeapi::PatreonSupporterTier tier) {
        switch (tier) {
            case xblazeapi::PatreonSupporterTier::None:
                return "none";
            case xblazeapi::PatreonSupporterTier::PlainNormalSupporter:
                return "plainnormalsupporter";
            case xblazeapi::PatreonSupporterTier::AmazingBeautifulCrab:
                return "amazingbeautifulcrab";
        }
    }

    xblazeapi::PatreonSupporterTier stringToTier(const std::string& str) {
        if (str == "plainnormalsupporter") {
            return xblazeapi::PatreonSupporterTier::PlainNormalSupporter;
        } else if (str == "amazingbeautifulcrab") {
            return xblazeapi::PatreonSupporterTier::AmazingBeautifulCrab;
        }
        return  xblazeapi::PatreonSupporterTier::None;
    }
};