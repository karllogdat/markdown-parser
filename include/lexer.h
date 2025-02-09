#ifndef LEXER_H
#define LEXER_H

#include <cstddef>
#include <string>

enum class TokenType {
    HASH,
    ASTERISK,
    BACKTICK,
    SPACE,
    NEW_LINE,
    // TAB, ?
    TEXT,
    NUMBER, // only count if start of line
    DASH,
    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string value;
    bool isStartOfLine;

    Token(TokenType t, std::string v = "", bool sol = false) :
        type(t), value(v), isStartOfLine(sol) {}

    void print();
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
};

#endif
