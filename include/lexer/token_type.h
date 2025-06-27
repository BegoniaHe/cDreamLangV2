#pragma once

namespace dreamlang::lexer {

/**
 * Token类型枚举
 */
enum class TokenType {
    // 非法Token
    ILLEGAL,
    // 标识符
    IDENT,
    // 空值
    NULL_LITERAL,
    // 数字
    NUMBER,
    // 布尔值：true
    BOOL_TRUE,
    // 布尔值：false
    BOOL_FALSE,
    // 字符串
    STRING,
    // 字符
    CHAR,
    // 单行注释
    SINGLE_COMMENT,
    // 多行注释
    MULTI_COMMENT,
    // 关键字
    KEYWORD,
    // 赋值
    ASSIGN,
    // 加
    PLUS,
    // 减
    MINUS,
    // 乘
    MULT,
    // 除
    DIVIDE,
    // 取余
    MODULO,
    // 次方
    POWER,
    // 等于
    EQUAL,
    // 不等于
    NOT_EQUAL,
    // 大于
    GREATER,
    // 小于
    LESS,
    // 大于等于
    GREATER_EQUAL,
    // 小于等于
    LESS_EQUAL,
    // 逻辑与
    LOGICAL_AND,
    // 逻辑或
    LOGICAL_OR,
    // 逻辑非
    LOGICAL_NOT,
    // 换行
    LINEBREAK,
    // 点
    DOT,
    // 逗号
    COMMA,
    // 冒号
    COLON,
    // 分号
    SEMICOLON,
    // 左小括号
    LEFT_PAREN,
    // 右小括号
    RIGHT_PAREN,
    // 左中括号
    LEFT_BRACKET,
    // 右中括号
    RIGHT_BRACKET,
    // 左大括号
    LEFT_BRACE,
    // 右大括号
    RIGHT_BRACE,
    // 文件结束
    EOF_TOKEN
};

/**
 * 将TokenType转换为字符串表示
 * @param type Token类型
 * @return 字符串表示
 */
const char* tokenTypeToString(TokenType type);

} // namespace dreamlang::lexer
