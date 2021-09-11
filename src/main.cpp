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
"\t-e, --eval\tevaluate program before dumping\n"
;

int main(int argc, char** argv) {
    try {
        bool eval = false;
        const char* file = nullptr;

        for (int i = 1; i != argc; ++i) {
            if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0) {
                std::cerr << usage;
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

        std::ifstream ifs(file);
        Parser parser(file, ifs);
        auto exp = parser.parse_prg();
        if (eval) exp = exp->eval(false);
        exp->dump();
    } catch (std::exception const& e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::cerr << usage;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "error: unknown exception" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
