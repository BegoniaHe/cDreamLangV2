#pragma once

#include <stdexcept>
#include <string>

namespace dreamlang::lexer {

/**
 * 词法分析异常类
 */
class LexicalException : public std::runtime_error {
public:
    /**
     * 构造函数
     * @param error_type 错误类型
     * @param error_char 引起错误的字符
     * @param error_token_type 错误的Token类型
     * @param line 错误行号
     * @param column 错误列号
     */
    LexicalException(const std::string& error_type,
                    char error_char,
                    const std::string& error_token_type,
                    int line,
                    int column = -1);

    /**
     * 构造函数（用于一般错误消息）
     * @param message 错误消息
     * @param line 错误行号
     * @param column 错误列号
     */
    LexicalException(const std::string& message, int line, int column = -1);

    /**
     * 获取错误类型
     */
    const std::string& getErrorType() const { return error_type_; }

    /**
     * 获取引起错误的字符
     */
    char getErrorChar() const { return error_char_; }

    /**
     * 获取错误的Token类型
     */
    const std::string& getErrorTokenType() const { return error_token_type_; }

    /**
     * 获取错误行号
     */
    int getLine() const { return line_; }

    /**
     * 获取错误列号
     */
    int getColumn() const { return column_; }

    /**
     * 获取完整的本地化错误消息
     */
    std::string getLocalizedMessage() const;

private:
    std::string error_type_;
    char error_char_;
    std::string error_token_type_;
    int line_;
    int column_;

    /**
     * 生成错误消息
     */
    std::string generateMessage() const;
};

} // namespace dreamlang::lexer
