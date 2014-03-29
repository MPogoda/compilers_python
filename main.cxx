#include <iostream>
#include <string>

#include "grammar.h"
#include "lexeme.h"

int main( int /* argc */, char* /* argv */[] )
{
    std::string str("    , + - \n    \n        =\n\n\n");
    std::cin.unsetf( std::ios::skipws );

    sap::Lexems lexems = sap::parse( str );

    std::cout << "\n\n\n";
    for (const auto& lexem : lexems )
        std::cout << lexem << "; ";

    std::cout << '\n';

    return 0;
}
