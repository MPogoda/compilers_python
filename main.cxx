#include <iostream>
#include <string>

#include "grammar.h"
#include "lexeme.h"

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
        } catch ( std::exception& ex ) {
            std::cerr << ex.what();
        }

        std::cout << "\n\n";
    }

    return 0;
}
