#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>

namespace dreamlang::config {

// 配置值类型，支持字符串、数字和布尔值
using ConfigValue = std::variant<std::string, int, double, bool>;

/**
 * 配置管理器，负责加载和管理配置文件
 */
class ConfigManager {
public:
    /**
     * 获取全局实例（单例模式）
     */
    static ConfigManager& getInstance();

    /**
     * 加载配置文件
     * @param config_path 配置文件路径
     * @return 是否成功加载
     */
    bool loadConfig(const std::string& config_path);

    /**
     * 从可执行文件路径加载默认配置
     * @param executable_path 可执行文件路径
     * @return 是否成功加载
     */
    bool loadDefaultConfig(const std::string& executable_path);

    /**
     * 获取字符串配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 配置值
     */
    std::string getString(const std::string& key, const std::string& default_value = "") const;

    /**
     * 获取整数配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 配置值
     */
    int getInt(const std::string& key, int default_value = 0) const;

    /**
     * 获取浮点数配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 配置值
     */
    double getDouble(const std::string& key, double default_value = 0.0) const;

    /**
     * 获取布尔配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 配置值
     */
    bool getBool(const std::string& key, bool default_value = false) const;

    /**
     * 设置配置值
     * @param key 配置键
     * @param value 配置值
     */
    void set(const std::string& key, const ConfigValue& value);

    /**
     * 检查配置项是否存在
     * @param key 配置键
     * @return 是否存在
     */
    bool hasKey(const std::string& key) const;

    /**
     * 保存配置到文件
     * @param config_path 配置文件路径
     * @return 是否成功保存
     */
    bool saveConfig(const std::string& config_path) const;

    /**
     * 创建默认配置文件
     * @param config_path 配置文件路径
     * @return 是否成功创建
     */
    static bool createDefaultConfig(const std::string& config_path);

    /**
     * 设置默认配置文件（验证并复制配置文件到默认位置）
     * @param source_config_path 源配置文件路径
     * @return 是否成功设置
     */
    bool setAsDefaultConfig(const std::string& source_config_path);

    /**
     * 获取默认配置文件目录
     * @param executable_path 可执行文件路径
     * @return 默认配置目录路径
     */
    static std::string getDefaultConfigDir(const std::string& executable_path);

    /**
     * 验证配置文件格式
     * @param config_path 配置文件路径
     * @return 是否格式正确
     */
    static bool validateConfig(const std::string& config_path);

    /**
     * 获取当前配置文件路径
     */
    const std::string& getConfigPath() const { return config_path_; }

    /**
     * 检查是否已加载配置
     */
    bool isLoaded() const { return loaded_; }

private:
    ConfigManager() = default;
    ~ConfigManager() = default;
    
    // 禁用拷贝构造和赋值
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    /**
     * 解析 JSON 配置文件
     * @param content 文件内容
     * @return 是否解析成功
     */
    bool parseJson(const std::string& content);

    /**
     * 生成 JSON 配置字符串
     * @return JSON 字符串
     */
    std::string generateJson() const;

    /**
     * 获取默认配置路径
     * @param executable_path 可执行文件路径
     * @return 默认配置路径
     */
    std::string getDefaultConfigPath(const std::string& executable_path) const;

    /**
     * 获取路径分隔符（跨平台兼容）
     * @return 路径分隔符
     */
    static char getPathSeparator();

    /**
     * 规范化路径（处理不同平台的路径分隔符）
     * @param path 原始路径
     * @return 规范化后的路径
     */
    static std::string normalizePath(const std::string& path);

    std::unordered_map<std::string, ConfigValue> config_data_;
    std::string config_path_;
    bool loaded_ = false;
};

} // namespace dreamlang::config
