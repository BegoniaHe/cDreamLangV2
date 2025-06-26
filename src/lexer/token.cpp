#include "lexer/token.h"
#include <sstream>

namespace dreamlang::lexer {

Token::Token(TokenType type, const std::string& value, int line, int column)
    : type_(type), value_(value), line_(line), column_(column) {
}

Token::Token(const Token& other)
    : type_(other.type_), value_(other.value_), line_(other.line_), column_(other.column_) {
}

Token::Token(Token&& other) noexcept
    : type_(other.type_), value_(std::move(other.value_)), line_(other.line_), column_(other.column_) {
}

Token& Token::operator=(const Token& other) {
    if (this != &other) {
        type_ = other.type_;
        value_ = other.value_;
        line_ = other.line_;
        column_ = other.column_;
    }
    return *this;
}

Token& Token::operator=(Token&& other) noexcept {
    if (this != &other) {
        type_ = other.type_;
        value_ = std::move(other.value_);
        line_ = other.line_;
        column_ = other.column_;
    }
    return *this;
}

bool Token::isOperator() const {
    switch (type_) {
        case TokenType::ASSIGN:
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULT:
        case TokenType::DIVIDE:
        case TokenType::MODULO:
        case TokenType::POWER:
        case TokenType::EQUAL:
        case TokenType::NOT_EQUAL:
        case TokenType::GREATER:
        case TokenType::LESS:
        case TokenType::GREATER_EQUAL:
        case TokenType::LESS_EQUAL:
        case TokenType::LOGICAL_AND:
        case TokenType::LOGICAL_OR:
        case TokenType::LOGICAL_NOT:
            return true;
        default:
            return false;
    }
}

bool Token::isLiteral() const {
    switch (type_) {
        case TokenType::NULL_LITERAL:
        case TokenType::NUMBER:
        case TokenType::BOOL_TRUE:
        case TokenType::BOOL_FALSE:
        case TokenType::STRING:
        case TokenType::CHAR:
            return true;
        default:
            return false;
    }
}

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token{type=" << tokenTypeToString(type_) 
        << ", value=\"" << value_ << "\""
        << ", line=" << line_ 
        << ", column=" << column_ << "}";
    return oss.str();
}

bool Token::operator==(const Token& other) const {
    return type_ == other.type_ && 
           value_ == other.value_ && 
           line_ == other.line_ && 
           column_ == other.column_;
}

bool Token::operator!=(const Token& other) const {
    return !(*this == other);
}

} // namespace dreamlang::lexer
