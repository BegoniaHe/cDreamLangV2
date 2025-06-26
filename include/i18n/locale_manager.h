#pragma once

#include "message_catalog.h"
#include <memory>

namespace dreamlang::i18n {

/**
 * 本地化管理器，全局管理本地化设置
 */
class LocaleManager {
public:
    /**
     * 获取全局实例（单例模式）
     */
    static LocaleManager& getInstance();

    /**
     * 初始化本地化系统
     * @param domain 消息域
     * @param locale_dir 本地化文件目录
     * @return 是否成功初始化
     */
    bool initialize(const std::string& domain, const std::string& locale_dir);

    /**
     * 设置语言环境
     * @param locale 语言环境代码
     * @return 是否成功设置
     */
    bool setLocale(const std::string& locale);

    /**
     * 获取本地化消息
     * @param msgid 消息标识符
     * @return 本地化后的消息
     */
    std::string gettext(const std::string& msgid) const;

    /**
     * 获取复数形式的本地化消息
     * @param msgid 单数形式的消息标识符
     * @param msgid_plural 复数形式的消息标识符
     * @param n 数量
     * @return 根据数量选择的本地化消息
     */
    std::string getPluralText(const std::string& msgid, 
                             const std::string& msgid_plural, 
                             int n) const;

    /**
     * 检查是否已初始化
     */
    bool isInitialized() const { return catalog_ != nullptr; }

private:
    LocaleManager() = default;
    ~LocaleManager() = default;
    
    // 禁用拷贝构造和赋值
    LocaleManager(const LocaleManager&) = delete;
    LocaleManager& operator=(const LocaleManager&) = delete;

    std::unique_ptr<MessageCatalog> catalog_;
};

} // namespace dreamlang::i18n
// 便利宏定义，类似于GNU gettext
#define _(msgid) dreamlang::i18n::LocaleManager::getInstance().gettext(msgid)
#define N_(msgid) (msgid)  // 用于标记但不翻译的字符串
#define ngettext(msgid, msgid_plural, n) \
    dreamlang::i18n::LocaleManager::getInstance().getPluralText(msgid, msgid_plural, n)
