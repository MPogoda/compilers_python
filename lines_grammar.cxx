#include "lines_grammar.h"

#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_kleene.hpp>
#include <boost/spirit/include/qi_operator.hpp>

namespace sap
{
LinesGrammar::LinesGrammar()
    : base_type( start_ )
{
    using qi::eol;
    using qi::ascii::char_;

    line_  %= *(char_ - eol)
           >> eol;
    start_ %= *line_;
}
} // namespace sap
