#pragma once

#include "program_tree.h"

#include <vector>
#include <string>
#include <list>
#include <iosfwd>
#ifndef NDEBUG
    #include <iostream>
#endif

#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace sap
{
struct Triad;

struct ProgramCode
{
    using set = std::set< Triad >;

    set code_;
    uint addTriad( const std::string& i_op, const std::string& i_lhs = "", const std::string& i_rhs = "" );
    static ProgramCode& instance();
private:
    ProgramCode() = default;
    ProgramCode( const ProgramCode& ) = delete;
    ProgramCode( ProgramCode&& ) = delete;
    ProgramCode& operator=( const ProgramCode& ) = delete;
    ProgramCode& operator=( ProgramCode&& ) = delete;
};

struct Triad
{
    static uint getNextCounter();

    const uint no_;
    std::string op_;
    std::string lhs_;
    std::string rhs_;

    Triad( const std::string& i_op, const std::string& i_lhs, const std::string& i_rhs );
};

struct ClassScope;
using Identifiers   = std::list< std::string >;
using Identifier    = Identifiers::const_iterator;
struct GlobalScope
{
    using Classes = std::map< std::string, ClassScope >;
    Classes classes_;

    bool nameIsFree( const std::string& i_name) const;
    ClassScope& addClass( const std::string& i_name );

    Identifiers getBlockedNames() const;
};

std::ostream& operator<<( std::ostream& out, const GlobalScope& scope );

struct MethodScope;
struct ClassScope
{
    using Methods = std::map< std::string, MethodScope >;

    const Identifiers blocked_;
    Methods methods_;

    bool nameIsFree( const std::string& i_name ) const;
    MethodScope& addMethod( const std::string& i_name);

    ClassScope( Identifiers&& i_blocked );

    Identifiers getBlockedNames() const;
};
std::ostream& operator<<( std::ostream& out, const ClassScope& scope );

struct Scope;
struct Scope
{
    using Scopes = std::list< Scope >;
    const Identifiers inherited_;
    const Identifiers blocked_;
    Identifiers local_;
    Scopes  inner_;

    Scope() = default;
    Scope( Identifiers&& i_blocked, Identifiers&& i_inherited );
    boost::optional< Identifier > getVariable_o( const std::string& i_name ) const;
    Identifier getVariable( const std::string& i_name ) const;
    Identifier addVariable( const std::string& i_name );

    Scope& addScope();

    Identifiers getBlockedNames() const;
    Identifiers getInherited() const;
};
std::ostream& operator<<( std::ostream& out, const Scope& scope );

struct MethodScope
{
    const Identifiers blocked_;
    Identifiers parameters_;
    std::shared_ptr< Scope > local_;

    MethodScope( Identifiers&& i_blocked );
    // MethodScope( const MethodScope& other ) : blocked_{ other.blocked_}, parameters_{ other.parameters_ }, local_(  other.local_ ) { }

    bool nameIsFree( const std::string& i_name ) const;
    Identifier addParameter( const std::string& i_name );

    Identifiers getBlockedNames() const;
    Identifiers getParameters() const;

    Scope& scope();
};
std::ostream& operator<<( std::ostream& out, const MethodScope& scope );

template < lex::rule Rule >
struct ProgramElement
{
    ProgramElement( const node& i_node )
    {
        #ifndef NDEBUG
            std::cout << "Parsing node with " << Rule << ';'
                    << "Actual " << i_node.rule_ << '\n';
        #endif
        assert( Rule == i_node.rule_ );
    }
};

struct OperandDouble : ProgramElement< lex::rule::OPERAND_INT >
{
    using Value = boost::variant< Identifier, double >;
    Value value_;

    OperandDouble( const node& i_node, const Scope& i_symbolTable );
    uint operator()() const;
};

struct OperandBool : ProgramElement< lex::rule::OPERAND_BOOL >
{
    using Value = boost::variant< Identifier, bool >;
    Value value_;

    OperandBool( const node& i_node, const Scope& i_symbolTable );
    uint operator()() const;
};

struct OperandString : ProgramElement< lex::rule::OPERAND_STR >
{
    using Value = boost::variant< Identifier, std::string >;
    Value value_;

    OperandString( const node& i_node, const Scope& i_symbolTable );
    uint operator()() const;
};

struct ExprDouble : ProgramElement< lex::rule::EXPR_INT >
{
    OperandDouble  lhs_;
    enum class Op { PLUS, MINUS, MULT, DIV };
    boost::optional< Op > op_;
    boost::optional< OperandDouble >  rhs_;

    ExprDouble( const node& i_node, const Scope& i_symbolTable );
    uint operator()() const;
};

enum class Cmp { EQ, NE, LE, GE };

struct LogicBool : ProgramElement< lex::rule::LOGIC_BOOL >
{
    OperandBool  lhs_;
    boost::optional< Cmp > cmp_;
    boost::optional< OperandBool >  rhs_;

    LogicBool( const node& i_node, const Scope& i_symbolTable );
};

struct LogicDouble : ProgramElement< lex::rule::LOGIC_INT >
{
    OperandDouble  lhs_;
    Cmp cmp_;
    OperandDouble rhs_;

    LogicDouble( const node& i_node, const Scope& i_symbolTable );
};

struct LogicString : ProgramElement< lex::rule::LOGIC_STR >
{
    OperandString  lhs_;
    Cmp cmp_;
    OperandString rhs_;

    LogicString( const node& i_node, const Scope& i_symbolTable );
};

using Logic = boost::variant< LogicBool, LogicDouble, LogicString >;

using Constructor = std::string;
struct MethodCall;
struct Input;
using Rightside = boost::variant< Logic, ExprDouble, Constructor, MethodCall, Input, std::string >;
using Parameters = std::vector< Rightside >;

struct Applicable : ProgramElement< lex::rule::APPLICABLE >
{
    boost::optional< Identifier > object_;
    boost::optional< Constructor > class_;

    Applicable( const node& i_node, const Scope& i_symbolTable );
};

struct MethodCall : ProgramElement< lex::rule::MCALL >
{
    Applicable lhs_;
    std::string method_;
    Parameters params_;

    MethodCall( const node& i_node, const Scope& i_symbolTable );
};

struct Input : ProgramElement< lex::rule::INPUT >
{
    Input( const node& i_node );
};

struct NewVariable : ProgramElement< lex::rule::NEW_IDENTIFIER >
{
    Identifier this_;
    NewVariable( const node& i_node, Scope& i_symbolTable );
};


struct Assignment : ProgramElement< lex::rule::ASSIGNMENT >
{
    NewVariable lhs_;
    Rightside rhs_;
    Assignment( const node& i_node, Scope& i_symbolTable );
};

struct Break : ProgramElement< lex::rule::BREAKLINE >
{
    Break( const node& i_node );
};

struct Return : ProgramElement< lex::rule::RETURNLINE >
{
    Rightside rhs_;
    Return( const node& i_node, const Scope& i_symbolTable );
};

struct Print : ProgramElement< lex::rule::PRINTLINE >
{
    Rightside rhs_;
    Print( const node& i_node, const Scope& i_symbolTable );
};

struct If;
struct While;
using Sline = boost::variant< Assignment, MethodCall, Break, Return, Print, If, While >;
using Slines = std::vector< Sline >;

struct If : ProgramElement< lex::rule::IFLINE >
{
    Logic logic_;
    Scope& thenTable_;
    Slines then_;
    Scope& elseTable_;
    Slines else_;

    If( const node& i_node, Scope& i_symbolTable );
};

struct While : ProgramElement< lex::rule::WHILELINE >
{
    Logic logic_;
    Scope& localTable_;
    Slines body_;

    While( const node& i_node, Scope& i_symbolTable );
};

using MethodParameters = std::vector< Identifier >;

struct MethodDecl : ProgramElement< lex::rule::METHOD_DECL >
{
    MethodScope& methodScope_;
    MethodParameters params_;
    Slines body_;

    MethodDecl( const node& i_node, ClassScope& i_symbolTable );
};

using MethodDecls = std::vector< MethodDecl >;

struct ClassDecl : ProgramElement< lex::rule::CLASS_DECL >
{
    ClassScope& classScope_;
    MethodDecls methods_;

    ClassDecl( const node& i_node, GlobalScope& i_symbolTable );
};

using ClassDecls = std::vector< ClassDecl >;

struct Program : ProgramElement< lex::rule::START >
{
    GlobalScope scope_;
    ClassDecls classes_;
    MethodCall run_;

    Program( const node& i_node );
};
} // namespace sap
