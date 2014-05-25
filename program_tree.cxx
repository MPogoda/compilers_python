#include "program_tree.h"

#include <iostream>

namespace sap
{
namespace
{
class node_printer
{
public:
    void print( const node& root )
    {
        p_printLevel();

        out_ << root.rule_;
        if (const lex* lhs = boost::get< lex >( &root.value_ )) {
            out_ << " == " << *lhs << '\n';
        } else if (const node::nodes* nodes = boost::get< node::nodes >( &root.value_)) {
            ++level_;
            out_ << '\n';
            for (const auto& nd : *nodes ) {
                print( nd );
            }
            --level_;
        }
    }

    node_printer( std::ostream& i_out ) : out_( i_out ) { }
private:
    uint level_ = 0;
    std::ostream& out_;

    void p_printLevel() const
    {
        for (uint i = 0; level_ != i; ++i) {
            out_ << "> ";
        }
    }
};
// get leftside of rule
lex::rule getRule( const uint i_ruleNumber )
{
    switch (i_ruleNumber) {
        case 1:
        case 2:
            return lex::rule::APPLICABLE;
        case 4:
            return lex::rule::ASSIGNMENT;
        case 5:
            return lex::rule::CLASS_DECL;
        case 6:
        case 7:
            return lex::rule::CLASS_LIST;
        case 8:
            return lex::rule::CLASSES;
        case 9:
        case 10:
            return lex::rule::COMPARATOR_EQ;
        case 11:
        case 12:
        case 13:
        case 14:
            return lex::rule::COMPARATOR_INT;
        case 15:
        case 16:
            return lex::rule::ELSELINE;
        case 17:
        case 18:
            return lex::rule::EXPR_INT;
        case 19:
            return lex::rule::FCALL;
        case 20:
            return lex::rule::IFLINE;
        case 21:
            return lex::rule::INPUT;
        case 22:
        case 23:
            return lex::rule::LOGIC_BOOL;
        case 24:
            return lex::rule::LOGIC_INT;
        case 25:
            return lex::rule::LOGIC_STR;
        case 26:
        case 27:
        case 28:
            return lex::rule::LOGIC;
        case 29:
            return lex::rule::MCALL;
        case 30:
            return lex::rule::METHOD_DECL;
        case 31:
        case 32:
            return lex::rule::METHOD_LIST;
        case 33:
            return lex::rule::METHODS;
        case 34:
            return lex::rule::MPARAMS;
        case 35:
        case 36:
            return lex::rule::MPARAM_LIST;
        case 37:
        case 38:
            return lex::rule::OPERAND_INT;
        case 39:
        case 40:
            return lex::rule::OPERAND_BOOL;
        case 41:
        case 42:
            return lex::rule::OPERAND_STR;
        case 43:
        case 44:
        case 45:
        case 46:
            return lex::rule::OPERATOR_INT;
        case 47:
        case 48:
            return lex::rule::PARAM_LIST;
        case 49:
        case 50:
            return lex::rule::PARAMS;
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 68:
            return lex::rule::RIGHTSIDE;
        case 56:
        case 57:
            return lex::rule::SLINE_LIST;
        case 58:
            return lex::rule::SLINES;
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
            return lex::rule::SLINE;
        case 66:
            return lex::rule::START;
        case 67:
            return lex::rule::WHILELINE;
        default:
            assert(!"NO SUCH RULE!");
    }

    return lex::rule::COUNT;
}

// get number of children for this rule
int getCount( const uint i_ruleNumber )
{
    switch (i_ruleNumber) {
        case 1:
        case 2:
            return 1;
        case 4:
        case 5:
            return 2;
        case 6:
            return 1;
        case 7:
            return -1;
        case 8:
            return 2;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return 0;
        case 15:
            return 1;
        case 16:
            return -1;
        case 17:
            return 1;
        case 18:
            return 3;
        case 19:
            return 2;
        case 20:
            return 3;
        case 21:
            return -1;
        case 22:
            return 1;
        case 23:
        case 24:
        case 25:
            return 3;
        case 26:
        case 27:
        case 28:
            return 1;
        case 29:
            return 2;
        case 30:
            return 3;
        case 31:
            return -1;
        case 32:
            return 1;
        case 33:
            return 2;
            return 2;
        case 35:
            return 1;
        case 36:
            return -1;
        case 37:
        case 39:
        case 41:
            return 1;
        case 38:
        case 40:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
            return 0;
        case 47:
            return 2;
        case 48:
            return -1;
        case 49:
            return 2;
        case 50:
            return -1;
        case 51:
        case 52:
        case 53:
        case 54:
        case 68:
            return 1;
        case 55:
            return 0;
        case 56:
            return -1;
        case 57:
            return 1;
        case 58:
            return 2;
        case 59:
            return -1;
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
            return 1;
        case 66:
            return 2;
        case 67:
            return 2;
        case 69:
            return 0;
        default:
            assert(!"No such rule!");
    }

    return 0;
}

// should we skip lexeme
bool shouldSkip( const lex i_lhs ) {
    switch (i_lhs.type_) {
        case lex::type::SYMBOL:
            switch (boost::get< lex::symbol >( i_lhs.value_ )) {
                case lex::symbol::COMMA:
                case lex::symbol::DOT:
                case lex::symbol::L_PARENTHESIS:
                case lex::symbol::R_PARENTHESIS:
                case lex::symbol::COLON:
                    return true;
                case lex::symbol::PLUS:
                case lex::symbol::MINUS:
                case lex::symbol::STAR:
                case lex::symbol::SLASH:
                case lex::symbol::EQUAL:
                case lex::symbol::LESS:
                case lex::symbol::GREATER:
                case lex::symbol::NOT:
                    return false;
                default:
                    assert(!"No such symbol!");
            }
        case lex::type::D_CONST:
        case lex::type::B_CONST:
        case lex::type::S_CONST:
        case lex::type::IDENTIFIER:
            return false;
        case lex::type::INDENT:
        case lex::type::DEDENT:
        case lex::type::NEWLINE:
        case lex::type::RESERVED:
            return true;
        default:
            assert(!"No such lexeme!");
    }

    return false;
}

// skip lexemes until found the one that shouldn't be skipped and return it
lex skip( LIterator& i_it ) {
    while (shouldSkip( *i_it ))
        ++i_it;

    return *i_it++;
}

} // namespace
std::ostream& operator<<( std::ostream& out, const node& root)
{
    node_printer{ out }.print( root );
    return out;
}

node::node( Queue& i_queue, LIterator& i_it )
{
    assert(!i_queue.empty());

    const auto rule = i_queue.front();
    i_queue.pop();

    rule_ = getRule( rule );

    const auto N = getCount( rule );
    if (0 > N) {
        value_ = lex{ lex::type::EPS, false };
    } else if (0 == N) {
        value_ = skip( i_it );

        switch( rule) {
            case 9:
            case 10:
            case 11:
            case 13:
                // skip second EQUAL sign in comparation operator
                assert( lex::symbol::EQUAL == boost::get< lex::symbol >( skip( i_it ).value_) );
                break;
            default:
                break;
                //noop
        }
    } else {
        auto nodes = node::nodes( );
        nodes.reserve( N );

        for (int i = 0; N != i; ++i) {
            nodes.emplace_back( node{ i_queue, i_it } );
        }

        value_ = nodes;
    }
}

} // namespace sap
