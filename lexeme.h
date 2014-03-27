#pragma once

namespace sap
{
struct lex
{
    enum class type : uint8_t
        { SYMBOL
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
        
            
        }; // enum class symbols

enum class reserved_word : uint8_t
{ DEF
, CLASS
, IF
, WHILE
, BREAK
, RETURN
}; // enum class reserved_words   
        
    type type_;
    
   // boost::variant< symbol, reserved_word, std::string > value_;
}; // struct lex

} // namespace sap