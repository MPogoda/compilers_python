#include "syntax.h"

namespace sap
{
void push_rule( Stack& st, const int rule_number )
{
    switch (rule_number) {
        case 1:
            st.push( { lex::type::RULE, lex::rule::APPLICABLE } );
            break;
        case 2:
            st.push( { lex::type::IDENTIFIER, "" } );
            break;
        case 3:
            st.push( { lex::type::RULE, lex::rule::MCALL } );
            break;
        case 4:
            st.push( { lex::type::RULE, lex::rule::RIGHTSIDE } );
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            st.push( { lex::type::IDENTIFIER, "" } );
            break;
        case 5:
            st.push( { lex::type::DEDENT, 0u } );
            st.push( { lex::type::RULE, lex::rule::METHODS } );
            st.push( { lex::type::INDENT, 0u } );
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::SYMBOL, lex::symbol::COLON } );
            st.push( { lex::type::IDENTIFIER, "" } );
            st.push( { lex::type::RESERVED, lex::reserved_word::CLASS } );
            break;
        case 6:
            st.push( { lex::type::RULE, lex::rule::CLASSES } );
            break;
        case 7:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 8:
            st.push( { lex::type::RULE, lex::rule::CLASS_LIST } );
            st.push( { lex::type::RULE, lex::rule::CLASS_DECL } );
            break;
        case 9:
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            st.push( { lex::type::SYMBOL, lex::symbol::NOT } );
            break;
        case 10:
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            break;
        case 11:
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            st.push( { lex::type::SYMBOL, lex::symbol::NOT } );
            break;
        case 12:
            st.push( { lex::type::SYMBOL, lex::symbol::LESS } );
            break;
        case 13:
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            break;
        case 14:
            st.push( { lex::type::SYMBOL, lex::symbol::GREATER } );
            break;
        case 15:
            st.push( { lex::type::DEDENT, 0u } );
            st.push( { lex::type::RULE, lex::rule::SLINES } );
            st.push( { lex::type::INDENT, 0u } );
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::SYMBOL, lex::symbol::COLON } );
            st.push( { lex::type::RESERVED, lex::reserved_word::ELSE } );
            break;
        case 16:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 17:
            st.push( { lex::type::RULE, lex::rule::OPERAND_INT } );
            break;
        case 18:
            st.push( { lex::type::RULE, lex::rule::OPERAND_INT } );
            st.push( { lex::type::RULE, lex::rule::OPERATOR_INT } );
            st.push( { lex::type::RULE, lex::rule::OPERAND_INT } );
            break;
        case 19:
            st.push( { lex::type::SYMBOL, lex::symbol::R_PARENTHESIS } );
            st.push( { lex::type::RULE, lex::rule::PARAMS } );
            st.push( { lex::type::SYMBOL, lex::symbol::L_PARENTHESIS } );
            st.push( { lex::type::IDENTIFIER, "" } );
            break;
        case 20:
            st.push( { lex::type::RULE, lex::rule::ELSELINE } );
            st.push( { lex::type::DEDENT, 0u } );
            st.push( { lex::type::RULE, lex::rule::SLINES } );
            st.push( { lex::type::INDENT, 0u } );
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::SYMBOL, lex::symbol::COLON } );
            st.push( { lex::type::RULE, lex::rule::LOGIC } );
            st.push( { lex::type::RESERVED, lex::reserved_word::IF } );
            break;
        case 21:
            st.push( { lex::type::SYMBOL, lex::symbol::R_PARENTHESIS } );
            st.push( { lex::type::SYMBOL, lex::symbol::L_PARENTHESIS } );
            st.push( { lex::type::RESERVED, lex::reserved_word::INPUT } );
            break;
        case 22:
            st.push( { lex::type::RULE, lex::rule::OPERAND_BOOL } );
            break;
        case 23:
            st.push( { lex::type::RULE, lex::rule::OPERAND_BOOL } );
            st.push( { lex::type::RULE, lex::rule::COMPARATOR_EQ } );
            st.push( { lex::type::RULE, lex::rule::OPERAND_BOOL } );
            break;
        case 24:
            st.push( { lex::type::RULE, lex::rule::OPERAND_INT } );
            st.push( { lex::type::RULE, lex::rule::COMPARATOR_INT } );
            st.push( { lex::type::RULE, lex::rule::OPERAND_INT } );
            break;
        case 25:
            st.push( { lex::type::RULE, lex::rule::OPERAND_STR } );
            st.push( { lex::type::RULE, lex::rule::COMPARATOR_EQ } );
            st.push( { lex::type::RULE, lex::rule::OPERAND_STR } );
            break;
        case 26:
            st.push( { lex::type::RULE, lex::rule::LOGIC_BOOL } );
            break;
        case 27:
            st.push( { lex::type::RULE, lex::rule::LOGIC_INT } );
            break;
        case 28:
            st.push( { lex::type::RULE, lex::rule::LOGIC_STR } );
            break;
        case 29:
            st.push( { lex::type::RULE, lex::rule::FCALL } );
            st.push( { lex::type::SYMBOL, lex::symbol::DOT } );
            st.push( { lex::type::RULE, lex::rule::APPLICABLE } );
            break;
        case 30:
            st.push( { lex::type::DEDENT, 0u } );
            st.push( { lex::type::RULE, lex::rule::SLINES } );
            st.push( { lex::type::INDENT, 0u } );
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::SYMBOL, lex::symbol::COLON } );
            st.push( { lex::type::SYMBOL, lex::symbol::R_PARENTHESIS } );
            st.push( { lex::type::RULE, lex::rule::MPARAMS } );
            st.push( { lex::type::SYMBOL, lex::symbol::L_PARENTHESIS } );
            st.push( { lex::type::IDENTIFIER, "" } );
            st.push( { lex::type::RESERVED, lex::reserved_word::DEF } );
            break;
        case 31:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 32:
            st.push( { lex::type::RULE, lex::rule::METHODS } );
            break;
        case 33:
            st.push( { lex::type::RULE, lex::rule::METHOD_LIST } );
            st.push( { lex::type::RULE, lex::rule::METHOD_DECL } );
            break;
        case 34:
            st.push( { lex::type::RULE, lex::rule::MPARAM_LIST } );
            st.push( { lex::type::IDENTIFIER, "" } );
            break;
        case 35:
            st.push( { lex::type::RULE, lex::rule::MPARAMS } );
            st.push( { lex::type::SYMBOL, lex::symbol::COMMA } );
            break;
        case 36:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 37:
            st.push( { lex::type::IDENTIFIER, "" } );
            break;
        case 38:
            st.push( { lex::type::D_CONST, 0u } );
            break;
        case 39:
            st.push( { lex::type::IDENTIFIER, "" } );
            break;
        case 40:
            st.push( { lex::type::B_CONST, false } );
            break;
        case 41:
            st.push( { lex::type::IDENTIFIER, "" } );
            break;
        case 42:
            st.push( { lex::type::S_CONST, 0u } );
            break;
        case 43:
            st.push( { lex::type::SYMBOL, lex::symbol::STAR } );
            break;
        case 44:
            st.push( { lex::type::SYMBOL, lex::symbol::PLUS } );
            break;
        case 45:
            st.push( { lex::type::SYMBOL, lex::symbol::MINUS } );
            break;
        case 46:
            st.push( { lex::type::SYMBOL, lex::symbol::SLASH } );
            break;
        case 47:
            st.push( { lex::type::RULE, lex::rule::PARAM_LIST } );
            st.push( { lex::type::RULE, lex::rule::RIGHTSIDE } );
            st.push( { lex::type::SYMBOL, lex::symbol::COMMA } );
            break;
        case 48:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 49:
            st.push( { lex::type::RULE, lex::rule::PARAM_LIST } );
            st.push( { lex::type::RULE, lex::rule::RIGHTSIDE } );
            break;
        case 50:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 51:
            st.push( { lex::type::RULE, lex::rule::EXPR_INT } );
            break;
        case 52:
            st.push( { lex::type::RULE, lex::rule::INPUT } );
            break;
        case 53:
            st.push( { lex::type::RULE, lex::rule::LOGIC } );
            break;
        case 54:
            st.push( { lex::type::RULE, lex::rule::MCALL } );
            break;
        case 55:
            st.push( { lex::type::S_CONST, "" } );
            break;
        case 56:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 57:
            st.push( { lex::type::RULE, lex::rule::SLINES } );
            break;
        case 58:
            st.push( { lex::type::RULE, lex::rule::SLINE_LIST } );
            st.push( { lex::type::RULE, lex::rule::SLINE } );
            break;
        case 59:
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::RESERVED, lex::reserved_word::BREAK } );
            break;
        case 60:
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::RULE, lex::rule::RIGHTSIDE } );
            st.push( { lex::type::RESERVED, lex::reserved_word::RETURN } );
            break;
        case 61:
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::SYMBOL, lex::symbol::R_PARENTHESIS } );
            st.push( { lex::type::RULE, lex::rule::RIGHTSIDE } );
            st.push( { lex::type::SYMBOL, lex::symbol::L_PARENTHESIS } );
            st.push( { lex::type::RESERVED, lex::reserved_word::PRINT } );
            break;
        case 62:
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::RULE, lex::rule::ASSIGNMENT } );
            break;
        case 63:
            st.push( { lex::type::RULE, lex::rule::IFLINE } );
            break;
        case 64:
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::RULE, lex::rule::MCALL } );
            break;
        case 65:
            st.push( { lex::type::RULE, lex::rule::WHILELINE } );
            break;
        case 66:
            st.push( { lex::type::DEDENT, 0u } );
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::RULE, lex::rule::MCALL } );
            st.push( { lex::type::RULE, lex::rule::CLASSES } );
            st.push( { lex::type::INDENT, 0u } );
            break;
        case 67:
            st.push( { lex::type::DEDENT, 0u } );
            st.push( { lex::type::RULE, lex::rule::SLINES } );
            st.push( { lex::type::INDENT, 0u } );
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::SYMBOL, lex::symbol::COLON } );
            st.push( { lex::type::RULE, lex::rule::LOGIC } );
            st.push( { lex::type::RESERVED, lex::reserved_word::WHILE } );
            break;

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
