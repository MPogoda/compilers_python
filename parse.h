#pragma once
#include <string>
#include <deque>

namespace sap
{
struct lex;
using Container = std::string;
using Iterator  = Container::const_iterator;
using Lexems    = std::deque< sap::lex >;

Lexems parse( Container input );
} // namespace sap
