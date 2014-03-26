#include "grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace sap
{
MyGrammar::MyGrammar()
    : base_type( indent_level )
{
    using qi::ascii::char_;
    using namespace qi::labels;
    using qi::eps;

    indent = char_(' ') >> char_(' ') >> char_(' ') >> char_(' ');
    indent_level = eps              [ _val = 0  ]
                >> *indent          [ _val += 1 ];
}
}
