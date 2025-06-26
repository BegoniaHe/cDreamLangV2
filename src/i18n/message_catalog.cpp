#include "i18n/message_catalog.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>

namespace dreamlang::i18n {

MessageCatalog::MessageCatalog(const std::string& domain, const std::string& locale_dir)
    : domain_(domain), locale_dir_(locale_dir), current_locale_("C") {
}

MessageCatalog::~MessageCatalog() = default;

bool MessageCatalog::setLocale(const std::string& locale) {
    if (locale == current_locale_) {
        return true;
    }

    if (loadMessages(locale)) {
        current_locale_ = locale;
        return true;
    }
    
    return false;
}

std::string MessageCatalog::getMessage(const std::string& msgid) const {
    auto it = messages_.find(msgid);
    if (it != messages_.end()) {
        return it->second;
    }
    
    // 如果未找到翻译，返回原始消息
    return msgid;
}

std::string MessageCatalog::getPluralMessage(const std::string& msgid, 
                                           const std::string& msgid_plural, 
                                           int n) const {
    // 简化的复数处理：对于中文，通常不区分单复数
    // 对于英文，n == 1时使用单数，否则使用复数
    if (current_locale_.find("zh") == 0) {
        // 中文语言环境
        return getMessage(msgid);
    } else {
        // 英文或其他语言环境
        if (n == 1) {
            return getMessage(msgid);
        } else {
            std::string plural_key = msgid + "\x00" + msgid_plural;
            auto it = messages_.find(plural_key);
            if (it != messages_.end()) {
                return it->second;
            }
            return getMessage(msgid_plural);
        }
    }
}

bool MessageCatalog::loadMessages(const std::string& locale) {
    messages_.clear();
    
    // 构建MO文件路径
    std::string mo_file = locale_dir_ + "/" + locale + "/LC_MESSAGES/" + domain_ + ".mo";
    
    // 尝试解析MO文件
    if (parseMoFile(mo_file)) {
        return true;
    }
    
    // 如果失败，尝试简化的locale（例如 zh_CN -> zh）
    size_t underscore_pos = locale.find('_');
    if (underscore_pos != std::string::npos) {
        std::string simplified_locale = locale.substr(0, underscore_pos);
        mo_file = locale_dir_ + "/" + simplified_locale + "/LC_MESSAGES/" + domain_ + ".mo";
        if (parseMoFile(mo_file)) {
            return true;
        }
    }
    
    return false;
}

bool MessageCatalog::parseMoFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // 读取MO文件头部
    uint32_t magic;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    
    // 检查魔数 (0x950412de 或 0xde120495)
    bool little_endian = true;
    if (magic == 0x950412de) {
        little_endian = true;
    } else if (magic == 0xde120495) {
        little_endian = false;
    } else {
        return false;
    }

    auto readUint32 = [&file, little_endian]() -> uint32_t {
        uint32_t value;
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (!little_endian) {
            // 转换字节序（简化实现）
            value = ((value & 0xFF) << 24) | 
                   (((value >> 8) & 0xFF) << 16) | 
                   (((value >> 16) & 0xFF) << 8) | 
                   ((value >> 24) & 0xFF);
        }
        return value;
    };

    uint32_t revision = readUint32();
    uint32_t num_strings = readUint32();
    uint32_t original_table_offset = readUint32();
    uint32_t translation_table_offset = readUint32();

    if (revision != 0) {
        return false;
    }

    // 读取原文字符串偏移表
    std::vector<std::pair<uint32_t, uint32_t>> original_strings(num_strings);
    file.seekg(original_table_offset);
    for (uint32_t i = 0; i < num_strings; ++i) {
        original_strings[i].first = readUint32();  // length
        original_strings[i].second = readUint32(); // offset
    }

    // 读取翻译字符串偏移表
    std::vector<std::pair<uint32_t, uint32_t>> translation_strings(num_strings);
    file.seekg(translation_table_offset);
    for (uint32_t i = 0; i < num_strings; ++i) {
        translation_strings[i].first = readUint32();  // length
        translation_strings[i].second = readUint32(); // offset
    }

    // 读取字符串并建立映射
    for (uint32_t i = 0; i < num_strings; ++i) {
        // 读取原文
        file.seekg(original_strings[i].second);
        std::string original(original_strings[i].first, '\0');
        file.read(&original[0], original_strings[i].first);

        // 读取翻译
        file.seekg(translation_strings[i].second);
        std::string translation(translation_strings[i].first, '\0');
        file.read(&translation[0], translation_strings[i].first);

        // 添加到消息映射
        if (!original.empty()) {
            messages_[original] = translation;
        }
    }

    return true;
}

} // namespace dreamlang::i18n
