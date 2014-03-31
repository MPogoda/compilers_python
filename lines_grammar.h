#pragma once
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>

#include <string>
#include <deque>

#include "parse.h"
namespace sap
{
namespace qi = boost::spirit::qi;

using lines = std::deque< std::string >;

struct LinesGrammar : qi::grammar< Iterator, lines() >
{
    LinesGrammar();

    qi::rule< Iterator, std::string() > line_;
    qi::rule< Iterator, lines()       > start_;
}; // struct LinesGrammar
} // namespace sap
