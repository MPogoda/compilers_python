#pragma once
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>

#include <string>

#include "parse.h"
#include "lexeme.h"
namespace sap
{
namespace qi = boost::spirit::qi;

struct PythonGrammar : qi::grammar< Iterator, Lexems() >
{
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

    qi::rule< Iterator, lex::type()   > constant_type;
    qi::rule< Iterator, std::string() > string_constant;
    qi::rule< Iterator, double()      > double_constant;
    qi::rule< Iterator, bool()        > bool_constant;
    qi::rule< Iterator, lex()         > constant_rule;

    qi::rule< Iterator, lex() > visible_lexeme;

    qi::rule< Iterator, void() >    empty_line;
    qi::rule< Iterator, Lexems() >  non_empty_line;

    qi::rule< Iterator, Lexems() > start;

    PythonGrammar( );
}; // struct PythonGrammar


} // namespace sap
