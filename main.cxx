#include <iostream>
#include <string>

#include <boost/spirit/include/qi_parse.hpp>

#include "grammar.h"

int main( int /*argc*/, char* /*argv*/[] )
{
    std::string str;
    std::cin.unsetf( std::ios::skipws );

    sap::MyGrammar myGrammar;
    while (getline( std::cin, str)) {
        if (str.empty()) {
            break;
        }

        sap::Return result;
        sap::Iterator beg{ str.begin() };
        sap::Iterator end{ str.end() };

        std::cout << "Distance: " << std::distance( beg, end ) << "\n";

        bool r = boost::spirit::qi::parse( beg, end, myGrammar, result );

        std::cout << "Distance: " << std::distance( beg, end ) << "\n";
        std::cout << "R: " << r << "\n";
        std::cout << "Result: " << static_cast< uint32_t >( result )<< "\n";


        std::cout.flush();
    }

    return 0;
}
