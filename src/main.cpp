#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "../include/lexer.h"
#include "../include/parser.h"

int main(void) {
    std::ifstream mdfile("README.md");
    std::string md;
    std::string line;
    while (std::getline(mdfile, line)) {
        md += line;
        md.push_back('\n');
    }
    Lexer mdlexer(md);
    Parser mdparser(mdlexer);

    std::cout << mdparser.parse()->render() << std::endl;
}
