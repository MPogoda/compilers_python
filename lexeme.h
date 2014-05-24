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
        , DEDENT
        , NEWLINE
        , D_CONST
        , B_CONST
        , S_CONST
        , RESERVED
        , IDENTIFIER
        , RULE
        , EPS

        , COUNT
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

        , COUNT
        }; // enum class symbol

    enum class reserved_word : uint8_t
        { DEF
        , CLASS
        , IF
        , ELSE
        , WHILE
        , BREAK
        , RETURN
        , PRINT
        , INPUT

        , COUNT
        }; // enum class reserved_words

    enum class rule : uint8_t
        { APPLICABLE
        , ASSIGNMENT
        , CLASS_DECL
        , CLASS_LIST
        , CLASSES
        , COMPARATOR_EQ
        , COMPARATOR_INT
        , ELSELINE
        , EXPR_INT
        , FCALL
        , IFLINE
        , INPUT
        , LOGIC_BOOL
        , LOGIC_INT
        , LOGIC_STR
        , LOGIC
        , MCALL
        , METHOD_DECL
        , METHOD_LIST
        , METHODS
        , MPARAMS
        , MPARAM_LIST
        , OPERAND_INT
        , OPERAND_BOOL
        , OPERAND_STR
        , OPERATOR_INT
        , PARAM_LIST
        , PARAMS
        , RIGHTSIDE
        , SLINE_LIST
        , SLINES
        , SLINE
        , START
        , WHILELINE

        , IDENTIFIER

        , COUNT
        }; // enum class rule

    type type_;

    typedef boost::variant< rule, symbol, reserved_word, std::string, uint, double, bool > value;
    value value_;

    static const char* const type_STRINGS       [ static_cast< uint8_t >( lex::type::COUNT )            ];
    static const char* const symbol_STRINGS     [ static_cast< uint8_t >( lex::symbol::COUNT )          ];
    static const char* const reserved_STRINGS   [ static_cast< uint8_t >( lex::reserved_word::COUNT )   ];
    static const char* const rule_STRINGS   [ static_cast< uint8_t >( lex::rule::COUNT )   ];

}; // struct lex

std::ostream& operator<<( std::ostream& out, lex::type          rhs );
std::ostream& operator<<( std::ostream& out, lex::symbol        rhs );
std::ostream& operator<<( std::ostream& out, lex::reserved_word rhs );
std::ostream& operator<<( std::ostream& out, lex::rule rhs );
std::ostream& operator<<( std::ostream& out, lex                rhs );
} // namespace sap
BOOST_FUSION_ADAPT_STRUCT(
    sap::lex,
    (sap::lex::type, type_)
    (sap::lex::value, value_)
)
