#include "lexeme.h"

#include <iostream>

namespace sap
{
const char* const lex::type_STRINGS[] = { "SYMBOL"
                                        , "INDENT"
                                        , "DEDENT"
                                        , "NEWLINE"
                                        , "Double const"
                                        , "Bool const"
                                        , "Str const"
                                        , "RESERVED"
                                        , "IDENTIFIER"
                                        , "RULE"
                                        , "Epsilon"
                                        }; // type_STRINGS
const char* const lex::symbol_STRINGS[] = { ","
                                          , "."
                                          , "("
                                          , ")"
                                          , "+"
                                          , "-"
                                          , "*"
                                          , "/"
                                          , "=="
                                          , "="
                                          , "<"
                                          , ">"
                                          , "!="
                                          , ":"
                                          }; // symbol_STRINGS
const char* const lex::reserved_STRINGS[] = { "def"
                                            , "class"
                                            , "if"
                                            , "else"
                                            , "while"
                                            , "break"
                                            , "return"
                                            , "print"
                                            , "input"
                                            }; // reserved_STRINGS
const char* const lex::rule_STRINGS[] = { "{applicable}"
                                        , "{assignment}"
                                        , "{class-decl}"
                                        , "{class-list}"
                                        , "{classes}"
                                        , "{comparator-eq}"
                                        , "{comparator-int}"
                                        , "{elseline}"
                                        , "{expr-int}"
                                        , "{fcall}"
                                        , "{ifline}"
                                        , "{input}"
                                        , "{logic-bool}"
                                        , "{logic-int}"
                                        , "{logic-str}"
                                        , "{logic}"
                                        , "{mcall}"
                                        , "{method-decl}"
                                        , "{method-list}"
                                        , "{methods}"
                                        , "{mparams}"
                                        , "{mparam-list}"
                                        , "{operand-int}"
                                        , "{operand-bool}"
                                        , "{operand-str}"
                                        , "{operator-int}"
                                        , "{param-list}"
                                        , "{params}"
                                        , "{rightside}"
                                        , "{sline-list}"
                                        , "{slines}"
                                        , "{sline}"
                                        , "{start}"
                                        , "{whileline}"
                                        , "{new_identifier}"
                                        , "{breakline}"
                                        , "{returnline}"
                                        , "{printline}"
                                        }; // rule_STRINGS

std::ostream& operator<<( std::ostream& out, lex::type rhs )
{
    out << lex::type_STRINGS[ static_cast< uint8_t >( rhs ) ];
    return out;
}

std::ostream& operator<<( std::ostream& out, lex::symbol rhs )
{
    out << lex::symbol_STRINGS[ static_cast< uint8_t >( rhs ) ];
    return out;
}

std::ostream& operator<<( std::ostream& out, lex::reserved_word rhs )
{
    out << lex::reserved_STRINGS[ static_cast< uint8_t >( rhs ) ];
    return out;
}

std::ostream& operator<<( std::ostream& out, lex::rule rhs )
{
    out << lex::rule_STRINGS[ static_cast< uint8_t >( rhs ) ];
    return out;
}

std::ostream& operator<<( std::ostream& out, lex rhs )
{
    out << "«"
        << "type: '"
        << rhs.type_
        << '\''
        << ", value: '"
        << rhs.value_
        << '\''
        << "»";
    return out;
}
} // namespace sap
