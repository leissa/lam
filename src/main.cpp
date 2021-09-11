#include "parser.h"

#include <cstring>
#include <iostream>
#include <fstream>

using namespace lam;

static const auto usage =
"Usage: lam [options] file\n"
"\n"
"Options:\n"
"\t-h, --help\tdisplay this help and exit\n"
"\t-v, --version\tdisplay version info and exit\n"
"\t-e, --eval\tevaluate program before dumping\n"
"\n"
"Hint: use '-' as file to read from stdin.\n"
;

static const auto version = "lam interpreter 0.1\n";

int main(int argc, char** argv) {
    try {
        bool eval = false;
        const char* file = nullptr;

        for (int i = 1; i != argc; ++i) {
            if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0) {
                std::cerr << usage;
                return EXIT_SUCCESS;
            } else if (strcmp("-v", argv[i]) == 0 || strcmp("--version", argv[i]) == 0) {
                std::cerr << version;
                return EXIT_SUCCESS;
            } else if (strcmp("-e", argv[i]) == 0 || strcmp("--eval", argv[i]) == 0) {
                eval = true;
            } else if (file == nullptr) {
                file = argv[i];
            } else {
                throw std::logic_error("multiple input files given");
            }
        }

        if (file == nullptr)
            throw std::logic_error("no input file given");

        Ptr<Exp> exp;
        if (strcmp("-", file) == 0) {
            Parser parser("<stdin>", std::cin);
            exp = parser.parse_prg();
        } else {
            std::ifstream ifs(file);
            Parser parser(file, ifs);
            exp = parser.parse_prg();
        }

        if (eval) exp = exp->eval();
        exp->dump();
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::cerr << usage;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "error: unknown exception" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
