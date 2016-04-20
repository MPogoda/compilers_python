#include <iostream>
#include <string>

#include "parse.h"
#include "lexeme.h"
#include "syntax.h"
#include "program_tree.h"
#include "program_stucture.h"

#include <fstream>
#include <streambuf>
int main( int argc, char* argv[] )
{
    if (2 > argc) {
        std::cout << "Little python parser by Michael Pogoda.\n"
                  << "Usage:\n"
                  << argv[0] << "[ file to parse ]\n";
        return 1;
    }

    const sap::Table table = sap::createTable();
    const sap::Stack ss{ { { sap::lex::type::COUNT, 0u }
                         , { sap::lex::type::RULE, sap::lex::rule::START }
                         } };

    while (1 < argc--) {
        std::ifstream in{ argv[ argc ] };
        in.unsetf( std::ios::skipws );
        const std::string str{ std::istreambuf_iterator< char >{ in }
                             , std::istreambuf_iterator< char >{} };

        std::cout << "File `\033[0;33m" << argv[ argc ] << "\033[0m'\n";
        try {
            sap::Lexems lexems = sap::parse( str );
            for (const auto& lexem : lexems ) {
                std::cout << lexem << '\n';
            }

            sap::Queue queue = sap::parse( table, lexems.begin(), lexems.end(), ss );

            sap::LIterator it = lexems.begin();
            sap::node node{ queue, it };
            std::cout << node << std::endl;

            sap::Program program{ node };
            std::cout << program.scope_;
            sap::ProgramCode::instance().reset();
            program();
            std::cout << sap::ProgramCode::instance();
        } catch ( std::exception& ex ) {
            std::cerr << ex.what();
        }


        std::cout << "\n\n";
    }

    return 0;
}
