#pragma once

#include <boost/functional/hash.hpp>

#include "lexeme.h"
#include "parse.h"

#include <unordered_map>
#include <stack>
#include <queue>

namespace std
{
template <>
struct hash< sap::lex >
{
    size_t operator()( const sap::lex& lhs ) const
    {
        using pair = std::pair< sap::lex::type, boost::variant< sap::lex::symbol
                                                              , sap::lex::reserved_word
                                                              , sap::lex::rule
                                                              , uint
                                                              >

                              >;

        boost::hash< pair > h{};
        switch (lhs.type_) {
            case sap::lex::type::SYMBOL:
                return h( { lhs.type_, boost::get< sap::lex::symbol >( lhs.value_ ) } );
            case sap::lex::type::RESERVED:
                return h( { lhs.type_, boost::get< sap::lex::reserved_word >( lhs.value_ ) } );
            case sap::lex::type::RULE:
                return h( { lhs.type_, boost::get< sap::lex::rule >( lhs.value_ ) } );
            default:
                return h( { lhs.type_, 0u } );
        }
    }
};
} // namespace std

namespace sap
{
using Table = std::unordered_map< lex::rule, std::unordered_multimap< lex, int >, boost::hash< lex::rule > >;
using MIterator = std::unordered_multimap< lex, int >::const_iterator;
using Stack = std::stack< lex >;
using LIterator = Lexems::const_iterator;
using Queue = std::queue< uint >;

void push_rule( Stack& st, const int rule_number );

Queue parse( const Table& table, LIterator begin, const LIterator end, Stack ss, uint newline = 0 );
Table createTable();
} // namespace sap
