#include <boost/variant.hpp>

//  Class:
//      has name
//      has class_used_names
//      has method_list
//      Constructor( node, used_names ):
//          ASSERT that node has rule CLASS_DECL
//          Get class name from it's first child
//          ASSERT that this name isn't used
//
//          Create class-used-names
//          Populate it with used_names + class_name
//
//          Iterate over all other children (should be 1+ because of grammar):
//              (maybe don't go into last node because it should be EPS)
//              Create Method from (node, class_used_names)
//              Add method->name to class_used_names
//              Add Method to method_list
//
//  Method:
//      has name
//      has method_used_names
//      has statements
//      Constructor( node, class_used_names ):
//          ASSERT that node is METHOD_DECL
//          Get method name from first child
//          ASSERT method name isn't used
//
//          method_used_names = class_used_names + method_name
//
//          Get method parameters from second child
//          Iterate over parameters:
//              ASSERT that paramter_name isn't used
//              method_used_names += parameter_name
//
//          Go to the last child: SLINES
//          Iteratate over all its children:
//              switch rule:
//                  break: REPORT ERROR
//                  return: create RETURN( node, method_used_names (const)), expect no more rules
//                  print: create PRINT( node, method_used_names (const))
//                  assignment: create ASSIGNMENT( node, method_used_names (non-const))
//                  ifline: create IF_STMT( node, method_used_names (const))
//                  whileline: create while_stmt( node, method_used_names (const))
//                  mcall: create METHOD_CALL( node, method_used_names
//

#include <vector>

#include "parse.h"
#include "lexeme.h"
#include "syntax.h"

    #include <iostream>
#ifdef NDEBUG
    #define DEBUG(a)
    #define DBG(a)
#else
    #include <iostream>
    #define DEBUG_MSG(a) std::cout << #a << " = " << (a) << '\n'
    #define DBG(a) std::cout << __LINE__ << " : " << a << '\n';
#endif
namespace sap
{
struct node
{
    lex::rule rule_;
    using nodes = std::vector< node >;
    using value = boost::variant< lex, nodes >;
    value value_;
};

struct node_printer
{
    uint level = 0;
    void printLevel() const {
        for (uint i = 0; level != i; ++i) {
            std::cout << "> ";
        }
    }
    void print( const node root ) {
        printLevel();
        std::cout << root.rule_;
        if (const lex* lhs = boost::get< lex >( &root.value_ )) {
            std::cout << " == " << *lhs << '\n';
        } else {
            const node::nodes* nodes = boost::get< node::nodes >( &root.value_);
            ++level;
            std::cout <<'\n';
            for (const auto& nd : *nodes ) {
                print( nd );
            }
            --level;
        }
    }
};

lex::rule getRule( const uint rule_number ) {
    switch (rule_number) {
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
            assert(!"FAIL!");
    }

    return lex::rule::COUNT;
}

int getCount( const uint rule_number ) {
    switch (rule_number) {
        case 1:
            return 1;
        case 2:
            return 1;
        case 4:
            return 2;
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
        case 34:
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
            assert(!"FAIL!");
    }

    return 0;
}

bool shouldSkip( const lex lhs ) {
    switch (lhs.type_) {
        case lex::type::SYMBOL:
            switch (boost::get< lex::symbol >( lhs.value_ )) {
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
                    assert(!"FAIL!");
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
            assert(!"FAIL!");
    }
    assert(!"FAIL!");
    return false;
}

lex skip( LIterator& it ) {
    while (shouldSkip( *it )) ++it;

    return *it++;
}

node createTree( Queue& queue, LIterator& it)
{
    assert(!queue.empty());
    const auto rule = queue.front();
    queue.pop();

    node root;
    root.rule_ = getRule( rule );

    const auto N = getCount( rule );
    if (0 > N) {
        root.value_ = lex{ lex::type::EPS, false };
    } else if (0 == N) {
        root.value_ = skip( it );

        switch( rule) {
            case 9:
            case 10:
            case 11:
            case 13:
                assert( lex::symbol::EQUAL == boost::get< lex::symbol >( skip( it ).value_) );
                break;
            default:
                break;
                //noop
        }
    } else {
        auto nodes = node::nodes( N );

        for (int i = 0; N != i; ++i) {
            nodes[ i ] = createTree( queue, it );
        }

        root.value_ = nodes;
    }

    return root;
}

enum class variable_name : uint8_t { CLASS , METHOD , VAR};

using identifier_table = std::unordered_map< std::string, variable_name >;


using identifiers = std::vector< std::string >;
using identifier  = identifiers::const_iterator;
using double_constants = std::vector< double >;
using double_constant  = double_constants::const_iterator;
using bool_constants   = std::vector< bool >;
using bool_constant    = bool_constants::const_iterator;
using string_constants = std::vector< std::string >;
using string_constant  = string_constants::const_iterator;

using operand_double = boost::variant< identifier, double_constant >;
using operand_bool   = boost::variant< identifier, bool_constant >;
using operand_string = boost::variant< identifier, string_constant >;

enum class operator_double : uint8_t { NONE, PLUS, MINUS, MULT, DIV };
struct expr_int
{
    // expr_int( LIterator it, const identifiers inherited ) {
    //     if (
    // }
    operand_double lhs;
    operator_double  op;
    operand_double rhs;
};

enum class comparator_eq : uint8_t { EQ, NE };
enum class comparator_double : uint8_t { EQ, NE, LE, GE };

struct logic_double
{
    operand_double lhs;
    comparator_double cmp;
    operand_double rhs;
};

struct logic_bool_b
{
    operand_bool lhs;
    comparator_eq cmp;
    operand_bool rhs;
};
using logic_bool = boost::variant< operand_bool, logic_bool_b >;

struct logic_string
{
    operand_string lhs;
    comparator_eq cmp;
    operand_string rhs;
};

using logic = boost::variant< logic_double, logic_bool, logic_string >;

using constructor = identifier;

using applicable = identifier;

struct method_call {
    applicable lhs;
    identifier rhs;
    // params
};

enum class reserved : uint8_t { READ, WRITE, BREAK };

using rightside = boost::variant< logic, expr_int, reserved, constructor, method_call, string_constant >;

struct assignment
{
    identifier lhs;
    rightside rhs;
};

struct returnline { rightside what; };

struct ifline;
struct whileline;
using sline = boost::variant< assignment, reserved, returnline, ifline, whileline, method_call>;

struct slines {
    std::vector< sline > lines;
    identifiers locals;
    const identifiers inherited;

    slines( const identifiers i_inherited ) : inherited{ i_inherited } {}

};


struct ifline {
    logic lhs;
    slines thenpart;
    slines elsepart;
};

struct whileline {
    logic condition;
    slines body;
};

struct method {
    identifier name;
    identifiers params;
    identifiers ids;
    slines body;
};

struct classdecl {
    identifier name;
    identifiers methods;
    std::vector< method > method_definitions;
};

struct program {
    identifiers classes;
    std::vector< classdecl > class_decls;
    method_call main;
};

} // namespace sap
