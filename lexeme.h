#pragma once

#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <iosfwd>

namespace sap
{
struct lex
{
    enum class type : uint8_t
        { SYMBOL
        , INDENT
        , NEWLINE
        , CONST
        , RESERVED
        , IDENTIFIER
        }; // enum class type

    enum class symbol : uint8_t
        { COMMA                 // ,
        , DOT                   // .
        , L_PARENTHESIS         // (
        , R_PARENTHESIS         // )
        , PLUS                  // +
        , MINUS                 // -
        , STAR                  // *
        , SLASH                 // /
        , EQUAL                 // =
        , LESS                  // <
        , GREATER               // >
        , NOT                   // !
        , COLON                 // :
        }; // enum class symbol

    enum class reserved_word : uint8_t
        { DEF
        , CLASS
        , IF
        , WHILE
        , BREAK
        , RETURN
        }; // enum class reserved_words

    type type_;

    typedef boost::variant< symbol, reserved_word, std::string, uint > value;
    value value_;
}; // struct lex

std::ostream& operator<<( std::ostream& out, lex::type          rhs );
std::ostream& operator<<( std::ostream& out, lex::symbol        rhs );
std::ostream& operator<<( std::ostream& out, lex::reserved_word rhs );
std::ostream& operator<<( std::ostream& out, lex                rhs );
} // namespace sap
BOOST_FUSION_ADAPT_STRUCT(
    sap::lex,
    (sap::lex::type, type_)
    (sap::lex::value, value_)
)
