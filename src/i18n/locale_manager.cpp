#include "i18n/locale_manager.h"
#include <iostream>

namespace dreamlang::i18n {
LocaleManager& LocaleManager::getInstance() {
    static LocaleManager instance;
    return instance;
}

bool LocaleManager::initialize(const std::string& domain, const std::string& locale_dir) {
    catalog_ = std::make_unique<MessageCatalog>(domain, locale_dir);
    
    // 尝试从环境变量获取本地化设置
    const char* env_lang = std::getenv("LANG");
    const char* env_lc_all = std::getenv("LC_ALL");
    const char* env_lc_messages = std::getenv("LC_MESSAGES");
    
    std::string locale = "en_US";  // 默认语言环境
    
    // 按优先级选择语言环境
    if (env_lc_all && strlen(env_lc_all) > 0) {
        locale = env_lc_all;
    } else if (env_lc_messages && strlen(env_lc_messages) > 0) {
        locale = env_lc_messages;
    } else if (env_lang && strlen(env_lang) > 0) {
        locale = env_lang;
    }
    
    // 移除编码部分（例如 zh_CN.UTF-8 -> zh_CN）
    size_t dot_pos = locale.find('.');
    if (dot_pos != std::string::npos) {
        locale = locale.substr(0, dot_pos);
    }
    
    // 尝试设置语言环境
    if (!catalog_->setLocale(locale)) {
        // 如果设置失败，尝试英文
        catalog_->setLocale("en_US");
    }
    
    return true;
}

bool LocaleManager::setLocale(const std::string& locale) {
    if (!catalog_) {
        return false;
    }
    
    return catalog_->setLocale(locale);
}

std::string LocaleManager::gettext(const std::string& msgid) const {
    if (!catalog_) {
        return msgid;
    }
    
    return catalog_->getMessage(msgid);
}

std::string LocaleManager::getPluralText(const std::string& msgid, 
                                         const std::string& msgid_plural, 
                                         int n) const {
    if (!catalog_) {
        return (n == 1) ? msgid : msgid_plural;
    }
    
    return catalog_->getPluralMessage(msgid, msgid_plural, n);
}

} // namespace dreamlang::i18n
