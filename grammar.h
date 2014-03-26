#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>

#include <string>

namespace sap
{
namespace qi = boost::spirit::qi;

using Iterator = std::string::const_iterator;
using Return   = uint32_t;

class MyGrammar : public qi::grammar< Iterator, Return() >
{
public:
    qi::rule< Iterator, void() >        indent;
    qi::rule< Iterator, uint32_t() >    indent_level;

    MyGrammar( );
}; // class MyGrammar

} // namespace sap
