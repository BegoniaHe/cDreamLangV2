#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace dreamlang::i18n {

/**
 * 消息目录类，负责管理本地化消息
 * 类似于GNU gettext的功能
 */
class MessageCatalog {
public:
    /**
     * 构造函数
     * @param domain 消息域（通常是程序名）
     * @param locale_dir 本地化文件目录
     */
    MessageCatalog(const std::string& domain, const std::string& locale_dir);
    
    /**
     * 析构函数
     */
    ~MessageCatalog();

    /**
     * 设置当前语言环境
     * @param locale 语言环境代码，如 "zh_CN", "en_US"
     * @return 是否成功设置
     */
    bool setLocale(const std::string& locale);

    /**
     * 获取本地化消息
     * @param msgid 消息标识符
     * @return 本地化后的消息，如果未找到则返回原始消息
     */
    std::string getMessage(const std::string& msgid) const;

    /**
     * 获取复数形式的本地化消息
     * @param msgid 单数形式的消息标识符
     * @param msgid_plural 复数形式的消息标识符
     * @param n 数量
     * @return 根据数量选择的本地化消息
     */
    std::string getPluralMessage(const std::string& msgid, 
                                const std::string& msgid_plural, 
                                int n) const;

    /**
     * 获取当前语言环境
     * @return 当前语言环境代码
     */
    const std::string& getCurrentLocale() const { return current_locale_; }

private:
    std::string domain_;
    std::string locale_dir_;
    std::string current_locale_;
    std::unordered_map<std::string, std::string> messages_;
    
    /**
     * 加载指定语言环境的消息文件
     * @param locale 语言环境代码
     * @return 是否成功加载
     */
    bool loadMessages(const std::string& locale);
    
    /**
     * 解析MO文件格式
     * @param filename MO文件路径
     * @return 是否成功解析
     */
    bool parseMoFile(const std::string& filename);
};

} // namespace dreamlang::i18n
