#include "lexer/lexical.h"
#include "i18n/locale_manager.h"


namespace dreamlang::lexer {

// 静态成员初始化
std::unordered_set<std::string> Lexical::keywords_;
bool Lexical::initialized_ = false;

Lexical::Lexical(std::string source_code)
    : source_code_(std::move(source_code)), index_(0), line_(1), column_(1) {
    if (!initialized_) {
        initializeStatic();
        initialized_ = true;
    }
}

void Lexical::initializeStatic() {
    keywords_ = {
        "bool", "number", "char", "string", "function", "array", "class", "object",
        "reference", "package", "import", "var", "val", "ref", "return", "fun",
        "if", "else", "for", "while", "break", "continue", "switch", "case",
        "default", "super", "this", "available", "in", "interface", "abstract",
        "null", "true", "false"
    };
}

Token Lexical::nextToken() {
    while (true) {
        skipWhitespace();

        if (isAtEnd()) {
            return makeToken(TokenType::EOF_TOKEN);
        }

        char c = currentChar();

        // 处理换行符
        if (c == '\n') {
            advance();
            return makeToken(TokenType::LINEBREAK, "\n");
        }

        // 处理注释
        if (c == '/' && peekChar() == '/') {
            skipSingleLineComment();
            continue; // 继续循环而不是递归调用
        }

        if (c == '/' && peekChar() == '*') {
            skipMultiLineComment();
            continue; // 继续循环而不是递归调用
        }

        // 处理字符串字面量
        if (c == '"') {
            return readString();
        }

        // 处理字符字面量
        if (c == '\'') {
            return readChar();
        }

        // 处理数字
        if (isDigit(c)) {
            return readNumber();
        }

        // 处理标识符和关键字
        if (isAlpha(c)) {
            return readIdentifierOrKeyword();
        }

        // 处理操作符和分隔符
        switch (c) {
            case '=':
                advance();
                if (currentChar() == '=' && !isAtEnd()) {
                    advance();
                    return makeToken(TokenType::EQUAL, "==");
                }
                return makeToken(TokenType::ASSIGN, "=");

            case '!':
                advance();
                if (currentChar() == '=' && !isAtEnd()) {
                    advance();
                    return makeToken(TokenType::NOT_EQUAL, "!=");
                }
                return makeToken(TokenType::LOGICAL_NOT, "!");

            case '<':
                advance();
                if (currentChar() == '=' && !isAtEnd()) {
                    advance();
                    return makeToken(TokenType::LESS_EQUAL, "<=");
                }
                return makeToken(TokenType::LESS, "<");

            case '>':
                advance();
                if (currentChar() == '=' && !isAtEnd()) {
                    advance();
                    return makeToken(TokenType::GREATER_EQUAL, ">=");
                }
                return makeToken(TokenType::GREATER, ">");

            case '&':
                advance();
                if (currentChar() == '&' && !isAtEnd()) {
                    advance();
                    return makeToken(TokenType::LOGICAL_AND, "&&");
                }
                throwError(_("Invalid character"), c, "UNKNOWN");
                break;

            case '|':
                advance();
                if (currentChar() == '|' && !isAtEnd()) {
                    advance();
                    return makeToken(TokenType::LOGICAL_OR, "||");
                }
                throwError(_("Invalid character"), c, "UNKNOWN");
                break;

            case '+':
                advance();
                return makeToken(TokenType::PLUS, "+");

            case '-':
                advance();
                return makeToken(TokenType::MINUS, "-");

            case '*':
                advance();
                if (currentChar() == '*' && !isAtEnd()) {
                    advance();
                    return makeToken(TokenType::POWER, "**");
                }
                return makeToken(TokenType::MULT, "*");

            case '/':
                advance();
                return makeToken(TokenType::DIVIDE, "/");

            case '%':
                advance();
                return makeToken(TokenType::MODULO, "%");

            case '.':
                advance();
                return makeToken(TokenType::DOT, ".");

            case ',':
                advance();
                return makeToken(TokenType::COMMA, ",");

            case ':':
                advance();
                return makeToken(TokenType::COLON, ":");

            case ';':
                advance();
                return makeToken(TokenType::SEMICOLON, ";");

            case '(':
                advance();
                return makeToken(TokenType::LEFT_PAREN, "(");

            case ')':
                advance();
                return makeToken(TokenType::RIGHT_PAREN, ")");

            case '[':
                advance();
                return makeToken(TokenType::LEFT_BRACKET, "[");

            case ']':
                advance();
                return makeToken(TokenType::RIGHT_BRACKET, "]");

            case '{':
                advance();
                return makeToken(TokenType::LEFT_BRACE, "{");

            case '}':
                advance();
                return makeToken(TokenType::RIGHT_BRACE, "}");

            default:
                throwError(_("Unexpected character"), c, "UNKNOWN");
        }
    }
}

std::vector<Token> Lexical::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        Token token = nextToken();
        if (token.getType() != TokenType::EOF_TOKEN) {
            tokens.push_back(token);
        } else {
            break;
        }
    }
    
    tokens.push_back(makeToken(TokenType::EOF_TOKEN));
    return tokens;
}

void Lexical::reset() {
    index_ = 0;
    line_ = 1;
    column_ = 1;
}

char Lexical::currentChar() const {
    if (isAtEnd()) {
        return '\0';
    }
    return source_code_[index_];
}

char Lexical::peekChar(size_t offset) const {
    size_t peek_index = index_ + offset;
    if (peek_index >= source_code_.length()) {
        return '\0';
    }
    return source_code_[peek_index];
}

void Lexical::advance() {
    if (!isAtEnd()) {
        if (source_code_[index_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        index_++;
    }
}
void Lexical::skipWhitespace() {
    while (!isAtEnd()) {
        char c = currentChar();
        if (c == ' ' || c == '\r' || c == '\t') {
            advance();
        } else {
            break;
        }
    }
}

void Lexical::skipSingleLineComment() {
    // 跳过 //
    advance();
    advance();
    
    while (!isAtEnd() && currentChar() != '\n') {
        advance();
    }
}

void Lexical::skipMultiLineComment() {
    // 跳过 /*
    advance();
    advance();
    
    while (!isAtEnd()) {
        if (currentChar() == '*' && peekChar() == '/') {
            advance(); // 跳过 *
            advance(); // 跳过 /
            break;
        }
        advance();
    }
    
    if (isAtEnd()) {
        throwError(_("Unterminated comment"), '*', "MULTI_COMMENT");
    }
}

Token Lexical::readIdentifierOrKeyword() {
    size_t start = index_;
    
    while (!isAtEnd() && isAlphaNumeric(currentChar())) {
        advance();
    }
    
    std::string text = source_code_.substr(start, index_ - start);
    
    // 检查特殊字面量
    if (text == "null") {
        return makeToken(TokenType::NULL_LITERAL, text);
    } else if (text == "true") {
        return makeToken(TokenType::BOOL_TRUE, text);
    } else if (text == "false") {
        return makeToken(TokenType::BOOL_FALSE, text);
    } else if (isKeyword(text)) {
        return makeToken(TokenType::KEYWORD, text);
    } else {
        return makeToken(TokenType::IDENT, text);
    }
}

Token Lexical::readNumber() {
    size_t start = index_;
    
    while (!isAtEnd() && isDigit(currentChar())) {
        advance();
    }
    
    // 处理小数点
    if (!isAtEnd() && currentChar() == '.' && isDigit(peekChar())) {
        advance(); // 跳过小数点
        while (!isAtEnd() && isDigit(currentChar())) {
            advance();
        }
    }
    
    // 处理科学计数法
    if (!isAtEnd() && (currentChar() == 'e' || currentChar() == 'E')) {
        advance();
        if (!isAtEnd() && (currentChar() == '+' || currentChar() == '-')) {
            advance();
        }
        if (isAtEnd() || !isDigit(currentChar())) {
            throwError(_("Invalid number format"), currentChar(), "NUMBER");
        }
        while (!isAtEnd() && isDigit(currentChar())) {
            advance();
        }
    }
    
    std::string text = source_code_.substr(start, index_ - start);
    return makeToken(TokenType::NUMBER, text);
}

Token Lexical::readString() {
    advance(); // 跳过开始的双引号
    
    std::string value;
    
    while (!isAtEnd() && currentChar() != '"') {
        if (currentChar() == '\\') {
            advance();
            char escaped = processEscapeSequence();
            value += escaped;
        } else {
            value += currentChar();
            advance();
        }
    }
    
    if (isAtEnd()) {
        throwError(_("Unterminated string"), '"', "STRING");
    }
    
    advance(); // 跳过结束的双引号
    return makeToken(TokenType::STRING, value);
}

Token Lexical::readChar() {
    advance(); // 跳过开始的单引号
    
    if (isAtEnd()) {
        throwError(_("Unterminated character literal"), '\'', "CHAR");
    }
    
    char value;
    if (currentChar() == '\\') {
        advance();
        value = processEscapeSequence();
    } else {
        value = currentChar();
        advance();
    }
    
    if (isAtEnd() || currentChar() != '\'') {
        throwError(_("Unterminated character literal"), '\'', "CHAR");
    }
    
    advance(); // 跳过结束的单引号
    return makeToken(TokenType::CHAR, std::string(1, value));
}

char Lexical::processEscapeSequence() {
    if (isAtEnd()) {
        throwError(_("Invalid escape sequence"), '\\', "ESCAPE");
    }
    
    char c = currentChar();
    advance();
    
    switch (c) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case 'b': return '\b';
        case 'f': return '\f';
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        case '0': return '\0';
        default:
            throwError(_("Invalid escape sequence"), c, "ESCAPE");
            return c;
    }
}

bool Lexical::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexical::isHexDigit(char c) {
    return isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool Lexical::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexical::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Lexical::isKeyword(const std::string& text) {
    return keywords_.find(text) != keywords_.end();
}

Token Lexical::makeToken(TokenType type, const std::string& value) const {
    return {type, value, line_, column_};
}

void Lexical::throwError(const std::string& error_type, char error_char, 
                        const std::string& token_type) const {
    throw LexicalException(error_type, error_char, token_type, line_, column_);
}

} // namespace dreamlang::lexer
