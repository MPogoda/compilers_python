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

#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace sap
{
class SymbolTable
{
public:
    using Identifiers = std::vector< std::string >;
    using Identifier  = Identifiers::const_iterator;

    Identifiers variables_;
    Identifiers classnames_;

    Identifier getClassName( const std::string& i_name ) const
    {
        const Identifier result = std::find( classnames_.begin(), classnames_.end(), i_name );
        if (classnames_.end() == result) {
            throw std::logic_error{ "No such class!" };
        }

        return result;
    }

    Identifier getVariable( const std::string& i_name ) const // can throw
    {
        const Identifier result = std::find( variables_.begin(), variables_.end(), i_name );
        if (variables_.end() == result) {
            throw std::logic_error{ "No such variable!" };
        }

        return result;
    }
};

template < lex::rule Rule >
struct ProgramElement
{
    ProgramElement( const node& i_node )
    {
        assert( Rule == i_node.rule_ );
    }
};

struct OperandDouble : ProgramElement< lex::rule::OPERAND_INT >
{
    using Value = boost::variant< SymbolTable::Identifier, double >;
    Value value_;

    OperandDouble( const node& i_node, const SymbolTable& i_symbolTable )
        : ProgramElement( i_node )
    {
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

struct OperandBool : ProgramElement< lex::rule::OPERAND_BOOL >
{
    using Value = boost::variant< SymbolTable::Identifier, bool >;
    Value value_;

    OperandBool( const node& i_node, const SymbolTable& i_symbolTable )
        : ProgramElement( i_node )
    {
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

struct OperandString : ProgramElement< lex::rule::OPERAND_STR >
{
    using Value = boost::variant< SymbolTable::Identifier, std::string >;
    Value value_;

    OperandString( const node& i_node, const SymbolTable& i_symbolTable )
        : ProgramElement( i_node )
    {
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

struct ExprDouble : ProgramElement< lex::rule::EXPR_INT >
{
    OperandDouble  lhs_;
    enum class Op { PLUS, MINUS, MULT, DIV };
    boost::optional< Op > op_;
    boost::optional< OperandDouble >  rhs_;

    ExprDouble( const node& i_node, const SymbolTable& i_symbolTable )
        : ProgramElement( i_node )
        , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
    {
        const auto& nodes = boost::get< node::nodes >( i_node.value_ );

        if (nodes.size() == 3) {
            const auto& op_node = nodes[ 1 ];
            assert( lex::rule::OPERATOR_INT == op_node.rule_ );
            const lex op_lexeme = boost::get< lex >( op_node.value_ );
            assert( lex::type::SYMBOL == op_lexeme.type_ );
            switch (boost::get< lex::symbol >(op_lexeme.value_)) {
                case lex::symbol::PLUS:
                    op_ = Op::PLUS;
                    break;
                case lex::symbol::MINUS:
                    op_ = Op::MINUS;
                    break;
                case lex::symbol::STAR:
                    op_ = Op::MULT;
                    break;
                case lex::symbol::SLASH:
                    op_ = Op::DIV;
                    break;
                default:
                    DEBUG( op_lexeme);
                    assert( !"Fail!" );
            }

            rhs_ = OperandDouble{  nodes[ 2 ], i_symbolTable };
        } else if (nodes.size() != 1) {
            throw std::logic_error{ "Wrong number of lexemes in expression!" };
        }
    }
};

enum class Cmp { EQ, NE, LE, GE };
Cmp pareCmp( const node& i_node )
{
    assert( (lex::rule::COMPARATOR_EQ == i_node.rule_) || (lex::rule::COMPARATOR_INT == i_node.rule_ ));

    const bool HAS_ORDER = lex::rule::COMPARATOR_INT == i_node.rule_;

    const lex op_lexeme = boost::get< lex >( i_node.value_ );
    assert( lex::type::SYMBOL == op_lexeme.type_ );

    switch (boost::get< lex::symbol >(op_lexeme.value_)) {
        case lex::symbol::LESS:
            assert( HAS_ORDER );
            return Cmp::LE;
            break;
        case lex::symbol::GREATER:
            assert( HAS_ORDER );
            return Cmp::GE;
            break;
        case lex::symbol::EQUAL:
            return Cmp::EQ;
            break;
        case lex::symbol::NOT_EQUAL:
            return Cmp::NE;
            break;
        default:
            DEBUG( op_lexeme);
            assert( !"Fail!" );
    }
}

struct LogicBool : ProgramElement< lex::rule::LOGIC_BOOL >
{
    OperandBool  lhs_;
    boost::optional< Cmp > cmp_;
    boost::optional< OperandBool >  rhs_;

    LogicBool( const node& i_node, const SymbolTable& i_symbolTable )
        : ProgramElement( i_node )
        , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
    {
        const auto& nodes = boost::get< node::nodes >( i_node.value_ );

        if (nodes.size() == 3) {
            cmp_ = pareCmp( nodes[ 1 ] );

            rhs_ = OperandBool{  nodes[ 2 ], i_symbolTable };
        } else if (nodes.size() != 1) {
            throw std::logic_error{ "Wrong number of lexemes in expression!" };
        }
    }
};

struct LogicDouble : ProgramElement< lex::rule::LOGIC_INT >
{
    OperandDouble  lhs_;
    Cmp cmp_;
    OperandDouble rhs_;

    LogicDouble( const node& i_node, const SymbolTable& i_symbolTable )
        : ProgramElement( i_node )
        , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
        , cmp_{ pareCmp( boost::get< node::nodes >( i_node.value_ ).at( 1 ) ) }
        , rhs_{ boost::get< node::nodes >( i_node.value_ ).at( 2 ), i_symbolTable }
    {
        if ( boost::get< node::nodes >( i_node.value_ ).size() != 3) {
            throw std::logic_error{ "Wrong number of lexemes in expression!" };
        }
    }
};

struct LogicString : ProgramElement< lex::rule::LOGIC_STR >
{
    OperandString  lhs_;
    Cmp cmp_;
    OperandString rhs_;

    LogicString( const node& i_node, const SymbolTable& i_symbolTable )
        : ProgramElement( i_node )
        , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
        , cmp_{ pareCmp( boost::get< node::nodes >( i_node.value_ ).at( 1 ) ) }
        , rhs_{ boost::get< node::nodes >( i_node.value_ ).at( 2 ), i_symbolTable }
    {
        if ( boost::get< node::nodes >( i_node.value_ ).size() != 3) {
            throw std::logic_error{ "Wrong number of lexemes in expression!" };
        }
    }
};

using Logic = boost::variant< LogicBool, LogicDouble, LogicString >;
Logic parseLogic( const node& i_node, const SymbolTable& i_symbolTable )
{
    ProgramElement< lex::rule::LOGIC > assertion{ i_node };

    const auto& nodes = boost::get< node::nodes >( i_node.value_ );
    assert( 1 == nodes.size() );

    const auto& child = nodes[ 0 ];
    switch (child.rule_) {
        case lex::rule::LOGIC_INT:
            return LogicDouble{ child, i_symbolTable };
        case lex::rule::LOGIC_BOOL:
            return LogicBool{ child, i_symbolTable };
        case lex::rule::LOGIC_STR:
            return LogicString{ child, i_symbolTable };
        default:
            DEBUG( child.rule_ );
            assert( !"Wrong child node!" );
    }
}

using Constructor = SymbolTable::Identifier;
Constructor parseConstructor( const node& i_node, const SymbolTable& i_symbolTable );
using Rightside = boost::variant< Logic, ExprDouble, Constructor >; // TODO: add more
Rightside parseRightside( const node& i_node, const SymbolTable& i_symbolTable )
{
    const ProgramElement< lex::rule::RIGHTSIDE > assertion{ i_node };

    const auto& nodes = boost::get< node::nodes >( i_node.value_ );
    assert( 1 == nodes.size() );

    const auto& child = nodes[ 0 ];
    switch (child.rule_) {
        case lex::rule::LOGIC:
            return parseLogic( child, i_symbolTable );
        case lex::rule::EXPR_INT:
            return ExprDouble{ child, i_symbolTable };
        case lex::rule::FCALL:
            return parseConstructor( child, i_symbolTable );
            // TODO: Add more
        default:
            DEBUG( child.rule_ );
            assert( !"Wrong child node!" );
    }
}

using Parameters = std::vector< Rightside >;
Parameters parseParameters( const node& i_node, const SymbolTable& i_symbolTable )
{
    const ProgramElement< lex::rule::RIGHTSIDE > assertion{ i_node };

    Parameters result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( parseRightside( (*nodes)[ 0 ], i_symbolTable ) );

        const ProgramElement< lex::rule::PARAM_LIST > assertion{ (*nodes)[1] };
        pnode = &(*nodes)[1];
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

Constructor parseConstructor( const node& i_node, const SymbolTable& i_symbolTable )
{
    const ProgramElement< lex::rule::FCALL > assertion{ i_node };

    const auto& children = boost::get< node::nodes >( i_node.value_ );
    assert( 2 == children.size() );

    {
        assert( parseParameters( children[ 1 ], i_symbolTable ).size() == 0 );
    }

    {
        const auto& id_node = children[ 0 ];
        const ProgramElement< lex::rule::NEW_IDENTIFIER > assertion{ id_node };

        const auto& lexeme = boost::get< lex >( id_node.value_ );
        return i_symbolTable.getClassName( boost::get< std::string >( lexeme.value_ ) );
    }
}
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
