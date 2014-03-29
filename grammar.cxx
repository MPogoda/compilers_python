#include "grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
namespace sap
{
namespace qi = boost::spirit::qi;
using lines = std::deque< std::string >;

struct LinesGrammar : qi::grammar< Iterator, lines() >
{
    LinesGrammar() : base_type( start_ )
    {
        using namespace qi::labels;
        using qi::eol;
        using qi::ascii::char_;

        line_ %= *(char_ - eol) >> eol;
        start_ %= *line_;

    }

    qi::rule< Iterator, std::string() > line_;
    qi::rule< Iterator, lines() >       start_;
} linesGrammar;

struct symbols_ : qi::symbols< char, lex::symbol >
{
    symbols_()
    {
        add
            ( ",", lex::symbol::COMMA )
            ( ".", lex::symbol::DOT )
            ( "(", lex::symbol::L_PARENTHESIS )
            ( ")", lex::symbol::R_PARENTHESIS )
            ( "+", lex::symbol::PLUS )
            ( "-", lex::symbol::MINUS )
            ( "*", lex::symbol::STAR )
            ( "/", lex::symbol::SLASH )
            ( "=", lex::symbol::EQUAL )
            ( "<", lex::symbol::LESS )
            ( ">", lex::symbol::GREATER )
            ( "!", lex::symbol::NOT )
            ( ":", lex::symbol::COLON )
        ;
    }
} symbols;

class MyGrammar : public qi::grammar< Iterator, Lexems() >
{
public:
    qi::rule< Iterator, void() >                indent;
    qi::rule< Iterator, uint() >                indent_level;
    qi::rule< Iterator, lex::type() >      indent_type;
    qi::rule< Iterator, lex() >            indent_rule;

    qi::rule< Iterator, lex::type() >      symbol_type;
    qi::rule< Iterator, lex::symbol() >    symbol;
    qi::rule< Iterator, lex() >            symbol_rule;

    qi::rule< Iterator, void() > empty_line;
    qi::rule< Iterator, Lexems() > non_empty_line;

    // qi::rule< Iterator, std::string() > identifier;
    // qi::rule< Iterator, std::vector< std::string >() > identifiers;

    qi::rule< Iterator, Lexems() >      start;

    MyGrammar( );
} myGrammar; // class MyGrammar

MyGrammar::MyGrammar()
    : base_type( start )
{
    using qi::ascii::char_;
    using qi::lit;
    using namespace qi::ascii;
    using namespace qi::labels;
    using qi::eps;
    using qi::eol;
    using qi::no_skip;
    using qi::skip;
    using qi::space;

    indent       = lit("    ");
    indent_level = eps              [ _val = 0  ]
                >> +indent          [ _val += 1 ];
    indent_type  = eps              [ _val = lex::type::INDENT ];
    indent_rule %= indent_type
                >> indent_level;

    symbol_type  = eps       [ _val = lex::type::SYMBOL ];
    symbol      %= symbols;
    symbol_rule %= symbol_type
                >> symbol;

    empty_line = indent_rule;
    non_empty_line %= indent_rule >>
                    +( *lit(" ") >> symbol_rule >> *lit(" "));

    start = non_empty_line [ _val = _1 ]
           | empty_line;


    // identifier %=  (alpha | char_('_')) >> *(alnum | char_('_'));
    // identifiers %= identifier % char_(' ');

    // start       %= indent_level
                // >> identifiers;
}

Lexems parse( const Container input )
{
    Lexems result;
    Iterator beg{ std::begin( input ) };
    Iterator end{ std::end( input ) };

    std::cout << "Distance: " << std::distance( beg, end ) << '\n';

    lines ls;
    std::cout << "LINES PARSING RESULT: " << boost::spirit::qi::parse( beg, end, linesGrammar, ls ) << '\n';
    std::cout << "Distance: " << std::distance( beg, end ) << '\n';

    uint i{ 0 };
    for (const auto& line : ls ) {
        std::cout << ++i << "\t'" << line << "'\n";
        Lexems tempResult;
        beg = std::begin( line );
        end = std::end( line );
        std::cout << "\tDistance: " << std::distance( beg, end ) << '\n';
        std::cout << "\tparse: " << qi::parse( beg, end, myGrammar, tempResult ) << "\n\t";
        for (const auto& lexem : tempResult ) {
            result.push_back( lexem );
            std::cout << lexem << ";  ";
        }
        lex newLine;
        newLine.type_ = lex::type::NEWLINE;
        newLine.value_ = i;
        result.push_back( newLine );
        std::cout << '\n';

    }

        // while (boost::spirit::qi::parse( beg, end, myGrammar, result ))
        //     ;

    return result;
}
} // namespace sap
