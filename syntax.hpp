// enum class type : uint8_t
// { SYMBOL
// , CONST
// , RESERVED
// , IDENTIFIER
// }; // enum class type
//
// enum class symbol : uint8_t
// { COMA
// , DOT
// , OPENING_BRACE
// , CLOSING_BRACE
// , TWO_DOTS
// }; // enum class symbols
//
// enum class reserved_word : uint8_t
// { DEF
// , CLASS
// , IF
// , WHILE
// , BREAK
// , RETURN
// }; // enum class reserved_words
//
// struct lex
// {
//     type type_;
//     boost::variant< symbol, reserved_word, std::string > value_;
// }; // struct lex
