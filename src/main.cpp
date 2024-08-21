#include "codegen.hpp"
#include "debug.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    debug("debug mode enabled");
    if (argc < 2) {
        error("incorrect usage, for help run `cale --help`");
        return EXIT_FAILURE;
    }

    std::string path;
    // parse arguments
    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        if (len > 1 && argv[i][0] == '-') {
            for (int j = 1; j < len; ++j) {
                switch (argv[i][j]) {
                case 'h': {
                    std::cout << "help" << std::endl;
                    return EXIT_SUCCESS;
                }
                case '-': {
                    if (!strcmp(argv[i], "--help")) {
                        std::cout << "help" << std::endl;
                        return EXIT_SUCCESS;
                    } else {
                        std::string err_msg =
                            "unknown argument `" + std::string(argv[i]) +
                            "`\n" + "for more info, run `cale --help`";
                        error(err_msg);
                        return EXIT_FAILURE;
                    }
                }
                default: {
                    std::string err_msg = "unknown argument `-" +
                                          std::string(1, argv[i][j]) + "`\n" +
                                          "for more info, run `cale --help`";
                    error(err_msg);
                    return EXIT_FAILURE;
                }
                }
            }
        } else {
            path = argv[i];
            // get file extension
            size_t ext_pos = path.find_last_of('.');
            if (ext_pos == std::string::npos ||
                path.find('/', ext_pos) != std::string::npos) {
                std::string err_msg =
                    "`" + std::string(argv[i]) + "` file has no extension";
                error(err_msg);
                return EXIT_FAILURE;
            }

            std::string ext = path.substr(ext_pos);

            // check if the file has the right extension
            if (ext != ".cal") {
                std::string err_msg =
                    "`" + ext + "` file extension not recognised";
                error(err_msg);
                return EXIT_FAILURE;
            }
        }
    }

    std::ifstream infile(path);
    if (!infile.is_open()) {
        std::string err_msg = "`" + path + "` file does not exist";
        perr(err_msg.c_str());
        return EXIT_FAILURE;
    }

    std::stringstream buf_stream;
    buf_stream << infile.rdbuf();
    std::string buf = buf_stream.str();

    infile.close();

    lexer_t lexer(buf);
    std::optional<std::vector<Token>> tokens = lexer.lex();
    if (!tokens.has_value()) {
        return EXIT_FAILURE;
    }

#if DEBUG
    for (auto &token : *tokens) {
        debug("Token: " << token.getLexeme());
    }
#endif

    parser_t parser(*tokens);
    std::optional<std::vector<stmt_t>> statements = parser.parse();
    if (!statements.has_value()) {
        return EXIT_FAILURE;
    }

    codegen_t codegen(*statements);
    if (codegen.generate() != 0) {
        return EXIT_FAILURE;
    }

    if (std::system("nasm -f elf32 temp.asm -o temp.o") != 0) {
        return EXIT_FAILURE;
    }
#if DEBUG
    std::stringstream asm_stream;
    std::ifstream asmfile("temp.asm");
    if (!asmfile.is_open()) {
        perr("failed to open file");
        return EXIT_FAILURE;
    }
    asm_stream << asmfile.rdbuf();
    std::string asm_str = asm_stream.str();
    debug("assembly" << std::endl << asm_str);
#endif
    if (std::system("rm -rf temp.asm") != 0) {
        return EXIT_FAILURE;
    }
    if (std::system("ld -m elf_i386 temp.o -o out") != 0) {
        return EXIT_FAILURE;
    }
    if (std::system("rm -rf temp.o") != 0) {
        return EXIT_FAILURE;
    }

    debug("success");
    return EXIT_SUCCESS;
}
