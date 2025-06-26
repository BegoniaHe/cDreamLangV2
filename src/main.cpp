#include "lexer/lexical.h"
#include "lexer/lexical_exception.h"
#include "i18n/locale_manager.h"
#include "config/config_manager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

void printUsage(const char* program_name) {
    using namespace dreamlang::i18n;
    auto& locale_mgr = LocaleManager::getInstance();
    
    std::cout << locale_mgr.gettext("Usage") << ": " << program_name 
              << " [" << locale_mgr.gettext("options") << "] [<" 
              << locale_mgr.gettext("source_file") << ".zv>]" << std::endl;
    std::cout << std::endl;
    std::cout << locale_mgr.gettext("Options") << ":" << std::endl;
    std::cout << "  -h, --help     " << locale_mgr.gettext("Show this help message") << std::endl;
    std::cout << "  -v, --version  " << locale_mgr.gettext("Show version information") << std::endl;
    std::cout << "  -l, --locale   " << locale_mgr.gettext("Set locale (e.g., zh_CN, en_US)") << std::endl;
    std::cout << "  -t, --tokens   " << locale_mgr.gettext("Show tokenization result") << std::endl;
    std::cout << "  -c, --config   " << locale_mgr.gettext("Set default config or specify config file") << std::endl;
    std::cout << std::endl;
    std::cout << locale_mgr.gettext("Note") << ": " 
              << locale_mgr.gettext("If source file has no extension, .zv will be automatically appended.") << std::endl;
}

void printVersion() {
    using namespace dreamlang::i18n;
    auto& locale_mgr = LocaleManager::getInstance();
    
    std::cout << "DreamLang " << locale_mgr.gettext("Compiler") << " v2.0.0" << std::endl;
    std::cout << locale_mgr.gettext("Copyright") << " (C) 2025 DreamLang " 
              << locale_mgr.gettext("Project") << std::endl;
}

std::string resolveSourceFile(const std::string& filename) {
    using namespace dreamlang::i18n;
    // auto& locale_mgr = LocaleManager::getInstance();
    
    // 如果文件名已经有扩展名，直接使用
    if (filename.find('.') != std::string::npos) {
        return filename;
    }
    
    // 尝试添加 .zv 后缀
    std::string zv_filename = filename + ".zv";
    std::ifstream test_file(zv_filename);
    if (test_file.good()) {
        test_file.close();
        return zv_filename;
    }
    
    // 如果找不到 .zv 文件，返回原文件名（让后续错误处理处理）
    return filename;
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        using namespace dreamlang::i18n;
        auto& locale_mgr = LocaleManager::getInstance();
        throw std::runtime_error(locale_mgr.gettext("Cannot open file") + ": " + filename);
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    return content;
}

void tokenizeAndPrint(const std::string& source_code, bool show_tokens = false) {
    using namespace dreamlang::lexer;
    using namespace dreamlang::i18n;
    
    auto& locale_mgr = LocaleManager::getInstance();
    
    try {
        Lexical lexer(source_code);
        std::vector<Token> tokens = lexer.tokenize();
        
        if (show_tokens) {
            std::cout << locale_mgr.gettext("Tokenization result") << ":" << std::endl;
            std::cout << "===========================================" << std::endl;
            
            for (const auto& token : tokens) {
                if (token.getType() != TokenType::LINEBREAK) {
                    std::cout << token.toString() << std::endl;
                }
            }
            
            std::cout << "===========================================" << std::endl;
            std::cout << locale_mgr.gettext("Total tokens") << ": " << tokens.size() << std::endl;
        } else {
            std::cout << locale_mgr.gettext("Lexical analysis completed successfully") 
                      << ". " << locale_mgr.gettext("Found") << " " << tokens.size() 
                      << " " << locale_mgr.gettext("tokens") << "." << std::endl;
        }
        
    } catch (const LexicalException& e) {
        std::cerr << locale_mgr.gettext("Lexical Error") << ": " 
                  << e.getLocalizedMessage() << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    using namespace dreamlang::i18n;
    using namespace dreamlang::config;
    
    // 首先加载配置文件
    auto& config_mgr = ConfigManager::getInstance();
    
    // 获取可执行文件的目录
    std::string program_path = argv[0];
    
#ifdef _WIN32
    // Windows路径处理
    std::replace(program_path.begin(), program_path.end(), '/', '\\');
    size_t last_separator = program_path.find_last_of('\\');
#else
    // Unix/Linux路径处理
    std::replace(program_path.begin(), program_path.end(), '\\', '/');
    size_t last_separator = program_path.find_last_of('/');
#endif
    
    std::string bin_dir = (last_separator != std::string::npos) ? 
                         program_path.substr(0, last_separator) : ".";
    
    // 加载默认配置
    if (!config_mgr.loadDefaultConfig(program_path)) {
        std::cerr << "Warning: Failed to load configuration file" << std::endl;
    }
    
    // 初始化国际化系统
    auto& locale_mgr = LocaleManager::getInstance();
    
    // 假设locale文件在 ../share/locale 相对于可执行文件
#ifdef _WIN32
    std::string locale_dir = bin_dir + "\\..\\share\\locale";
#else
    std::string locale_dir = bin_dir + "/../share/locale";
#endif
    
    if (!locale_mgr.initialize("dreamlang", locale_dir)) {
        std::cerr << "Warning: Failed to initialize localization system" << std::endl;
    }
    
    // 从配置文件设置默认locale
    if (config_mgr.isLoaded()) {
        std::string default_locale = config_mgr.getString("language.default_locale", "en_US");
        if (!locale_mgr.setLocale(default_locale)) {
            // 尝试fallback locale
            std::string fallback_locale = config_mgr.getString("language.fallback_locale", "en_US");
            if (!locale_mgr.setLocale(fallback_locale)) {
                std::cerr << "Warning: Failed to set default locale from config" << std::endl;
            }
        }
    }
    
    // 解析命令行参数
    std::string source_file;
    std::string custom_locale;
    std::string custom_config;
    bool show_help = false;
    bool show_version = false;
    bool show_tokens = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            show_help = true;
        } else if (arg == "-v" || arg == "--version") {
            show_version = true;
        } else if (arg == "-t" || arg == "--tokens") {
            show_tokens = true;
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                custom_config = argv[++i];
            } else {
                std::cerr << locale_mgr.gettext("Error") << ": " 
                          << locale_mgr.gettext("Option --config requires an argument") << std::endl;
                return 1;
            }
        } else if (arg == "-l" || arg == "--locale") {
            if (i + 1 < argc) {
                custom_locale = argv[++i];
            } else {
                std::cerr << locale_mgr.gettext("Error") << ": " 
                          << locale_mgr.gettext("Option --locale requires an argument") << std::endl;
                return 1;
            }
        } else if (arg[0] == '-') {
            std::cerr << locale_mgr.gettext("Error") << ": " 
                      << locale_mgr.gettext("Unknown option") << " '" << arg << "'" << std::endl;
            printUsage(argv[0]);
            return 1;
        } else {
            if (source_file.empty()) {
                source_file = arg;
            } else {
                std::cerr << locale_mgr.gettext("Error") << ": " 
                          << locale_mgr.gettext("Multiple source files specified") << std::endl;
                return 1;
            }
        }
    }
    
    // 如果指定了自定义配置文件，重新加载配置
    if (!custom_config.empty()) {
        // 检查是否只指定了配置文件而没有源文件（设置默认配置模式）
        if (source_file.empty() && !show_help && !show_version) {
            // 设置默认配置模式
            if (config_mgr.setAsDefaultConfig(custom_config)) {
                std::cout << locale_mgr.gettext("Default config set successfully") << ": " 
                          << custom_config << std::endl;
                return 0;
            } else {
                std::cerr << locale_mgr.gettext("Error") << ": " 
                          << locale_mgr.gettext("Failed to set default config") << " '" 
                          << custom_config << "'" << std::endl;
                return 1;
            }
        }
        
        // 正常的临时配置文件使用模式
        if (!config_mgr.loadConfig(custom_config)) {
            std::cerr << locale_mgr.gettext("Warning") << ": " 
                      << locale_mgr.gettext("Failed to load config file") << " '" 
                      << custom_config << "'" << std::endl;
        } else {
            // 重新应用配置中的locale设置
            std::string config_locale = config_mgr.getString("language.default_locale", "en_US");
            if (!locale_mgr.setLocale(config_locale)) {
                std::string fallback_locale = config_mgr.getString("language.fallback_locale", "en_US");
                locale_mgr.setLocale(fallback_locale);
            }
        }
    }
    
    // 设置自定义locale（命令行参数优先级最高）
    if (!custom_locale.empty()) {
        if (!locale_mgr.setLocale(custom_locale)) {
            std::cerr << locale_mgr.gettext("Warning") << ": " 
                      << locale_mgr.gettext("Failed to set locale") << " '" 
                      << custom_locale << "'" << std::endl;
        }
    }
    
    if (show_help) {
        printUsage(argv[0]);
        return 0;
    }
    
    if (show_version) {
        printVersion();
        return 0;
    }
    
    if (source_file.empty()) {
        std::cerr << locale_mgr.gettext("Error") << ": " 
                  << locale_mgr.gettext("No source file specified") << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        std::string resolved_file = resolveSourceFile(source_file);
        std::string source_code = readFile(resolved_file);
        tokenizeAndPrint(source_code, show_tokens);
    } catch (const std::exception& e) {
        std::cerr << locale_mgr.gettext("Error") << ": " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
