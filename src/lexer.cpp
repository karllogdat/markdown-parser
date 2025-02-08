#include "../include/lexer.h"
#include <cctype>
#include <string>

Lexer::Lexer(const std::string& input) : input(input), position(0) {}

char Lexer::peek() const {
    return position < input.size() ? input[position] : '\0';
}

char Lexer::advance() {
    return position < input.size() ? input[position++] : '\0';
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek())) {
        advance();
    }
}

Token Lexer::nextToken() {
    skipWhitespace();

    // end of file token    
    if (position >= input.size()) {
        return {TokenType::END_OF_FILE, ""};
    }

    char c = peek();

    if (c == '#') {
        std::string value;
        while (peek() == '#') {
            value += advance();
        }
        return {TokenType::HEADING, value};
    }

    if (c == '*') {
        // list items only start at the start of a line
        if (position == 0 || input[position - 1] == '\n') {
            if (position + 1 < input.size() && std::isspace(input[position] + 1)) {
                std::string value;
                // consume list marker *
                value += advance();
                // consume whitespaces
                while (std::isspace(peek()) && peek() !=  '\n') {
                    value += advance();
                }
                while (peek() != '\n' && peek() != '\0') {
                    value += advance();
                }
                return {TokenType::LIST_ITEM, value};
            }
        }

        // checks for bold / italic
        char marker = advance(); // consumes marker
        bool isBold = (peek() == marker);

        // consume second marker
        if (isBold)
            advance();

        std::string value;
        while (peek() != marker && peek() != '\0') {
            value += advance();
        }

        if (peek() == marker) {
            advance();
            if (isBold && peek() == marker) {
                advance();
                return {TokenType::BOLD, value};
            }
            return {TokenType::ITALIC, value};
        }
    } 

    // default: text
    // if the function reaches here it means it didnt
    // identify all the other shit
    std::string value;
    while (position <= input.size() && peek() != '\n') {
        value += advance();
    }
    return {TokenType::TEXT, value};
}

bool Lexer::hasNext() const {
    return position < input.size();
}