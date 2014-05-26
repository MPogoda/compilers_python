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
//          construct SLINES( node, method_used_names , -1 ) and save it  to statements
//
//  Slines:
//      has lines
//      has block_names
//      has nesting
//      Constructor( node, used_names, nesting ):
//          ASSERT type of node
//          block_names = used_names
//          nesting = nesting
//          Iteratate over all its children: (except last (EPS))
//              switch rule:
//                  break: REPORT ERROR if nesting == -1
//                  return: create RETURN( node, method_used_names (const)), expect no more rules
//                  print: create PRINT( node, method_used_names (const))
//                  assignment: create ASSIGNMENT( node, method_used_names (non-const))
//                  ifline: create IF_STMT( node, method_used_names (const), nesting = nesting)
//                  whileline: create while_stmt( node, method_used_names(const), nesting = nesting)
//                  mcall: create METHOD_CALL( node, method_used_names (const))
//              whatever is created, append to lines
//
//  If_stmt: (same for while, except (nesting=nesting+1))
//      has logic
//      has then_block
//      has else_block
//      Constructor( node, method_used_names, nesting ):
//          ASSERT node type is IFLINE
//          Construct LOGIC from first children
//
//          Goto second child (THENPART)
//          construct SLINES( node, method_used_names , nesting ) and save it to then_block
//
//          Goto thirt chils (ELSEPART)
//          If its child is EPS, NOP
//          construct SLINES( node, method_used_names , nesting ) and save it to else_part
//
//  Operand_*:
//      has name or value
//      Constructor( node, used_names ):
//          ASSERT node type
//          Get LEAF
//          if its type is const, store  it
//          else, look up for identifier
//          If such name not exists or its type is not variable, ABORT
//          Else store iterator to it.
//
//  EXPR: LOGIC_*
//      has lhs
//      has op or NOP
//      has rhs
//      Constructor( node, used_names ):
//          NODE TYPE ASSERTION
//          Get children count
//          if it is 3
//              get lhs
//              get op
//              get rhs
//          else
//              get lhs
//              set op = NOP
//
//  FCALL:
//      has classname
//      has params
//      CONSTRUCTOR( node, used_names ):
//          assert node type
//          get classname
//          check whether it presents and is CLASSNAME
//
//  APPLICABLE:
//      has identifier
//      has classname
//
//      Constructor( node, used_names):
//          assert node type
//          get first child
//          if type == FCALL:
//              store it as classname
//          else:
//              type should be equal to IDENTIFIER
//              check that this identifier is VARIABLE
//              store it as identifier
//              get identifier type
//              store it as classname
//
//  MCALL:
//      has applicable
//      has methodname
//      has parameters
//      Ctor( node, used_names ):
//          assert node type
//          get applicable( child[0], used_names )
//          get methodname from child[ 1 ]
//          assert applicable.classname has methodname
//
//          get number of parameters from class
//  RIGHTSIDE: combine all previous typer
//
//
//
//
//

#include <vector>

#include "program_tree.h"

#ifdef NDEBUG
    #define DEBUG(a)
    #define DBG(a)
#else
    #include <iostream>
    #define DEBUG(a) std::cout << #a << " = " << (a) << '\n'
    #define DBG(a) std::cout << __LINE__ << " : " << a << '\n';
#endif

#include <string>
#include <algorithm>
namespace sap
{
class symbol_table
{
public:
    using identifiers = std::vector< std::string >;
    using identifier  = identifiers::const_iterator;

    identifiers variables;

    identifier getVariable( const std::string& i_name ) const // can throw
    {
        const identifier result = std::find( variables.cbegin(), variables.cend(), i_name );
        if (variables.end() == result) {
            throw std::logic_error{ "No such variable!" };
        }

        return result;
    }
};

struct operand_double
{
    using value = boost::variant< symbol_table::identifier, double >;
    value value_;

    operand_double( node i_node, const symbol_table& i_symbolTable )
    {
        assert( i_node.rule_ == lex::rule::OPERAND_INT );
        const lex value = boost::get< lex >( i_node.value_);
        if (value.type_ == lex::type::D_CONST) {
            value_ = boost::get< double >( value.value_ );
        } else {
            assert( value.type_ == lex::type::IDENTIFIER );

            value_ = i_symbolTable.getVariable( boost::get< std::string >( value.value_ ) );

            // TODO: check type of variable
        }
    }
};
struct operand_bool
{
    using value = boost::variant< symbol_table::identifier, bool >;
    value value_;

    operand_bool( node i_node, const symbol_table& i_symbolTable )
    {
        assert( i_node.rule_ == lex::rule::OPERAND_BOOL );
        const lex value = boost::get< lex >( i_node.value_);
        if (value.type_ == lex::type::B_CONST) {
            value_ = boost::get< bool >( value.value_ );
        } else {
            assert( value.type_ == lex::type::IDENTIFIER );

            value_ = i_symbolTable.getVariable( boost::get< std::string >( value.value_ ) );

            // TODO: check type of variable
        }
    }
};
struct operand_string
{
    using value = boost::variant< symbol_table::identifier, std::string >;
    value value_;

    operand_string( node i_node, const symbol_table& i_symbolTable )
    {
        assert( i_node.rule_ == lex::rule::OPERAND_STR );
        const lex value = boost::get< lex >( i_node.value_);
        if (value.type_ == lex::type::S_CONST) {
            value_ = boost::get< std::string >( value.value_ );
        } else {
            assert( value.type_ == lex::type::IDENTIFIER );

            value_ = i_symbolTable.getVariable( boost::get< std::string >( value.value_ ) );

            // TODO: check type of variable
        }
    }
};

// enum class variable_name : uint8_t { CLASS , METHOD , VAR};
//
// using identifier_table = std::unordered_map< std::string, variable_name >;
//
//
// using identifiers = std::vector< std::string >;
// using identifier  = identifiers::const_iterator;
// using double_constants = std::vector< double >;
// using double_constant  = double_constants::const_iterator;
// using bool_constants   = std::vector< bool >;
// using bool_constant    = bool_constants::const_iterator;
// using string_constants = std::vector< std::string >;
// using string_constant  = string_constants::const_iterator;
//
// using operand_double = boost::variant< identifier, double_constant >;
// using operand_bool   = boost::variant< identifier, bool_constant >;
// using operand_string = boost::variant< identifier, string_constant >;
//
// enum class operator_double : uint8_t { NONE, PLUS, MINUS, MULT, DIV };
// struct expr_int
// {
//     // expr_int( LIterator it, const identifiers inherited ) {
//     //     if (
//     // }
//     operand_double lhs;
//     operator_double  op;
//     operand_double rhs;
// };
//
// enum class comparator_eq : uint8_t { EQ, NE };
// enum class comparator_double : uint8_t { EQ, NE, LE, GE };
//
// struct logic_double
// {
//     operand_double lhs;
//     comparator_double cmp;
//     operand_double rhs;
// };
//
// struct logic_bool_b
// {
//     operand_bool lhs;
//     comparator_eq cmp;
//     operand_bool rhs;
// };
// using logic_bool = boost::variant< operand_bool, logic_bool_b >;
//
// struct logic_string
// {
//     operand_string lhs;
//     comparator_eq cmp;
//     operand_string rhs;
// };
//
// using logic = boost::variant< logic_double, logic_bool, logic_string >;
//
// using constructor = identifier;
//
// using applicable = identifier;
//
// struct method_call {
//     applicable lhs;
//     identifier rhs;
//     // params
// };
//
// enum class reserved : uint8_t { READ, WRITE, BREAK };
//
// using rightside = boost::variant< logic, expr_int, reserved, constructor, method_call, string_constant >;
//
// struct assignment
// {
//     identifier lhs;
//     rightside rhs;
// };
//
// struct returnline { rightside what; };
//
// struct ifline;
// struct whileline;
// using sline = boost::variant< assignment, reserved, returnline, ifline, whileline, method_call>;
//
// struct slines {
//     std::vector< sline > lines;
//     identifiers locals;
//     const identifiers inherited;
//
//     slines( const identifiers i_inherited ) : inherited{ i_inherited } {}
//
// };
//
//
// struct ifline {
//     logic lhs;
//     slines thenpart;
//     slines elsepart;
// };
//
// struct whileline {
//     logic condition;
//     slines body;
// };
//
// struct method {
//     identifier name;
//     identifiers params;
//     identifiers ids;
//     slines body;
// };
//
// struct classdecl {
//     identifier name;
//     identifiers methods;
//     std::vector< method > method_definitions;
// };
//
// struct program {
//     identifiers classes;
//     std::vector< classdecl > class_decls;
//     method_call main;
// };
//
} // namespace sap
