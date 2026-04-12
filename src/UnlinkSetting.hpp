#pragma once

#include <Geode/loader/SettingV3.hpp>
#include <Geode/Result.hpp>
#include <Geode/utils/async.hpp>

#include <argon/argon.hpp>
#include <arc/future/Future.hpp>
#include <matjson.hpp>
#include <memory>

class UnlinkSetting : public geode::SettingV3 {
public:
    static geode::Result<std::shared_ptr<geode::SettingV3>> parse(
        const std::string& key,
        const std::string& modID,
        const matjson::Value& json
    );

    bool load(const matjson::Value& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
    bool isDefaultValue() const override { return true; }
    void reset() override {}

    geode::SettingNodeV3* createNode(float width) override;
};

class UnlinkSettingNode : public geode::SettingNodeV3 {
public:
    static UnlinkSettingNode* create(std::shared_ptr<UnlinkSetting> setting, float width);

    bool hasUncommittedChanges() const override { return false; }
    bool hasNonDefaultValue() const override { return false; }
protected:
    geode::async::TaskHolder<geode::Result<>> m_unlinkTask;

    bool init(std::shared_ptr<UnlinkSetting> setting, float width);
    void onUnlink(CCObject*);
    arc::Future<geode::Result<>> unlinkAccount(int accountID, argon::AccountData accData);

    void onCommit() override {}
    void onResetToDefault() override {}
};