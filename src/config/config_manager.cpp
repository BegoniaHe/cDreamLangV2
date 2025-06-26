#include "config/config_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <map>
#include <algorithm>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #include <windows.h>
    #define mkdir(path, mode) _mkdir(path)
    #define access(path, mode) _access(path, mode)
    #define F_OK 0
#else
    #include <unistd.h>
    #include <sys/stat.h>

#endif

namespace dreamlang::config {

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::loadConfig(const std::string& config_path) {
    std::ifstream file(config_path);
    if (!file.is_open()) {
        return false;
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    if (parseJson(content)) {
        config_path_ = config_path;
        loaded_ = true;
        return true;
    }

    return false;
}

bool ConfigManager::loadDefaultConfig(const std::string& executable_path) {
    // 首先尝试加载当前目录的默认配置
    char separator = getPathSeparator();
    std::string local_default = ".config" + std::string(1, separator) + "default.json";
    
    if (access(local_default.c_str(), F_OK) == 0) {
        return loadConfig(local_default);
    }
    
    // 如果本地默认配置不存在，尝试全局默认配置
    std::string global_default = getDefaultConfigPath(executable_path);
    
    // 如果全局默认配置也不存在，创建一个
    if (access(global_default.c_str(), F_OK) != 0) {
        // 确保目录存在
        std::string config_dir = global_default.substr(0, global_default.find_last_of(separator));
        if (access(config_dir.c_str(), F_OK) != 0) {
            mkdir(config_dir.c_str(), 0755);
        }
        
        if (!createDefaultConfig(global_default)) {
            return false;
        }
    }
    
    return loadConfig(global_default);
}

std::string ConfigManager::getString(const std::string& key, const std::string& default_value) const {
    auto it = config_data_.find(key);
    if (it != config_data_.end()) {
        if (auto* value = std::get_if<std::string>(&it->second)) {
            return *value;
        }
    }
    return default_value;
}

int ConfigManager::getInt(const std::string& key, int default_value) const {
    auto it = config_data_.find(key);
    if (it != config_data_.end()) {
        if (auto* value = std::get_if<int>(&it->second)) {
            return *value;
        }
    }
    return default_value;
}

double ConfigManager::getDouble(const std::string& key, double default_value) const {
    auto it = config_data_.find(key);
    if (it != config_data_.end()) {
        if (auto* value = std::get_if<double>(&it->second)) {
            return *value;
        }
    }
    return default_value;
}

bool ConfigManager::getBool(const std::string& key, bool default_value) const {
    auto it = config_data_.find(key);
    if (it != config_data_.end()) {
        if (auto* value = std::get_if<bool>(&it->second)) {
            return *value;
        }
    }
    return default_value;
}

void ConfigManager::set(const std::string& key, const ConfigValue& value) {
    config_data_[key] = value;
}

bool ConfigManager::hasKey(const std::string& key) const {
    return config_data_.find(key) != config_data_.end();
}

bool ConfigManager::saveConfig(const std::string& config_path) const {
    std::ofstream file(config_path);
    if (!file.is_open()) {
        return false;
    }
    
    file << generateJson();
    file.close();
    return true;
}

bool ConfigManager::createDefaultConfig(const std::string& config_path) {
    std::ofstream file(config_path);
    if (!file.is_open()) {
        return false;
    }
    
    // 创建默认配置
    file << "{\n";
    file << "  \"language\": {\n";
    file << "    \"default_locale\": \"en_US\",\n";
    file << "    \"fallback_locale\": \"en_US\",\n";
    file << "    \"auto_detect\": true\n";
    file << "  },\n";
    file << "  \"compiler\": {\n";
    file << "    \"optimization_level\": 2,\n";
    file << "    \"debug_info\": false,\n";
    file << "    \"warnings_as_errors\": false\n";
    file << "  },\n";
    file << "  \"output\": {\n";
    file << "    \"verbose\": false,\n";
    file << "    \"show_progress\": true,\n";
    file << "    \"colored_output\": true\n";
    file << "  }\n";
    file << "}\n";
    
    file.close();
    return true;
}

bool ConfigManager::parseJson(const std::string& content) {
    // 简单的 JSON 解析器（仅支持我们需要的格式）
    // 这是一个基础实现，实际项目中可能需要使用专业的 JSON 库
    
    config_data_.clear();
    
    // 移除注释和空白
    std::string cleaned_content;
    std::istringstream iss(content);
    std::string line;
    while (std::getline(iss, line)) {
        // 移除单行注释
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        cleaned_content += line + "\n";
    }
    
    // 解析键值对  
    std::regex key_value_regex("\"([^\"]+)\"\\s*:\\s*(\"([^\"]*)\"|([0-9]+\\.?[0-9]*)|true|false)");
    std::regex nested_regex("\"([^\"]+)\"\\s*:\\s*\\{");
    
    std::string current_section;
    std::sregex_iterator iter(cleaned_content.begin(), cleaned_content.end(), key_value_regex);
    std::sregex_iterator end;
    
    // 首先处理嵌套结构
    std::sregex_iterator section_iter(cleaned_content.begin(), cleaned_content.end(), nested_regex);
    std::sregex_iterator section_end;
    
    for (; section_iter != section_end; ++section_iter) {
        std::string section_name = (*section_iter)[1].str();
        
        // 找到这个section的内容
        size_t start_pos = section_iter->position() + section_iter->length();
        size_t brace_count = 1;
        size_t end_pos = start_pos;
        
        while (end_pos < cleaned_content.length() && brace_count > 0) {
            if (cleaned_content[end_pos] == '{') {
                brace_count++;
            } else if (cleaned_content[end_pos] == '}') {
                brace_count--;
            }
            end_pos++;
        }
        
        std::string section_content = cleaned_content.substr(start_pos, end_pos - start_pos - 1);
        
        // 解析section内的键值对
        std::sregex_iterator section_kv_iter(section_content.begin(), section_content.end(), key_value_regex);
        for (; section_kv_iter != section_end; ++section_kv_iter) {
            std::string key = section_name + "." + (*section_kv_iter)[1].str();
            std::string value_str = (*section_kv_iter)[2].str();
            
            // 判断值的类型
            if (value_str.front() == '"' && value_str.back() == '"') {
                // 字符串值
                config_data_[key] = value_str.substr(1, value_str.length() - 2);
            } else if (value_str == "true") {
                config_data_[key] = true;
            } else if (value_str == "false") {
                config_data_[key] = false;
            } else if (value_str.find('.') != std::string::npos) {
                // 浮点数
                try {
                    config_data_[key] = std::stod(value_str);
                } catch (...) {
                    return false;
                }
            } else {
                // 整数
                try {
                    config_data_[key] = std::stoi(value_str);
                } catch (...) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

std::string ConfigManager::generateJson() const {
    std::ostringstream oss;
    oss << "{\n";
    
    // 按节分组配置项
    std::map<std::string, std::map<std::string, ConfigValue>> sections;
    
    for (const auto& [key, value] : config_data_) {
        size_t dot_pos = key.find('.');
        if (dot_pos != std::string::npos) {
            std::string section = key.substr(0, dot_pos);
            std::string sub_key = key.substr(dot_pos + 1);
            sections[section][sub_key] = value;
        }
    }
    
    bool first_section = true;
    for (const auto& [section_name, section_data] : sections) {
        if (!first_section) oss << ",\n";
        first_section = false;
        
        oss << "  \"" << section_name << "\": {\n";
        bool first_item = true;
        for (const auto& [sub_key, value] : section_data) {
            if (!first_item) oss << ",\n";
            first_item = false;
            
            oss << "    \"" << sub_key << "\": ";
            std::visit([&oss](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    oss << "\"" << v << "\"";
                } else if constexpr (std::is_same_v<T, bool>) {
                    oss << (v ? "true" : "false");
                } else {
                    oss << v;
                }
            }, value);
        }
        oss << "\n  }";
    }
    
    oss << "\n}\n";
    return oss.str();
}

std::string ConfigManager::getDefaultConfigPath(const std::string& executable_path) const {
    char separator = getPathSeparator();
    std::string normalized_path = normalizePath(executable_path);
    
    size_t last_separator = normalized_path.find_last_of(separator);
    std::string bin_dir = (last_separator != std::string::npos) ? 
                         normalized_path.substr(0, last_separator) : ".";
    
    // 配置文件在可执行文件同目录的.config子目录
    return bin_dir + separator + ".config" + separator + "default.json";
}

char ConfigManager::getPathSeparator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

std::string ConfigManager::normalizePath(const std::string& path) {
    std::string normalized = path;

    // 将所有路径分隔符统一为当前系统的分隔符
#ifdef _WIN32
    std::replace(normalized.begin(), normalized.end(), '/', '\\');
#else
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
#endif
    
    return normalized;
}

bool ConfigManager::setAsDefaultConfig(const std::string& config_path) {
    // 验证配置文件是否有效
    if (!loadConfig(config_path)) {
        return false;
    }
    
    // 获取默认配置文件路径
    // 这里需要从当前工作目录开始，而不是从可执行文件路径
    char separator = getPathSeparator();
    std::string default_config_dir = ".config";
    std::string default_config_path = default_config_dir + separator + "default.json";
    
    // 创建.config目录（如果不存在）
    if (access(default_config_dir.c_str(), F_OK) != 0) {
        if (mkdir(default_config_dir.c_str(), 0755) != 0) {
            return false;
        }
    }
    
    // 复制配置文件到默认位置
    std::ifstream src(config_path, std::ios::binary);
    if (!src.is_open()) {
        return false;
    }
    
    std::ofstream dst(default_config_path, std::ios::binary);
    if (!dst.is_open()) {
        src.close();
        return false;
    }
    
    dst << src.rdbuf();
    src.close();
    dst.close();
    
    // 重新加载配置
    return loadConfig(default_config_path);
}

std::string ConfigManager::getDefaultConfigDir(const std::string& executable_path) {
    size_t last_slash = executable_path.find_last_of("/\\");
    std::string bin_dir = (last_slash != std::string::npos) ? 
                         executable_path.substr(0, last_slash) : ".";
    
    // 用户默认配置目录在可执行文件目录下的 .config
    return bin_dir + "/.config";
}

bool ConfigManager::validateConfig(const std::string& config_path) {
    std::ifstream file(config_path);
    if (!file.is_open()) {
        return false;
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    // 尝试解析JSON来验证格式
    ConfigManager temp_manager;
    return temp_manager.parseJson(content);
}

} // namespace dreamlang::config
