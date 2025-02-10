#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// base ast node
struct Node {
    virtual ~Node() = default;
    virtual std::string render() = 0;
};

using NodePtr = std::shared_ptr<Node>;

// actual nodes
struct TextNode : Node {
    std::string text;
    explicit TextNode(std::string s) : text(s) {}
    std::string render() override;
};

// inline nodes
struct BoldNode : Node {
    std::vector<NodePtr> children;
    std::string render() override;
};

struct ItalicNode : Node {
    std::vector<NodePtr> children;
    std::string render() override;
};

struct InlineCodeNode : Node {
    std::string code;
    explicit InlineCodeNode(std::string s) : code(s) {}
    std::string render() override;
};

struct ListEntryNode : Node {
    std::vector<NodePtr> children;
    std::string render() override;
};

// block level nodes
struct HeaderNode : Node {
    int level;
    std::vector<NodePtr> children;
    HeaderNode(int l) : level(l) {}
    std::string render() override;
};

struct ParagraphNode : Node {
    std::vector<NodePtr> children;
    std::string render() override;
};

struct BlockCodeNode : Node {
    std::string code;
    explicit BlockCodeNode(std::string c) : code(c) {}
    std::string render() override;
};

struct OrderedListNode : Node {
    std::vector<NodePtr> children;
    std::string render() override;
};

struct UnorderedListNode : Node {
    std::vector<NodePtr> children;
    std::string render() override;
};

// document node 
struct DocumentNode : Node {
    std::vector<NodePtr> children;
    std::string render() override;
};

class Parser {
public:
    explicit Parser(Lexer lexer);
    NodePtr parse();

private:
    Lexer lexer;
    std::vector<Token> tokens;
    size_t position;

    Token peek() const;
    Token advance();

    NodePtr parseBlock();
    NodePtr parseInline();
};

#endif
