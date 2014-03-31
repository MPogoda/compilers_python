#include "python_grammar.h"

#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_kleene.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_real.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "symbols.h"

namespace sap
{
static Symbols          symbols;
static ReservedWords    reserved_words;

PythonGrammar::PythonGrammar()
    : base_type( start )
{
    using qi::ascii::char_;
    using qi::ascii::alnum;
    using qi::ascii::alpha;
    using qi::labels::_val;
    using qi::labels::_1;
    using qi::lit;
    using qi::eps;
    using qi::double_;

    // match 4 spaces
    indent       = lit("    ");
    // calculate indent level
    indent_level = eps              [ _val = 0  ]
                // for every indent, increase _val
                >> *indent          [ _val += 1 ];
    // dummy, sets lex.type_ field
    indent_type  = eps              [ _val = lex::type::INDENT ];
    indent_rule %= indent_type
                >> indent_level;

    // dummy, sets lex.type_ field
    symbol_type  = eps          [ _val = lex::type::SYMBOL ];
    // match symbols against symbol-table
    symbol      %= symbols;
    symbol_rule %= symbol_type
                >> symbol;

    // dummy, sets lex.type_ field
    reserved_type    = eps              [ _val = lex::type::RESERVED ];
    // match against symbol table
    reserved        %= reserved_words;
    reserved_rule   %= reserved_type
                    >> reserved;

    // dummy, sets lex.type_ field
    identifier_type  = eps                  [ _val = lex::type::IDENTIFIER ];

    identifier      %=  (alpha | char_('_'))    // letter or _
                                                // followed by letter, number or _ undetermined number of times
                    >> *(alnum | char_('_'));
    identifier_rule  = identifier_type
                    >> identifier;

    constant_type    = eps                      [ _val = lex::type::CONST ];
    double_constant %= double_;
    // bool constant is either True or False
    bool_constant    = lit("True")              [ _val = true ]
                     | lit("False")             [ _val = false ];
    // string constant is any number of characters between " "
    string_constant %= lit("\"")
                    >> *(char_ - char_('"'))
                    >> lit("\"");
    // constant is either double, or bool, or string constant
    constant_rule   %= constant_type
                    >> ( double_constant
                       | bool_constant
                       | string_constant
                       );

    // matches all lexemes, that a visible (not indent & newline )
    // it's either reserved word, or constant, or identifier, or symbol
    visible_lexeme  %= reserved_rule
                    |  constant_rule
                    |  identifier_rule
                    |  symbol_rule
                    ;

    // Matches only indent, but doesn't set anything (it's type is void)
    empty_line       = indent_rule;
    // Matches typical line (indent, followed by visible_lexeme s )
    non_empty_line  %= indent_rule
                    >> +(   // one or more
                           *lit(" ")        // ignore spaces
                        >> visible_lexeme   // some visible_lexeme
                        >> *lit(" ")        // ignore spaces
                        );

    start   = non_empty_line    [ _val = _1 ]
            | empty_line;
}
} // namespace sap
