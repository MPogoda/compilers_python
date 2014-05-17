#include "syntax.h"

namespace sap
{
void push_rule( Stack& st, const int rule_number )
{
    switch (rule_number) {
        default:
            assert( !"NO SUCH RULE!" );
    }
}

bool operator==( const lex& lhs, const lex& rhs )
{
    if (( lex::type::EPS == lhs.type_ ) || ( lex::type::EPS == rhs.type_ )) return true;
    if (lhs.type_ != rhs.type_) return false;

    switch (lhs.type_) {
        case lex::type::SYMBOL:
            return boost::get< lex::symbol >( lhs.value_ ) == boost::get< lex::symbol >( rhs.value_ );
        case lex::type::RESERVED:
            return boost::get< lex::reserved_word >( lhs.value_ ) == boost::get< lex::reserved_word >( rhs.value_);
        case lex::type::RULE:
            return boost::get< lex::rule >( lhs.value_ ) == boost::get< lex::rule >( rhs.value_ );
        case lex::type::INDENT:
        case lex::type::DEDENT:
        case lex::type::NEWLINE:
        case lex::type::D_CONST:
        case lex::type::B_CONST:
        case lex::type::S_CONST:
        case lex::type::IDENTIFIER:
            return true;
        default:
            assert( !"Wrong lexeme!");
    }
    return false;
}

Queue parse( const Table& table, LIterator begin, const LIterator end, Stack ss)
{
    Queue result;
    while (ss.size() > 1) {
        const bool atEnd = begin == end;

        if (ss.top().type_ == lex::type::COUNT ) {
            break;
        } else if (ss.top().type_ == lex::type::EPS ) {
            ss.pop();
        } else if (ss.top() == *begin) {
            ++begin;
            ss.pop();
        } else if (lex::type::RULE == ss.top().type_) {
            const auto table_top = table.find( boost::get< lex::rule >( ss.top().value_ ) );
            if (table.end() == table_top) throw std::logic_error{ "Cannot find rule!" };
            std::pair< MIterator, MIterator > eq;
            if (!atEnd) eq = table_top->second.equal_range( *begin );
            std::pair< MIterator, MIterator > eq_eps = table_top->second.equal_range( { lex::type::EPS, false } );

            int length{ 0 };
            if (!atEnd) length = std::distance( eq.first, eq.second );
            const int length_eps = std::distance( eq_eps.first, eq_eps.second );
            const int sum_length = length + length_eps;

            if (0 == sum_length) throw std::logic_error{ "asd" };
            ss.pop();

            if (1 == sum_length) {
                const uint rule = (length == 0) ? eq_eps.first->second : eq.first->second;

                result.push( rule );
                push_rule( ss, rule );
            } else { // length > 1
                if (!atEnd)
                for (; eq.second != eq.first; ++eq.first) {
                    Stack newSS{ ss };
                    const uint rule = eq.first->second;
                    push_rule( newSS, rule );
                    try {
                        Queue subQueue = parse( table, begin, end, std::move( newSS ) );
                        result.push( rule );

                        while (!subQueue.empty()) {
                            result.push( subQueue.front());
                            subQueue.pop();
                        }
                        return result;
                    } catch (std::logic_error& ex) {
                        continue;
                    }
                }

                for (; eq_eps.second != eq_eps.first; ++eq_eps.first) {
                    Stack newSS{ ss };
                    const uint rule = eq_eps.first->second;
                    push_rule( newSS, rule );
                    try {
                        Queue subQueue = parse( table, begin, end, std::move( newSS ) );
                        result.push( rule );
                        while (!subQueue.empty()) {
                            result.push( subQueue.front());
                            subQueue.pop();
                        }
                        return result;
                    } catch (std::logic_error& ex) {
                        continue;
                    }
                }


                throw std::logic_error{ "Cannot find rule" };
            }

        } else {
            throw std::logic_error{ "Cannot find rule!" };
        }
    }

    if (0 != std::distance( begin, end)) throw std::logic_error{ "Cannot perform syntax parsing!" };

    return result;
}

Table createTable()
{
    Table result;

    using map = std::unordered_multimap< lex, int >;
    // map tmp;

    return result;
}
} // namespace sap
