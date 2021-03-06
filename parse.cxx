#include "parse.h"

#include <boost/spirit/include/qi_parse.hpp>

#include <stdexcept>
#include <sstream>
#include <stack>

#include "lexeme.h"
#include "lines_grammar.h"
#include "python_grammar.h"

namespace sap
{
namespace qi = boost::spirit::qi;

std::string createErrorMessage( const std::string& str, uint row, uint col );

Lexems parse( const Container input )
{
    static LinesGrammar     lines_grammar;
    static PythonGrammar    python_grammar;

    Lexems result;

    lines ls;
    {
        Iterator beg{ std::begin( input ) };
        Iterator end{ std::end( input ) };
        if (   (! qi::parse( beg, end, lines_grammar, ls ) )
            || (0 != std::distance( beg, end ) ) ) {
                throw std::invalid_argument{ "Lines aren't properly terminated." };
        }
    }

    uint i{ 0 };
    std::stack< uint > indents;
    result.push_back( { lex::type::INDENT, 0u } );
    indents.push( 0 );
    for (const auto& line : ls ) {
        ++i;
        Lexems tempResult;
        Iterator beg{ std::begin( line ) };
        Iterator end{ std::end( line ) };
        if (     (! qi::parse( beg, end, python_grammar, tempResult ) )
              || (0 != std::distance( beg, end ) ) ) {
                uint column = std::distance( std::begin( line ), beg);
                throw std::invalid_argument{ createErrorMessage( line, i, column ) };
        }

        if (tempResult.empty()) continue;

        // Compare indent levels of previous line and this one.
        const auto indent = tempResult.front();
        tempResult.pop_front();
        assert( indent.type_ == lex::type::INDENT );
        const uint new_indent = boost::get< uint >( indent.value_ );
        // If indent changed, push lexem.
        if ( new_indent != indents.top()) {
            if (new_indent < indents.top()) {
                while (new_indent < indents.top()) {
                    result.push_back( { lex::type::DEDENT, indents.top() } );
                    indents.pop();
                }
            } else {
                result.push_back( indent );
                indents.push( new_indent );
            }
        }

        for (const auto& lexem : tempResult ) {
            result.push_back( lexem );
        }

        result.push_back( { lex::type::NEWLINE, i } );
    }
    result.push_back( { lex::type::DEDENT, 0u } );
    assert( indents.size() == 1 );

    return std::move( result );
}

std::string createErrorMessage( const std::string& str, const uint row, uint col )
{
    std::stringstream ss;

    ss << "Cannot parse at position (" << row << ", " << col + 1 << "):\n"
       << str << '\n';
    while (0 != col-- )
        ss << ' ';
    ss << "\033[0;31m^~~~\033[0m\n";

    return ss.str();
}
} // namespace sap
