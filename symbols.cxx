#include "symbols.h"

namespace sap
{
Symbols::Symbols()
{
    using symbol = lex::symbol;
    // assert enumerations are continious.
    // because i've created them
    constexpr uint8_t MAX{ static_cast< uint8_t >( symbol::COUNT ) };
    for ( uint8_t s = 0; s != MAX; ++s) {
        add( lex::symbol_STRINGS[ s ], static_cast< symbol >( s ) );
    }
}

ReservedWords::ReservedWords()
{
    using rword = lex::reserved_word;
    constexpr uint8_t MAX{ static_cast< uint8_t >( rword::COUNT ) };
    for ( uint8_t s = 0; s != MAX; ++s ) {
        add( lex::reserved_STRINGS[ s ], static_cast< rword >( s ) );
    }
}
} // namespace sap
