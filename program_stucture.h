#pragma once

#include "program_tree.h"

#include <vector>
#include <string>
#include <list>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace sap
{
class SymbolTable
{
public:
    using Identifiers   = std::list< std::string >;
    using Identifier    = Identifiers::const_iterator;
    using Methods       = std::multimap< std::string, uint >; // method → number of parameters
    using Method        = Methods::const_iterator;
    // using ClassMethods  = std::multimap< Identifier, Method >; // class -> method

    Identifiers variables_;     // all visible variables
    Identifiers classNames_;    // all visible classes
    // ClassMethods classMethods_; // class -> method
    Methods  methods_;      // all methods

    SymbolTable( const SymbolTable& other );
    // boost::optional< Method > getMethod_o( const Identifier i_className, const std::string& i_methodName ) const;
    // Method getMethod( const Identifier i_className, const std::string& i_methodName ) const;
    // Methods getMethod( const std::string& i_name ) const
    // {
    //     Methods result;
    //     std::copy_if( methods_.cbegin(), methods_.cend(), std::insert_iterator< Methods >( result, result.cend() ),
    //             [&i_name] ( const Methods::value_type& lhs )
    //             {
    //                 return i_name == lhs.first;
    //             }
    //             );
    //     return result;
    // }

    boost::optional< Identifier > getClassName_o( const std::string& i_name ) const;
    Identifier getClassName( const std::string& i_name ) const;
    boost::optional< Identifier > getVariable_o( const std::string& i_name ) const;
    Identifier getVariable( const std::string& i_name ) const;

    Identifier addVariable( const std::string& i_name );
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

    OperandDouble( const node& i_node, const SymbolTable& i_symbolTable );
};

struct OperandBool : ProgramElement< lex::rule::OPERAND_BOOL >
{
    using Value = boost::variant< SymbolTable::Identifier, bool >;
    Value value_;

    OperandBool( const node& i_node, const SymbolTable& i_symbolTable );
};

struct OperandString : ProgramElement< lex::rule::OPERAND_STR >
{
    using Value = boost::variant< SymbolTable::Identifier, std::string >;
    Value value_;

    OperandString( const node& i_node, const SymbolTable& i_symbolTable );
};

struct ExprDouble : ProgramElement< lex::rule::EXPR_INT >
{
    OperandDouble  lhs_;
    enum class Op { PLUS, MINUS, MULT, DIV };
    boost::optional< Op > op_;
    boost::optional< OperandDouble >  rhs_;

    ExprDouble( const node& i_node, const SymbolTable& i_symbolTable );
};

enum class Cmp { EQ, NE, LE, GE };

struct LogicBool : ProgramElement< lex::rule::LOGIC_BOOL >
{
    OperandBool  lhs_;
    boost::optional< Cmp > cmp_;
    boost::optional< OperandBool >  rhs_;

    LogicBool( const node& i_node, const SymbolTable& i_symbolTable );
};

struct LogicDouble : ProgramElement< lex::rule::LOGIC_INT >
{
    OperandDouble  lhs_;
    Cmp cmp_;
    OperandDouble rhs_;

    LogicDouble( const node& i_node, const SymbolTable& i_symbolTable );
};

struct LogicString : ProgramElement< lex::rule::LOGIC_STR >
{
    OperandString  lhs_;
    Cmp cmp_;
    OperandString rhs_;

    LogicString( const node& i_node, const SymbolTable& i_symbolTable );
};

using Logic = boost::variant< LogicBool, LogicDouble, LogicString >;

using Constructor = SymbolTable::Identifier;
struct MethodCall;
struct Input;
using Rightside = boost::variant< Logic, ExprDouble, Constructor, MethodCall, Input, std::string >;
using Parameters = std::vector< Rightside >;

struct Applicable : ProgramElement< lex::rule::APPLICABLE >
{
    boost::optional< SymbolTable::Identifier > object_;
    boost::optional< Constructor > class_;

    Applicable( const node& i_node, const SymbolTable& i_symbolTable );
};

struct MethodCall : ProgramElement< lex::rule::MCALL >
{
    Applicable lhs_;
    std::string method_;
    Parameters params_;

    MethodCall( const node& i_node, const SymbolTable& i_symbolTable );
};

struct Input : ProgramElement< lex::rule::INPUT >
{
    Input( const node& i_node );
};

struct NewVariable : ProgramElement< lex::rule::NEW_IDENTIFIER >
{
    SymbolTable::Identifier this_;
    NewVariable( const node& i_node, SymbolTable& i_symbolTable );
};


struct Assignment : ProgramElement< lex::rule::ASSIGNMENT >
{
    NewVariable lhs_;
    Rightside rhs_;
    Assignment( const node& i_node, SymbolTable& i_symbolTable );
};

struct Break : ProgramElement< lex::rule::BREAKLINE >
{
    Break( const node& i_node );
};

struct Return : ProgramElement< lex::rule::RETURNLINE >
{
    Rightside rhs_;
    Return( const node& i_node, const SymbolTable& i_symbolTable );
};

struct Print : ProgramElement< lex::rule::PRINTLINE >
{
    Rightside rhs_;
    Print( const node& i_node, const SymbolTable& i_symbolTable );
};

struct If;
struct While;
using Sline = boost::variant< Assignment, MethodCall, Break, Return, Print, If, While >;
using Slines = std::vector< Sline >;

struct If : ProgramElement< lex::rule::IFLINE >
{
    Logic logic_;
    SymbolTable thenTable_;
    Slines then_;
    SymbolTable elseTable_;
    Slines else_;

    If( const node& i_node, const SymbolTable& i_symbolTable );
};

struct While : ProgramElement< lex::rule::WHILELINE >
{
    Logic logic_;
    SymbolTable localTable_;
    Slines body_;

    While( const node& i_node, const SymbolTable& i_symbolTable );
};
} // namespace sap
