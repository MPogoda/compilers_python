#pragma once
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>

#include <string>
#include <deque>

#include "lexeme.h"

namespace sap
{
namespace qi = boost::spirit::qi;

using Iterator = std::string::const_iterator;
using Return   = std::deque< sap::lex >;

class MyGrammar : public qi::grammar< Iterator, Return() >
{
public:
    qi::rule< Iterator, void() >            indent;
    qi::rule< Iterator, short() >         indent_level;
    qi::rule< Iterator, sap::lex::type() >    indent_type;
    qi::rule< Iterator, sap::lex() >          indent_rule;

    qi::rule< Iterator, sap::lex::type() >    symbol_type;
    qi::rule< Iterator, sap::lex::symbol() >  symbol;
    qi::rule< Iterator, sap::lex() >          symbol_rule;

    qi::rule< Iterator, sap::lex::symbol() > newline;
    qi::rule< Iterator, sap::lex() >           newline_rule;

    // qi::rule< Iterator, std::string() > identifier;
    // qi::rule< Iterator, std::vector< std::string >() > identifiers;

    qi::rule< Iterator, Return() >      start;

    MyGrammar( );
}; // class MyGrammar

} // namespace sap
