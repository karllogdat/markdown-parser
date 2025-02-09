#include <fstream>
#include <iostream>
#include <string>
#include "../include/lexer.h"

int main(void) {
    std::cout << "[INFO] Markdown parser initialized" << std::endl;

    std::ifstream mdfile("README.md");
    std::cout << "[INFO] Initializing markdown file: " << ((mdfile) ? "OPEN" : "ERROR") << std::endl;

    std::string md;
    std::string line;
    std::cout << "[INFO] File contents: " << std::endl;
    while (std::getline(mdfile, line)) {
        md += line;
        std::cout << "[INFO] " << line << std::endl;
        md.push_back('\n');
    }

    Lexer markdownLexer(md);

    while (markdownLexer.hasNext()) {
        Token tok = markdownLexer.nextToken();
        std::cout << "--------------------" << std::endl;
        tok.print();
    }

    return 0;
}
