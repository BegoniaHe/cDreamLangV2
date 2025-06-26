#pragma once

#include "token_type.h"
#include <string>

namespace dreamlang::lexer {

/**
 * Token类，表示词法分析的基本单元
 */
class Token {
public:
    /**
     * 构造函数
     * @param type Token类型
     * @param value Token值
     * @param line 行号
     * @param column 列号
     */
    Token(TokenType type, const std::string& value, int line, int column);

    /**
     * 拷贝构造函数
     */
    Token(const Token& other);

    /**
     * 移动构造函数
     */
    Token(Token&& other) noexcept;

    /**
     * 赋值操作符
     */
    Token& operator=(const Token& other);

    /**
     * 移动赋值操作符
     */
    Token& operator=(Token&& other) noexcept;

    /**
     * 析构函数
     */
    ~Token() = default;

    // Getter方法
    TokenType getType() const { return type_; }
    const std::string& getValue() const { return value_; }
    int getLine() const { return line_; }
    int getColumn() const { return column_; }

    /**
     * 检查是否是关键字
     */
    bool isKeyword() const { return type_ == TokenType::KEYWORD; }

    /**
     * 检查是否是操作符
     */
    bool isOperator() const;

    /**
     * 检查是否是字面量
     */
    bool isLiteral() const;

    /**
     * 获取Token的字符串表示
     */
    std::string toString() const;

    /**
     * 等于操作符
     */
    bool operator==(const Token& other) const;

    /**
     * 不等于操作符
     */
    bool operator!=(const Token& other) const;

private:
    TokenType type_;
    std::string value_;
    int line_;
    int column_;
};

} // namespace dreamlang::lexer
