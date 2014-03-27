#include "grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace sap
{
MyGrammar::MyGrammar()
    : base_type( start )
{
    using qi::ascii::char_;
    using qi::lit;
    using namespace qi::ascii;
    using namespace qi::labels;
    using qi::eps;
    using qi::no_skip;

    indent       = lit("    ");
    indent_level = eps              [ _val = 0  ]
                >> *indent          [ _val += 1 ];
    identifier %=  (alpha | char_('_')) >> *(alnum | char_('_'));
    identifiers %= identifier % char_(' ');
    
    start       %= indent_level
                >> identifiers;
}
}
