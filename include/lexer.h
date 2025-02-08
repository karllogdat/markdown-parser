#ifndef LEXER_H
#define LEXER_H

#include <cstddef>
#include <string>

enum class TokenType {
    HEADING,
    TEXT,
    BOLD,
    ITALIC,
    CODE,
    LIST_ITEM,
    BLOCKQUOTE,
    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input);
    Token nextToken();
    bool hasNext() const;

private:
    std::string input;
    size_t position;

    char peek() const;
    char advance();
    void skipWhitespace();
};

#endif