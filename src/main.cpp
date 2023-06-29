#include <chrono>
#include <fstream>
#include <iostream>

#include "astformatter.hpp"
#include "asthighlighter.hpp"
#include "astjsonwriter.hpp"
// #include "astprinter.hpp"
#include "mcc.hpp"

auto main(int argc, const char** argv) -> int {
    if (argc < 2) {
        std::cout << "\nUsage: mcc <file>\n\n";
    } else {
        auto source_stream = mcc::SrcStream(argv[1]);
        auto token_stream  = mcc::lex(std::move(source_stream));
        auto preprocessor  = mcc::preprocess(std::move(token_stream));
        auto compile_unit  = mcc::parse(std::move(preprocessor));

        auto json        = std::ofstream("out.json");
        auto source      = std::ofstream("out.c");
        auto jsonizer    = mcc::AstJsonWriter(json);
        auto formatter   = mcc::AstFormatter(source);
        auto highlighter = mcc::AstHighlighter(std::cout);

        compile_unit.accept(jsonizer);
        compile_unit.accept(formatter);
        compile_unit.accept(highlighter);
    }

    return 0;
}