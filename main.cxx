#include <iostream>
#include <string>

#include "grammar.h"
#include "lexeme.h"

int main( int /* argc */, char* /* argv */[] )
{
    std::string str("class someclass:\n"
                    "    def foo( self, x ):\n"
                    "        if x == 5:\n"
                    "            print( \"foo\" )\n"
                    "        else:\n"
                    "            print( \"bar\" )\n"
                    "\n"
                    "    def bar( self, a, b ):\n"
                    "        c = b - a\n"
                    "        return c\n"
                    "\n"
                    "class someotherclass:\n"
                    "    def foo( self, a, b):\n"
                    "        c = someclass()\n"
                    "        z = c.bar( a, b )\n"
                    "        c.foo( z )\n"
                    "\n"
                    "\n"
                    "class main:\n"
                    "    def run( self ):\n"
                    "        m = someotherclass()\n"
                    "        m.foo( 5, 10 )\n"
                    "        m.foo( 3, 6 )\n"
                    "\n"
                    "\n"
                    "main().run()\n"
                    );




    std::cin.unsetf( std::ios::skipws );

    sap::Lexems lexems = sap::parse( str );

    std::cout << "\n\n\n";
    for (const auto& lexem : lexems )
        std::cout << lexem << '\n';

    std::cout << '\n';

    return 0;
}
