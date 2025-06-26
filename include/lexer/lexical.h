#pragma once

#include "token.h"
#include "lexical_exception.h"
#include <string>
#include <vector>
#include <unordered_set>

namespace dreamlang::lexer {

/**
 * 词法分析器类
 */
class Lexical {
public:
    /**
     * 构造函数
     * @param source_code 源代码字符串
     */
    explicit Lexical(std::string source_code);

    /**
     * 析构函数
     */
    ~Lexical() = default;

    /**
     * 获取下一个Token
     * @return 下一个Token，如果到达文件末尾则返回EOF Token
     */
    Token nextToken();

    /**
     * 获取所有Token
     * @return Token列表
     */
    std::vector<Token> tokenize();

    /**
     * 重置词法分析器到起始位置
     */
    void reset();

    /**
     * 获取当前行号
     */
    [[nodiscard]] int getCurrentLine() const { return line_; }

    /**
     * 获取当前列号
     */
    [[nodiscard]] int getCurrentColumn() const { return column_; }

    /**
     * 检查是否到达文件末尾
     */
    [[nodiscard]] bool isAtEnd() const { return index_ >= source_code_.length(); }

private:
    std::string source_code_;
    size_t index_;
    int line_;
    int column_;

    // 静态查找表
    static std::unordered_set<std::string> keywords_;
    static bool initialized_;

    /**
     * 初始化静态数据
     */
    static void initializeStatic();

    /**
     * 获取当前字符
     */
    [[nodiscard]] char currentChar() const;

    /**
     * 获取下一个字符（不移动位置）
     */
    [[nodiscard]] char peekChar(size_t offset = 1) const;

    /**
     * 前进一个字符
     */
    void advance();

    /**
     * 跳过空白字符
     */
    void skipWhitespace();

    /**
     * 跳过单行注释
     */
    void skipSingleLineComment();

    /**
     * 跳过多行注释
     */
    void skipMultiLineComment();

    /**
     * 读取标识符或关键字
     */
    Token readIdentifierOrKeyword();

    /**
     * 读取数字
     */
    Token readNumber();

    /**
     * 读取字符串字面量
     */
    Token readString();

    /**
     * 读取字符字面量
     */
    Token readChar();

    /**
     * 处理转义字符
     */
    char processEscapeSequence();

    /**
     * 检查字符是否为数字
     */
    static bool isDigit(char c);

    /**
     * 检查字符是否为十六进制数字
     */
    static bool isHexDigit(char c);

    /**
     * 检查字符是否为字母或下划线
     */
    static bool isAlpha(char c);

    /**
     * 检查字符是否为字母数字或下划线
     */
    static bool isAlphaNumeric(char c);

    /**
     * 检查字符串是否为关键字
     */
    static bool isKeyword(const std::string& text);

    /**
     * 创建Token
     */
    [[nodiscard]] Token makeToken(TokenType type, const std::string& value = "") const;

    /**
     * 抛出词法错误
     */
    void throwError(const std::string& error_type, char error_char, 
                   const std::string& token_type) const;
};

} // namespace dreamlang::lexer
