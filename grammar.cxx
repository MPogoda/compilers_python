#include "grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace sap
{
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
    indent_type  = eps [ _val = lex::type::INDENT ];
    indent_rule %= indent_type >> indent_level;

    symbol_type  = eps [ _val = lex::type::SYMBOL ];
    symbol      %=  symbols;
    symbol_rule  %= symbol_type >> symbol;

    newline  = eol [ _val = lex::symbol::EOL ];
    newline_rule %= symbol_type >> newline;

    start %= indent_rule
          >> *(*lit(" ") >> symbol_rule >> *lit(" "))
          >> newline_rule;

    // identifier %=  (alpha | char_('_')) >> *(alnum | char_('_'));
    // identifiers %= identifier % char_(' ');

    // start       %= indent_level
                // >> identifiers;
}
} // namespace sap
