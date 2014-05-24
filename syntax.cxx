#include "syntax.h"

#include <sstream>

#ifdef NDEBUG
    #define DEBUG(a)
    #define DBG(a)
#else
    #include <iostream>
    #define DEBUG(a) std::cout << #a << " = " << (a) << '\n'
    #define DBG(a) std::cout << __LINE__ << " : " << a << '\n';
#endif
namespace sap
{
void push_rule( Stack& st, const int rule_number )
{
    DEBUG( rule_number );
    switch (rule_number) {
        case 1:
            st.push( { lex::type::RULE, lex::rule::FCALL } );
            break;
        case 2:
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER } );
            break;
        // case 3:
        //     st.push( { lex::type::RULE, lex::rule::MCALL } );
        //     break;
        case 4:
            st.push( { lex::type::RULE, lex::rule::RIGHTSIDE } );
            st.push( { lex::type::SYMBOL, lex::symbol::EQUAL } );
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
            break;
        case 5:
            st.push( { lex::type::DEDENT, 0u } );
            st.push( { lex::type::RULE, lex::rule::METHODS } );
            st.push( { lex::type::INDENT, 0u } );
            st.push( { lex::type::NEWLINE, 0u } );
            st.push( { lex::type::SYMBOL, lex::symbol::COLON } );
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
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
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
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
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
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
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
            break;
        case 35:
            st.push( { lex::type::RULE, lex::rule::MPARAMS } );
            st.push( { lex::type::SYMBOL, lex::symbol::COMMA } );
            break;
        case 36:
            st.push( { lex::type::EPS, 0u } );
            break;
        case 37:
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
            break;
        case 38:
            st.push( { lex::type::D_CONST, 0u } );
            break;
        case 39:
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
            break;
        case 40:
            st.push( { lex::type::B_CONST, false } );
            break;
        case 41:
            st.push( { lex::type::RULE, lex::rule::IDENTIFIER} );
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
        case 68:
            st.push( { lex::type::RULE, lex::rule::FCALL } );
            break;
        case 69:
            st.push( { lex::type::IDENTIFIER, "" } );

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

std::string builtError( const uint newline )
{
    std::stringstream ss;
    ss << "Syntax error at ";
    ss << newline << " line!.";

    return ss.str();
}

Queue parse( const Table& table, LIterator begin, const LIterator end, Stack ss, uint newline )
{
    Queue result;
    while (ss.size() > 1) {
        const bool atEnd = begin == end;
        if (!atEnd) DEBUG( *begin );
        DEBUG( ss.top() );

        if (ss.top().type_ == lex::type::COUNT ) {
            break;
        } else if (ss.top().type_ == lex::type::EPS ) {
            ss.pop();
        } else if (ss.top() == *begin) {
            if (ss.top().type_ == lex::type::NEWLINE) {
                newline = boost::get< uint >( begin->value_ );
            }

            ++begin;
            ss.pop();
        } else if (lex::type::RULE == ss.top().type_) {
            const auto table_top = table.find( boost::get< lex::rule >( ss.top().value_ ) );

            if (table.end() == table_top) throw std::logic_error{ builtError( newline ) };
            std::pair< MIterator, MIterator > eq;
            if (!atEnd) eq = table_top->second.equal_range( *begin );
            std::pair< MIterator, MIterator > eq_eps = table_top->second.equal_range( { lex::type::EPS, false } );

            int length{ 0 };
            if (!atEnd) length = std::distance( eq.first, eq.second );
            const int length_eps = std::distance( eq_eps.first, eq_eps.second );
            const int sum_length = length + length_eps;

            if (0 == sum_length) throw std::logic_error{ builtError( newline ) };
            ss.pop();

            if (1 == sum_length) {
                const uint rule = (length == 0) ? eq_eps.first->second : eq.first->second;

                result.push( rule );
                push_rule( ss, rule );
            } else { // length > 1
                DBG( "TRYING" );
                if (!atEnd)
                for (; eq.second != eq.first; ++eq.first) {
                    DEBUG( std::distance( eq.first, eq.second ) );
                    Stack newSS{ ss };
                    const uint rule = eq.first->second;
                    push_rule( newSS, rule );
                    try {
                        Queue subQueue = parse( table, begin, end, std::move( newSS ), newline );
                        result.push( rule );

                        while (!subQueue.empty()) {
                            result.push( subQueue.front());
                            subQueue.pop();
                        }
                        return result;
                    } catch (std::logic_error& ex) {
                        DBG( "FAILED" );
                        DEBUG( rule );
                        continue;
                    }
                }

                for (; eq_eps.second != eq_eps.first; ++eq_eps.first) {
                    Stack newSS{ ss };
                    const uint rule = eq_eps.first->second;
                    push_rule( newSS, rule );
                    try {
                        Queue subQueue = parse( table, begin, end, std::move( newSS ), newline);
                        result.push( rule );
                        while (!subQueue.empty()) {
                            result.push( subQueue.front());
                            subQueue.pop();
                        }
                        return result;
                    } catch (std::logic_error& ex) {
                        DBG( "FAILED" );
                        DEBUG( rule );
                        continue;
                    }
                }


                throw std::logic_error{ builtError( newline ) };
            }

        } else {
            throw std::logic_error{ builtError( newline ) };
        }
    }

    if (0 != std::distance( begin, end)) throw std::logic_error{ "Cannot perform syntax parsing!" };

    return result;
}

Table createTable()
{
    Table result;

    using map = std::unordered_multimap< lex, int >;
    map tmp;

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 1 }
                , { { lex::type::IDENTIFIER, "" }, 2 }
                // , { { lex::type::IDENTIFIER, "" }, 3 }
                } );
    result.insert( { lex::rule::APPLICABLE, tmp } );
    tmp.clear();

    tmp.insert( { { lex::type::IDENTIFIER, "" }, 4 } );
    result.insert( { lex::rule::ASSIGNMENT, tmp } );
    tmp.clear();

    tmp.insert( { { lex::type::RESERVED, lex::reserved_word::CLASS }, 5 } );
    result.insert( { lex::rule::CLASS_DECL, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::RESERVED, lex::reserved_word::CLASS }, 6 }
                , { { lex::type::EPS, 0u }, 7 } } );
    result.insert( { lex::rule::CLASS_LIST, tmp } );
    tmp.clear();

    tmp.insert( { { lex::type::RESERVED, lex::reserved_word::CLASS }, 8 } );
    result.insert( { lex::rule::CLASSES, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::SYMBOL, lex::symbol::NOT }, 9 }
                , { { lex::type::SYMBOL, lex::symbol::EQUAL }, 10 } } );
    result.insert( { lex::rule::COMPARATOR_EQ, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::SYMBOL, lex::symbol::NOT }, 11 }
                , { { lex::type::SYMBOL, lex::symbol::LESS }, 12 }
                , { { lex::type::SYMBOL, lex::symbol::EQUAL }, 13 }
                , { { lex::type::SYMBOL, lex::symbol::GREATER }, 14 }
                } );
    result.insert( { lex::rule::COMPARATOR_INT, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::RESERVED, lex::reserved_word::ELSE }, 15 }
                , { { lex::type::EPS, 0u }, 16 }
                } );
    result.insert( { lex::rule::ELSELINE, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 17 }
                , { { lex::type::IDENTIFIER, "" }, 18 }
                , { { lex::type::D_CONST, "" }, 17 }
                , { { lex::type::D_CONST, "" }, 18 }
                } );
    result.insert( { lex::rule::EXPR_INT, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::IDENTIFIER, "" }, 19 }
                 );
    result.insert( { lex::rule::FCALL, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::RESERVED, lex::reserved_word::IF }, 20 }
                 );
    result.insert( { lex::rule::IFLINE, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::RESERVED, lex::reserved_word::INPUT }, 21 }
                 );
    result.insert( { lex::rule::INPUT, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 22 }
                , { { lex::type::IDENTIFIER, "" }, 23 }
                , { { lex::type::B_CONST, "" }, 22 }
                , { { lex::type::B_CONST, "" }, 23 }
                } );
    result.insert( { lex::rule::LOGIC_BOOL, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 24 }
                , { { lex::type::D_CONST, "" }, 24 }
                } );
    result.insert( { lex::rule::LOGIC_INT, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 25 }
                , { { lex::type::S_CONST, "" }, 25 }
                } );
    result.insert( { lex::rule::LOGIC_STR, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 26 }
                , { { lex::type::IDENTIFIER, "" }, 27 }
                , { { lex::type::IDENTIFIER, "" }, 28 }
                , { { lex::type::B_CONST, "" }, 26 }
                , { { lex::type::D_CONST, "" }, 27 }
                , { { lex::type::S_CONST, "" }, 28 }
                } );
    result.insert( { lex::rule::LOGIC, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::IDENTIFIER, "" }, 29 }
                 );
    result.insert( { lex::rule::MCALL, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::RESERVED, lex::reserved_word::DEF }, 30 }
                 );
    result.insert( { lex::rule::METHOD_DECL, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::RESERVED, lex::reserved_word::DEF }, 32 }
                , { { lex::type::EPS, 0u }, 31 }
                } );
    result.insert( { lex::rule::METHOD_LIST, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::RESERVED, lex::reserved_word::DEF }, 33 }
                 );
    result.insert( { lex::rule::METHODS, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::IDENTIFIER, "" }, 34 }
                 );
    result.insert( { lex::rule::MPARAMS, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::SYMBOL, lex::symbol::COMMA }, 35 }
                , { { lex::type::EPS, 0u }, 36 }
                } );
    result.insert( { lex::rule::MPARAM_LIST, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 37 }
                , { { lex::type::D_CONST, 0u }, 38 }
                } );
    result.insert( { lex::rule::OPERAND_INT, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 39 }
                , { { lex::type::B_CONST, false }, 40 }
                } );
    result.insert( { lex::rule::OPERAND_BOOL, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 41 }
                , { { lex::type::S_CONST, "" }, 42 }
                } );
    result.insert( { lex::rule::OPERAND_STR, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::SYMBOL, lex::symbol::STAR }, 43 }
                , { { lex::type::SYMBOL, lex::symbol::PLUS }, 44 }
                , { { lex::type::SYMBOL, lex::symbol::MINUS }, 45 }
                , { { lex::type::SYMBOL, lex::symbol::SLASH }, 46 }
                } );
    result.insert( { lex::rule::OPERATOR_INT, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::SYMBOL, lex::symbol::COMMA }, 47 }
                , { { lex::type::EPS, 0u }, 48 }
                } );
    result.insert( { lex::rule::PARAM_LIST, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 49 }
                , { { lex::type::D_CONST, 0u }, 49 }
                , { { lex::type::S_CONST, 0u }, 49 }
                , { { lex::type::B_CONST, 0u }, 49 }
                , { { lex::type::RESERVED, lex::reserved_word::INPUT }, 49 }
                , { { lex::type::EPS, 0u }, 50 }
                } );
    result.insert( { lex::rule::PARAMS, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 51 }
                , { { lex::type::IDENTIFIER, "" }, 53 }
                , { { lex::type::IDENTIFIER, "" }, 54 }
                , { { lex::type::IDENTIFIER, "" }, 68 }
                , { { lex::type::D_CONST, 0u }, 51 }
                , { { lex::type::D_CONST, 0u }, 53 }
                , { { lex::type::RESERVED, lex::reserved_word::INPUT }, 52 }
                , { { lex::type::B_CONST, 0u }, 53 }
                , { { lex::type::S_CONST, 0u }, 53 }
                , { { lex::type::S_CONST, 0u }, 55 }
                } );
    result.insert( { lex::rule::RIGHTSIDE, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 57 }
                , { { lex::type::RESERVED, lex::reserved_word::PRINT }, 57 }
                , { { lex::type::RESERVED, lex::reserved_word::BREAK }, 57 }
                , { { lex::type::RESERVED, lex::reserved_word::RETURN }, 57 }
                , { { lex::type::RESERVED, lex::reserved_word::IF }, 57 }
                , { { lex::type::RESERVED, lex::reserved_word::WHILE }, 57 }
                , { { lex::type::EPS, "" }, 56 }
                } );
    result.insert( { lex::rule::SLINE_LIST, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 58 }
                , { { lex::type::RESERVED, lex::reserved_word::PRINT }, 58 }
                , { { lex::type::RESERVED, lex::reserved_word::BREAK }, 58 }
                , { { lex::type::RESERVED, lex::reserved_word::RETURN }, 58 }
                , { { lex::type::RESERVED, lex::reserved_word::IF }, 58 }
                , { { lex::type::RESERVED, lex::reserved_word::WHILE }, 58 }
                } );
    result.insert( { lex::rule::SLINES, tmp } );
    tmp.clear();

    tmp.insert( { { { lex::type::IDENTIFIER, "" }, 62 }
                , { { lex::type::IDENTIFIER, "" }, 64 }
                , { { lex::type::RESERVED, lex::reserved_word::PRINT }, 61 }
                , { { lex::type::RESERVED, lex::reserved_word::BREAK }, 59 }
                , { { lex::type::RESERVED, lex::reserved_word::RETURN }, 60 }
                , { { lex::type::RESERVED, lex::reserved_word::IF }, 63 }
                , { { lex::type::RESERVED, lex::reserved_word::WHILE }, 65 }
                } );
    result.insert( { lex::rule::SLINE, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::INDENT, 0u }, 66 }
                 );
    result.insert( { lex::rule::START, tmp } );
    tmp.clear();

    tmp.insert(  { { lex::type::RESERVED, lex::reserved_word::WHILE }, 67 }
                 );
    result.insert( { lex::rule::WHILELINE, tmp } );
    tmp.clear();

    tmp.insert( { { lex::type::IDENTIFIER, "" }, 69 } );
    result.insert( { lex::rule::IDENTIFIER, tmp } );

    return result;
}
} // namespace sap
