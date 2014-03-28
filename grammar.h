#pragma once
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>

#include <string>

#include <boost/fusion/include/adapt_struct.hpp>

namespace sap
{
namespace qi = boost::spirit::qi;

struct r
{
    uint32_t indent;
    std::vector< std::string > identifiers;
};
} // namepspace sap

BOOST_FUSION_ADAPT_STRUCT(
    sap::r,
    (uint32_t, indent)
    (std::vector< std::string >, identifiers)
)

namespace sap
{
using Iterator = std::string::const_iterator;
using Return   = r;

class MyGrammar : public qi::grammar< Iterator, Return() >
{
public:
    qi::rule< Iterator, void() >        indent;
    qi::rule< Iterator, uint32_t() >    indent_level;
    qi::rule< Iterator, std::string() > identifier;
    qi::rule< Iterator, std::vector< std::string >() > identifiers;

    qi::rule< Iterator, Return() >      start;

    MyGrammar( );
}; // class MyGrammar

} // namespace sap
