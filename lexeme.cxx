#include "lexeme.h"

#include <iostream>

namespace sap
{
static constexpr const char* const type_STRINGS[] = { "SYMBOL"
                                                    , "CONST"
                                                    , "RESERVED"
                                                    , "IDENTIFIER"
                                                    }; // type_STRINGS
static constexpr const char* const symbol_STRINGS[] = { ","
                                                      , "."
                                                      , "("
                                                      , ")"
                                                      , "+"
                                                      , "-"
                                                      , "*"
                                                      , "/"
                                                      , "="
                                                      , "<"
                                                      , ">"
                                                      , "!"
                                                      , ":"
                                                      }; // symbol_STRINGS
static constexpr const char* const reserved_STRINGS[] = { "DEF"
                                                        , "CLASS"
                                                        , "IF"
                                                        , "WHILE"
                                                        , "BREAK"
                                                        , "RETURN"
                                                        }; // reserved_STRINGS

std::ostream& operator<<( std::ostream& out, lex::type rhs )
{
    out << type_STRINGS[ static_cast< uint8_t >( rhs ) ];
    return out;
}

std::ostream& operator<<( std::ostream& out, lex::symbol rhs )
{
    out << symbol_STRINGS[ static_cast< uint8_t >( rhs ) ];
    return out;
}

std::ostream& operator<<( std::ostream& out, lex::reserved_word rhs )
{
    out << reserved_STRINGS[ static_cast< uint8_t >( rhs ) ];
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
