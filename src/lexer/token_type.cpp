#include "lexer/token_type.h"

namespace dreamlang::lexer {

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENT: return "IDENT";
        case TokenType::NULL_LITERAL: return "NULL";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::BOOL_TRUE: return "BOOL_TRUE";
        case TokenType::BOOL_FALSE: return "BOOL_FALSE";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::SINGLE_COMMENT: return "SINGLE_COMMENT";
        case TokenType::MULTI_COMMENT: return "MULTI_COMMENT";
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULT: return "MULT";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::POWER: return "POWER";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::LOGICAL_AND: return "LOGICAL_AND";
        case TokenType::LOGICAL_OR: return "LOGICAL_OR";
        case TokenType::LOGICAL_NOT: return "LOGICAL_NOT";
        case TokenType::LINEBREAK: return "LINEBREAK";
        case TokenType::DOT: return "DOT";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::EOF_TOKEN: return "EOF";
        default: return "UNKNOWN";
    }
}

} // namespace dreamlang::lexer
