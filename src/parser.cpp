#include "../include/parser.h"
#include <memory>
#include <string>

std::string TextNode::render() {
    return text;
}

std::string BoldNode::render() {
    std::string result = "<strong>";
    for (const auto& child : children) {
        result += child->render();
    }
    return result + "</strong>";
}

std::string ItalicNode::render() {
    std::string result = "<em>";
    for (const auto& child : children) {
        result += child->render();
    }
    return result + "</em>";
}

std::string InlineCodeNode::render() {
    return "<code>" + code + "</code>";
}

std::string ListEntryNode::render() {
    std::string result = "<li>";
    for (const auto& child : children) {
        result += child->render();
    }
    return result + "</li>";
}

// block level node rendering
std::string HeaderNode::render() {
    std::string result = "<h" + std::to_string(level) + ">";
    for (const auto& child : children) {
        result += child->render();
    }
    return result + "</h" + std::to_string(level) + ">";
}

std::string ParagraphNode::render() {
    std::string result = "<p>";
    for (const auto& child : children) {
        result += child->render();
    }
    return result + "<p>";
}

std::string BlockCodeNode::render() {
    std::string result = "<pre><code>";
    result += code; 
    return result + "</code></pre>";
}

std::string OrderedListNode::render() {
    std::string result = "<ol>";
    for (const auto& child : children) {
        result += child->render();
    }
    return result + "</ol>";
}

std::string UnorderedListNode::render() {
    std::string result = "<ul>";
    for (const auto& child : children) {
        result += child->render();
    }
    return result + "</ul>";
}

std::string DocumentNode::render() {
    std::string result;
    for (const auto& child : children) {
        result += child->render();
    }
    return result;
}

Parser::Parser(Lexer lexer) : lexer(lexer) {
    Token token(TokenType::END_OF_FILE);
    do {
        token = lexer.nextToken();
        tokens.push_back(token);
    } while (token.type != TokenType::END_OF_FILE);
}

Token Parser::peek() const {
    if (position >= tokens.size()) return Token(TokenType::END_OF_FILE);
    return tokens[position];
}

Token Parser::advance() {
    Token token = peek();
    if (token.type != TokenType::END_OF_FILE)
        position++;
    return token;
}

NodePtr Parser::parseInline() {
    Token token = peek();

    if (token.type == TokenType::TEXT) {
        advance();
        return std::make_shared<TextNode>(token.value);
    }

    if (token.type == TokenType::ASTERISK) {
        advance();
        Token next = peek();
        if (next.type == TokenType::ASTERISK) {
            advance();
            auto node = std::make_shared<BoldNode>();
            while (peek().type != TokenType::END_OF_FILE &&
                   !(peek().type != TokenType::ASTERISK &&
                     position + 1 < tokens.size() &&
                     tokens[position + 1].type == TokenType::ASTERISK)) {
                node->children.push_back(parseInline());
            }
            if (peek().type == TokenType::ASTERISK) {
                advance();
                advance();
            }
            return node;
        } else {
            auto node = std::make_shared<ItalicNode>();
            while (peek().type != TokenType::END_OF_FILE &&
                   peek().type != TokenType::ASTERISK) {
                node->children.push_back(parseInline());
            }
            if (peek().type == TokenType::ASTERISK) 
                advance();
            
            return node;
        }
    }
    
    if (token.type == TokenType::BACKTICK) {
        advance();
        std::string code;
        while (peek().type != TokenType::END_OF_FILE &&
               peek().type != TokenType::BACKTICK) {
            Token t = advance();
            code = t.value;
        }
        if (peek().type == TokenType::BACKTICK) {
            advance();
        }
        return std::make_shared<InlineCodeNode>(code);
    }

    advance();
    return std::make_shared<TextNode>(token.value);
}

NodePtr Parser::parseBlock() {
    Token token = peek();
    
    if (token.type == TokenType::HASH && token.isStartOfLine) {
        int level = 0;
        while (peek().type == TokenType::HASH) {
            advance();
            level++;
        }

        if (peek().type == TokenType::SPACE)
            advance();

        auto node = std::make_shared<HeaderNode>(level);
        while (peek().type != TokenType::END_OF_FILE &&
               peek().type != TokenType::NEW_LINE)
            node->children.push_back(parseInline());

        if (peek().type == TokenType::NEW_LINE)
            advance();

        return node;
    }

    if ((token.type == TokenType::ASTERISK) 
        && token.isStartOfLine) {
        auto node = std::make_shared<UnorderedListNode>();
        while (peek().type != TokenType::END_OF_FILE && 
               peek().isStartOfLine &&
               (peek().type == TokenType::ASTERISK || peek().type == TokenType::TEXT)) {
            if (peek().type == TokenType::ASTERISK) advance();
            if (peek().type == TokenType::SPACE) advance();

            auto item = std::make_shared<ListEntryNode>();
            while (peek().type != TokenType::END_OF_FILE &&
                   peek().type != TokenType::NEW_LINE) {
                item->children.push_back(parseInline());
            }
            if (peek().type == TokenType::NEW_LINE) {
                advance();
            }
            node->children.push_back(item);
        }
        return node;
    }

    if ((token.type == TokenType::NUMBER)
         && token.isStartOfLine) {
        auto node = std::make_shared<OrderedListNode>();
        while (peek().type != TokenType::END_OF_FILE &&
               peek().isStartOfLine) {
            if (peek().type == TokenType::NUMBER) advance();
            if (peek().type == TokenType::SPACE) advance();

            auto item = std::make_shared<ListEntryNode>();
            while (peek().type != TokenType::END_OF_FILE &&
                   peek().type != TokenType::NEW_LINE) {
                item->children.push_back(parseInline());
            }
            if (peek().type == TokenType::NEW_LINE) {
                advance();
            }
            node->children.push_back(item);
        }
        return node;
    }

    auto node = std::make_shared<ParagraphNode>();
    while (peek().type != TokenType::END_OF_FILE) {
        if (peek().type == TokenType::NEW_LINE && peek().isStartOfLine)
            break;
        node->children.push_back(parseInline());
    }

    if (peek().type == TokenType::NEW_LINE)
        advance();

    return node;
}

NodePtr Parser::parse() {
    auto document = std::make_shared<DocumentNode>();
    while (peek().type != TokenType::END_OF_FILE) {
        if (peek().type == TokenType::NEW_LINE) {
            advance();
            continue;
        }
        document->children.push_back(parseBlock());
    }
    return document;
}
