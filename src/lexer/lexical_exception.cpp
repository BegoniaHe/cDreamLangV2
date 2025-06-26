#include "lexer/lexical_exception.h"
#include "i18n/locale_manager.h"
#include <sstream>

namespace dreamlang:: lexer {

LexicalException::LexicalException(const std::string& error_type,
                                 char error_char,
                                 const std::string& error_token_type,
                                 int line,
                                 int column)
    : std::runtime_error(generateMessage()),
      error_type_(error_type),
      error_char_(error_char),
      error_token_type_(error_token_type),
      line_(line),
      column_(column) {
}

LexicalException::LexicalException(const std::string& message, int line, int column)
    : std::runtime_error(message),
      error_type_(message),
      error_char_('\0'),
      error_token_type_(""),
      line_(line),
      column_(column) {
}

std::string LexicalException::generateMessage() const {
    std::ostringstream oss;
    
    if (column_ >= 0) {
        oss << error_type_ << " at line " << line_ << ", column " << column_;
    } else {
        oss << error_type_ << " at line " << line_;
    }
    
    if (error_char_ != '\0') {
        oss << ": unexpected character '" << error_char_ << "'";
    }
    
    if (!error_token_type_.empty()) {
        oss << " (token type: " << error_token_type_ << ")";
    }
    
    return oss.str();
}

std::string LexicalException::getLocalizedMessage() const {
    using namespace dreamlang::i18n;
    
    auto& locale_mgr = LocaleManager::getInstance();
    
    if (!locale_mgr.isInitialized()) {
        return what();
    }
    
    std::ostringstream oss;
    
    if (column_ >= 0) {
        // 使用本地化的错误消息格式
        std::string format = locale_mgr.gettext("Lexical error at line %d, column %d");
        char buffer[256];
        snprintf(buffer, sizeof(buffer), format.c_str(), line_, column_);
        oss << buffer;
    } else {
        std::string format = locale_mgr.gettext("Lexical error at line %d");
        char buffer[256];
        snprintf(buffer, sizeof(buffer), format.c_str(), line_);
        oss << buffer;
    }
    
    if (error_char_ != '\0') {
        std::string unexpected_msg = locale_mgr.gettext("unexpected character");
        oss << ": " << unexpected_msg << " '" << error_char_ << "'";
    }
    
    if (!error_token_type_.empty()) {
        std::string token_type_msg = locale_mgr.gettext("token type");
        oss << " (" << token_type_msg << ": " << error_token_type_ << ")";
    }
    
    // 添加具体的错误类型翻译
    if (!error_type_.empty()) {
        std::string localized_error_type = locale_mgr.gettext(error_type_);
        if (localized_error_type != error_type_) {
            oss << " - " << localized_error_type;
        }
    }
    
    return oss.str();
}

} // namespace dreamlang::lexer
