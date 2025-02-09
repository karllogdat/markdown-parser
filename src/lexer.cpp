#include "../include/lexer.h"
#include <cctype>
#include <iostream>
#include <string>

void Token::print() {
    std::string tstr;
    switch (type) {
        case TokenType::HASH:
            tstr = "HASH";
            break;
        case TokenType::ASTERISK:
            tstr = "ASTERISK";
            break;
        case TokenType::BACKTICK:
            tstr = "BACKTICK";
            break;
        case TokenType::SPACE:
            tstr = "SPACE";
            break;
        case TokenType::NEW_LINE:
            tstr = "NEW_LINE";
            break;
        case TokenType::TEXT:
            tstr = "TEXT";
            break;
        case TokenType::NUMBER:
            tstr = "NUMBER";
            break;
        case TokenType::DASH:
            tstr = "DASH";
            break;
        case TokenType::END_OF_FILE:
            tstr = "END_OF_FILE";
            break;
        default:
            tstr = "UNKNOWN";
            break;
    }

    std::cout << "[INFO] TokType: " << tstr << std::endl;
    std::cout << "[INFO] IsSOL  : " << (isStartOfLine ? "True" : "False") << std::endl;
    std::cout << "[INFO] Value  : " << value << std::endl;
}

Lexer::Lexer(const std::string& input) : input(input), position(0) {}

char Lexer::peek() const {
    return position < input.size() ? input[position] : '\0';
}

char Lexer::advance() {
    return position < input.size() ? input[position++] : '\0';
}


Token Lexer::nextToken() {
    if (position >= input.size()) {
        return Token(TokenType::END_OF_FILE);
    }

    char c = advance();
    bool isSol = true;
    if (position > 1 || c == '\n') {
        isSol = (input[position - 2] == '\n') ? true : false;
    }

    if (c == '#') {
        return Token(TokenType::HASH, {c}, isSol);
    }

    if (c == '*') {
        return Token(TokenType::ASTERISK, {c}, isSol);
    }

    if (c == '`') {
        return Token(TokenType::BACKTICK, {c}, isSol);
    }

    if (c == '\n') {
        return Token(TokenType::NEW_LINE, {c}, isSol);
    }

    if (c == ' ' || c == '\t') {
        return Token(TokenType::SPACE, {c}, isSol);
    }

    if (c == '-') {
        return Token(TokenType::DASH, {c}, isSol);
    }

    if (std::isdigit(c) && isSol) {
        std::string num;
        num += c;
        while (std::isdigit(peek()) || peek() == '.') {
            num += advance();
        }
        return Token(TokenType::NUMBER, num, isSol);
    }

    // current char did not match any marker
    std::string text;
    text += c;
    while (position <= input.size() &&
            // list all markers here
            peek() != '#' &&
            peek() != '*' &&
            peek() != '`' &&
            peek() != '\n' &&
            peek() != '-'
            ) {
        text += advance();
    }
    return Token(TokenType::TEXT, text, isSol);
}

bool Lexer::hasNext() const {
    return position < input.size();
}
