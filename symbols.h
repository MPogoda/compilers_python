#pragma once
#include <boost/spirit/include/qi_symbols.hpp>

#include "lexeme.h"

namespace sap
{
namespace qi = boost::spirit::qi;

struct Symbols : qi::symbols< char, lex::symbol >
{
    Symbols();
}; // struct Symbols

struct ReservedWords : qi::symbols< char, lex::reserved_word >
{
    ReservedWords();
}; // struct ReservedWords

} // namespace sap
