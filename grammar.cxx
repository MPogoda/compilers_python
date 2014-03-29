#include "grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "lexeme.h"

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

        line_  %= *(char_ - eol) >> eol;
        start_ %= *line_;

    }

    qi::rule< Iterator, std::string() > line_;
    qi::rule< Iterator, lines() >       start_;
} linesGrammar;

struct symbols_ : qi::symbols< char, lex::symbol >
{
    symbols_()
    {
        using symbol = lex::symbol;
        // assert enumerations are continious.
        // because i've created them
        constexpr uint8_t MAX{ static_cast< uint8_t >( symbol::COUNT ) };
        for ( uint8_t s = 0; s != MAX; ++s)
            add( lex::symbol_STRINGS[ s ], static_cast< symbol >( s ) );
    }
} symbols;

struct reserved_words_ : qi::symbols< char, lex::reserved_word >
{
    reserved_words_()
    {
        using rword = lex::reserved_word;
        constexpr uint8_t MAX{ static_cast< uint8_t >( rword::COUNT ) };
        for ( uint8_t s = 0; s != MAX; ++s )
            add( lex::reserved_STRINGS[ s ], static_cast< rword >( s ) );
    }
} reserved_words;

class MyGrammar : public qi::grammar< Iterator, Lexems() >
{
public:
    qi::rule< Iterator, void() >        indent;
    qi::rule< Iterator, uint() >        indent_level;
    qi::rule< Iterator, lex::type() >   indent_type;
    qi::rule< Iterator, lex() >         indent_rule;

    qi::rule< Iterator, lex::type() >   symbol_type;
    qi::rule< Iterator, lex::symbol() > symbol;
    qi::rule< Iterator, lex() >         symbol_rule;

    qi::rule< Iterator, lex::type() >           reserved_type;
    qi::rule< Iterator, lex::reserved_word() >  reserved;
    qi::rule< Iterator, lex() >                 reserved_rule;

    qi::rule< Iterator, lex::type() >   identifier_type;
    qi::rule< Iterator, std::string() > identifier;
    qi::rule< Iterator, lex() >         identifier_rule;

    qi::rule< Iterator, lex() > visible_lexeme;

    qi::rule< Iterator, void() >    empty_line;
    qi::rule< Iterator, Lexems() >  non_empty_line;

    qi::rule< Iterator, Lexems() > start;

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

    // match 4 spaces
    indent       = lit("    ");
    // calculate indent level
    indent_level = eps              [ _val = 0  ]
                // for every indent, increase _val
                >> *indent          [ _val += 1 ];
    // dummy, sets lex.type_ field
    indent_type  = eps              [ _val = lex::type::INDENT ];
    indent_rule %= indent_type
                >> indent_level;

    // dummy, sets lex.type_ field
    symbol_type  = eps          [ _val = lex::type::SYMBOL ];
    // match symbols against symbol-table
    symbol      %= symbols;
    symbol_rule %= symbol_type
                >> symbol;

    // dummy, sets lex.type_ field
    reserved_type    = eps              [ _val = lex::type::RESERVED ];
    // match against symbol table
    reserved        %= reserved_words;
    reserved_rule   %= reserved_type
                    >> reserved;

    // dummy, sets lex.type_ field
    identifier_type  = eps                  [ _val = lex::type::IDENTIFIER ];

    identifier      %=  (alpha | char_('_'))    // letter or _
                                                // followed by letter, number or _ undetermined number of times
                    >> *(alnum | char_('_'));
    identifier_rule  = identifier_type
                    >> identifier;

    // matches all lexemes, that a visible (not indent & newline )
    // it's either reserved word, or symbol, or identifier
    visible_lexeme  %= reserved_rule
                    |  symbol_rule
                    |  identifier_rule
                    ;

    // Matches only indent, but doesn't set anything (it's type is void)
    empty_line       = indent_rule;
    // Matches typical line (indent, followed by visible_lexeme s )
    non_empty_line  %= indent_rule
                    >> +(   // one or more
                           *lit(" ")        // ignore spaces
                        >> visible_lexeme   // some visible_lexeme
                        >> *lit(" ")        // ignore spaces
                        );

    start   = non_empty_line    [ _val = _1 ]
            | empty_line;
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

    return result;
}
} // namespace sap
