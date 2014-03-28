#include <iostream>
#include <string>

#include <boost/spirit/include/qi_parse.hpp>

#include "grammar.h"
#include "lexeme.h"

int main( int /* argc */, char* /* argv */[] )
{
    std::string str("    , + - \n    \n        \n\n\n");
    std::cin.unsetf( std::ios::skipws );

    sap::MyGrammar myGrammar;

        sap::Return result;
        sap::Iterator beg{ str.begin() };
        sap::Iterator end{ str.end() };

        std::cout << "Distance: " << std::distance( beg, end ) << "\n";

        while (boost::spirit::qi::parse( beg, end, myGrammar, result ))
            ;

        std::cout << "Distance: " << std::distance( beg, end ) << "\n";
        for (const auto& r : result)
            std::cout << r << '\t';
        // std::cout << "Result: " << result << "\n";
        //
        std::cout.flush();

    return 0;
}
