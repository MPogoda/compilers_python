#include "program_stucture.h"

#ifdef NDEBUG
    #define DEBUG(a)
    #define DBG(a)
#else
    #include <iostream>
    #define DEBUG(a) std::cout << #a << " = " << (a) << '\n'
    #define DBG(a) std::cout << __LINE__ << " : " << a << '\n';
#endif

#include <algorithm>
#include <iterator>

namespace sap
{
bool GlobalScope::nameIsFree( const std::string& i_name ) const
{
    return classes_.find( i_name ) == classes_.cend();
}

ClassScope& GlobalScope::addClass( const std::string& i_name )
{
    assert( nameIsFree( i_name ) );
    Identifiers blocked = getBlockedNames();
    blocked.emplace_back( i_name );
    ClassScope newClass{ std::move( blocked ) };
    classes_.insert( { i_name, newClass } );

    return classes_.find( i_name )->second;
}

Identifiers
GlobalScope::getBlockedNames() const
{
    Identifiers result;
    for (const auto& p : classes_)
        result.emplace_back( p.first );

    return result;
}

ClassScope::ClassScope( Identifiers&& i_blocked )
    : blocked_{ i_blocked }
{
}

bool ClassScope::nameIsFree( const std::string& i_name ) const
{
    return (std::find( blocked_.cbegin(), blocked_.cend(), i_name ) == blocked_.cend())
        && (methods_.find( i_name ) == methods_.cend());
}

MethodScope& ClassScope::addMethod( const std::string& i_name )
{
    assert( nameIsFree( i_name ) );

    Identifiers blocked = getBlockedNames();
    blocked.emplace_back( i_name );

    MethodScope methodScope{ std::move( blocked ) };
    methods_.insert( { i_name, methodScope } );

    return methods_.find( i_name )->second;
}

Identifiers
ClassScope::getBlockedNames() const
{
    Identifiers result;
    for (const auto& p : methods_)
        result.emplace_back( p.first );

    result.splice( result.end(), Identifiers{ blocked_ } );
    return result;
}

MethodScope::MethodScope( Identifiers&& i_blocked )
    : blocked_{ i_blocked }
    , local_{ nullptr }
{
}

bool MethodScope::nameIsFree( const std::string& i_name ) const
{
    return (std::find( blocked_.cbegin(), blocked_.cend(), i_name ) == blocked_.cend())
        && (std::find( parameters_.cbegin(), parameters_.cend(), i_name ) == parameters_.cend());
}

Identifier MethodScope::addParameter( const std::string& i_name )
{
    assert( nameIsFree( i_name ) );
    parameters_.emplace_back( i_name );

    return std::prev( parameters_.cend() );
}

Identifiers
MethodScope::getBlockedNames() const
{
    return blocked_;
    // Identifiers result = parameters_;
    //
    // result.splice( result.end(), Identifiers{ blocked_ } );
    // return result;
}

Identifiers
MethodScope::getParameters() const
{
    return parameters_;
}

Scope&
MethodScope::scope()
{
    if (nullptr == local_.get())
        local_.reset( new Scope{ getBlockedNames(), getParameters() } );
    return *local_.get();
}

Scope::Scope( Identifiers&& i_blocked, Identifiers&& i_inherited )
    : inherited_{ i_inherited }
    , blocked_{ i_blocked }
{
}

boost::optional< Identifier > Scope::getVariable_o( const std::string& i_name ) const
{
    auto result = std::find( inherited_.cbegin(), inherited_.cend(), i_name );
    if (result != inherited_.cend() ) return result;
    result = std::find( local_.cbegin(), local_.cend(), i_name );
    if (result != local_.cend() ) return result;

    return boost::optional< Identifier >{};
}

Identifier Scope::getVariable( const std::string& i_name ) const
{
    const auto result_o = getVariable_o( i_name );
    if (result_o) return result_o.get();

    throw std::logic_error{ "Cannot find variable!" };
}

Identifier Scope::addVariable( const std::string& i_name )
{
    const auto result_o = getVariable_o( i_name );
    if (result_o) return result_o.get();

    if (blocked_.cend() != std::find( blocked_.cbegin(), blocked_.cend(), i_name ))
        throw std::logic_error{ "Name is used already!" };

    local_.emplace_back( i_name );
    return std::prev( local_.cend() );
}

Identifiers Scope::getBlockedNames() const
{
    return blocked_;
}

Identifiers Scope::getInherited() const
{
    Identifiers result = inherited_;
    result.splice( result.end(), Identifiers{ local_ } );

    return result;
}

OperandDouble::OperandDouble( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
{
    DBG( '!' );
    const lex value = boost::get< lex >( i_node.value_);
    if (value.type_ == lex::type::D_CONST) {
        value_ = boost::get< double >( value.value_ );
    } else {
        assert( value.type_ == lex::type::IDENTIFIER );

        value_ = i_symbolTable.getVariable( boost::get< std::string >( value.value_ ) );
    }
}

OperandBool::OperandBool( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
{
    DBG( '!' );
    const lex value = boost::get< lex >( i_node.value_);
    if (value.type_ == lex::type::B_CONST) {
        value_ = boost::get< bool >( value.value_ );
    } else {
        assert( value.type_ == lex::type::IDENTIFIER );

        value_ = i_symbolTable.getVariable( boost::get< std::string >( value.value_ ) );
    }
}

OperandString::OperandString( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
{
    DBG( '!' );
    const lex value = boost::get< lex >( i_node.value_);
    if (value.type_ == lex::type::S_CONST) {
        value_ = boost::get< std::string >( value.value_ );
    } else {
        assert( value.type_ == lex::type::IDENTIFIER );

        value_ = i_symbolTable.getVariable( boost::get< std::string >( value.value_ ) );
    }
}
//
ExprDouble::Op parseOp( const node& op_node )
{
    DBG( '!' );
    ProgramElement< lex::rule::OPERATOR_INT > assertion{ op_node };

    const lex op_lexeme = boost::get< lex >( op_node.value_ );
    assert( lex::type::SYMBOL == op_lexeme.type_ );
    switch (boost::get< lex::symbol >(op_lexeme.value_)) {
        case lex::symbol::PLUS:
            return ExprDouble::Op::PLUS;
            break;
        case lex::symbol::MINUS:
            return ExprDouble::Op::MINUS;
            break;
        case lex::symbol::STAR:
            return ExprDouble::Op::MULT;
            break;
        case lex::symbol::SLASH:
            return ExprDouble::Op::DIV;
            break;
        default:
            DEBUG( op_lexeme);
            assert( !"Fail!" );
    }
}

ExprDouble::ExprDouble( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
{
    DBG( '!' );
    const auto& nodes = boost::get< node::nodes >( i_node.value_ );

    if (nodes.size() == 3) {
        const auto& op_node = nodes[ 1 ];

        op_ = parseOp( op_node );

        rhs_ = OperandDouble{  nodes[ 2 ], i_symbolTable };
    } else if (nodes.size() != 1) {
        throw std::logic_error{ "Wrong number of lexemes in expression!" };
    }
}

Cmp parseCmp( const node& i_node )
{
    DBG( '!' );
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

LogicBool::LogicBool( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
{
    DBG( '!' );
    const auto& nodes = boost::get< node::nodes >( i_node.value_ );

    if (nodes.size() == 3) {
        cmp_ = parseCmp( nodes[ 1 ] );

        rhs_ = OperandBool{  nodes[ 2 ], i_symbolTable };
    } else if (nodes.size() != 1) {
        throw std::logic_error{ "Wrong number of lexemes in expression!" };
    }
}

LogicDouble::LogicDouble( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
    , cmp_{ parseCmp( boost::get< node::nodes >( i_node.value_ ).at( 1 ) ) }
    , rhs_{ boost::get< node::nodes >( i_node.value_ ).at( 2 ), i_symbolTable }
{
    DBG( '!' );
    if ( boost::get< node::nodes >( i_node.value_ ).size() != 3) {
        throw std::logic_error{ "Wrong number of lexemes in expression!" };
    }
}

LogicString::LogicString( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement( i_node )
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
    , cmp_{ parseCmp( boost::get< node::nodes >( i_node.value_ ).at( 1 ) ) }
    , rhs_{ boost::get< node::nodes >( i_node.value_ ).at( 2 ), i_symbolTable }
{
    DBG( '!' );
    if ( boost::get< node::nodes >( i_node.value_ ).size() != 3) {
        throw std::logic_error{ "Wrong number of lexemes in expression!" };
    }
}

Logic parseLogic( const node& i_node, const Scope& i_symbolTable )
{
    DBG( '!' );
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

Rightside parseRightside( const node& i_node, const Scope& i_symbolTable )
{
    DBG( '!' );
    const ProgramElement< lex::rule::RIGHTSIDE > assertion{ i_node };

    if (const auto nodes = boost::get< node::nodes >( &i_node.value_ )) {
        assert( 1 == nodes->size() );

        const auto& child = (*nodes)[ 0 ];
        switch (child.rule_) {
            case lex::rule::LOGIC:
                return parseLogic( child, i_symbolTable );
            case lex::rule::EXPR_INT:
                return ExprDouble{ child, i_symbolTable };
            case lex::rule::FCALL:
                return Constructor( child, i_symbolTable );
            case lex::rule::MCALL:
                return MethodCall{ child, i_symbolTable };
            case lex::rule::INPUT:
                return Input{ child };
                // TODO: Add more
            default:
                DEBUG( child.rule_ );
                assert( !"Wrong child node!" );
        }
    } else {
        return boost::get< std::string >( boost::get< lex >( i_node.value_ ).value_ );
    }
}

Parameters parseParameters( const node& i_node, const Scope& i_symbolTable )
{
    DBG( '!' );
    const ProgramElement< lex::rule::PARAMS > assertion{ i_node };

    Parameters result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( parseRightside( (*nodes)[ 0 ], i_symbolTable ) );

        const ProgramElement< lex::rule::PARAM_LIST > assertion{ (*nodes)[1] };
        pnode = &(*nodes)[1];
        // if (const auto subnodes = boost::get< node::nodes >( &pnode->value_)) {
        //     assert( subnodes->size() == 1);
        //     pnode = &(*subnodes)[ 0 ];
        //     const ProgramElement< lex::rule::PARAMS > assertion( *pnode );
        // } else {
        //     break;
        // }
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

Constructor::Constructor( const node& i_node, const Scope& i_scope )
    : ProgramElement{ i_node }
{
    DBG( '!' );

    const auto& children = boost::get< node::nodes >( i_node.value_ );
    assert( 2 == children.size() );

    {
        assert( parseParameters( children[ 1 ], i_scope ).size() == 0 );
    }

    {
        const auto& id_node = children[ 0 ];
        const ProgramElement< lex::rule::NEW_IDENTIFIER > assertion{ id_node };

        const auto& lexeme = boost::get< lex >( id_node.value_ );
        name_ = boost::get< std::string >( lexeme.value_ );
    }
}

Applicable::Applicable( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
{
    DBG( '!' );
    if (const auto nodes = boost::get< node::nodes >( &i_node.value_ )) {
        // it's a constructor
        assert( 1 == nodes->size() );
        class_ = Constructor( (*nodes)[ 0 ], i_symbolTable );
    } else {
        const lex id = boost::get< lex >( i_node.value_ );
        assert( lex::type::IDENTIFIER == id.type_ );

        object_ = i_symbolTable.getVariable( boost::get< std::string >( id.value_ ) );
    }
}

MethodCall::MethodCall( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , lhs_{ boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable }
{
    DBG( '!' );
    const auto& nodes = boost::get< node::nodes >( i_node.value_ );
    assert( 2 == nodes.size() );
    const auto& fnode = nodes[ 1 ];
    {
        const ProgramElement< lex::rule::FCALL > assertion{ fnode };

        const auto& children = boost::get< node::nodes >( fnode.value_ );
        assert( 2 == children.size() );

        params_ = parseParameters( children[ 1 ], i_symbolTable );

        {
            const auto& id_node = children[ 0 ];
            const ProgramElement< lex::rule::NEW_IDENTIFIER > assertion{ id_node };

            const auto& lexeme = boost::get< lex >( id_node.value_ );
            method_ = boost::get< std::string >( lexeme.value_ );
        }
    }
}

Input::Input( const node& i_node )
    : ProgramElement{ i_node }
{
    DBG( '!' );
    assert( lex::type::EPS == boost::get< lex >( i_node.value_ ).type_ );
}

NewVariable::NewVariable( const node& i_node, Scope& i_symbolTable )
    : ProgramElement{ i_node }
{
    DBG( '!' );
    const auto& child = boost::get< lex >( i_node.value_ );
    assert( lex::type::IDENTIFIER == child.type_ );
    const auto name = boost::get< std::string >( child.value_ );

    this_ = i_symbolTable.addVariable( name );
}

Assignment::Assignment( const node& i_node, Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , lhs_{ boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable }
    , rhs_{ parseRightside( boost::get< node::nodes >( i_node.value_ )[ 1 ], i_symbolTable ) }
{
    DBG( '!' );
}

Break::Break( const node& i_node )
    : ProgramElement{ i_node }
{
    DBG( '!' );
    assert( boost::get< lex >( i_node.value_ ).type_ == lex::type::EPS );
}

Return::Return( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , rhs_{ parseRightside( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
{
    DBG( '!' );
}

Print::Print( const node& i_node, const Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , rhs_{ parseRightside( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
{
    DBG( '!' );
}

Scope& Scope::addScope()
{
    DBG( '!' );
    inner_.emplace_back(  getBlockedNames(), getInherited() );

    return  *std::prev( inner_.end() );
}

Sline parseSline( const node& i_node, Scope& i_symbolTable )
{
    DBG( '!' );
    const ProgramElement< lex::rule::SLINE > assertion{ i_node };

    const auto nodes = boost::get< node::nodes >( i_node.value_ );
    assert( 1 == nodes.size() );

    const auto& child = nodes[ 0 ];
    switch (child.rule_) {
        case lex::rule::MCALL:
            return MethodCall{ child, i_symbolTable };
        case lex::rule::BREAKLINE:
            return Break{ child };
        case lex::rule::RETURNLINE:
            return Return{ child, i_symbolTable };
        case lex::rule::ASSIGNMENT:
            return Assignment{ child, i_symbolTable };
        case lex::rule::PRINTLINE:
            return Print{ child, i_symbolTable };
        case lex::rule::IFLINE:
            return If{ child, i_symbolTable };
        case lex::rule::WHILELINE:
            return While{ child, i_symbolTable };
        default:
            DEBUG( child.rule_ );
            assert( !"Wrong child node!" );
    }
}

Slines parseSlines( const node& i_node, Scope& i_symbolTable )
{
    DBG( '!' );
    const ProgramElement< lex::rule::SLINES > assertion{ i_node };

    Slines result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( parseSline( (*nodes)[ 0 ], i_symbolTable ) );

        const ProgramElement< lex::rule::SLINE_LIST > assertion{ (*nodes)[1] };
        pnode = &(*nodes)[1];
        if (const auto subnodes = boost::get< node::nodes >( &pnode->value_)) {
            assert( subnodes->size() == 1);
            pnode = &(*subnodes)[ 0 ];
            const ProgramElement< lex::rule::SLINES > assertion( *pnode );
        } else {
            break;
        }
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

Slines parseElse( const node& i_node, Scope& i_symbolTable )
{
    DBG( '!' );
    const ProgramElement< lex::rule::ELSELINE > assertion{ i_node };

    if (const auto nodes = boost::get< node::nodes >( &i_node.value_ )) {
        assert( 1 == nodes->size() );
        return parseSlines( (*nodes)[ 0 ], i_symbolTable );
    }

    assert( lex::type::EPS == boost::get< lex >( i_node.value_ ).type_ );

    return {};
}

If::If( const node& i_node, Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , logic_{ parseLogic( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
    , thenTable_( i_symbolTable.addScope() )
    , then_( parseSlines( boost::get< node::nodes >( i_node.value_ )[ 1 ], thenTable_ ) )
    , elseTable_( i_symbolTable.addScope() )
    , else_( parseElse( boost::get< node::nodes >( i_node.value_ )[ 2 ], elseTable_ ) )
{
    DBG( '!' );
}

While::While( const node& i_node, Scope& i_symbolTable )
    : ProgramElement{ i_node }
    , logic_{ parseLogic( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
    , localTable_( i_symbolTable.addScope() )
    , body_( parseSlines( boost::get< node::nodes >( i_node.value_ )[ 1 ], localTable_ ) )
{
    DBG( '!' );
}

Identifier parseMethodParameter( const node& i_node, MethodScope& i_symbolTable )
{
    DBG( '!' );
    ProgramElement< lex::rule::NEW_IDENTIFIER > assertion{ i_node };

    const auto lexeme = boost::get< lex >( i_node.value_);
    assert( lex::type::IDENTIFIER == lexeme.type_ );
    const auto name = boost::get< std::string >( lexeme.value_ );
    if (!i_symbolTable.nameIsFree( name )) throw std::logic_error{ "Name is already used!" };

    return i_symbolTable.addParameter( name );
}

MethodParameters parseMethodParameters( const node& i_node, MethodScope& i_symbolTable )
{
    DBG( '!' );
    const ProgramElement< lex::rule::MPARAMS > assertion{ i_node };

    MethodParameters result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        DEBUG( nodes->size()  );
        assert( 2 == nodes->size() );

        result.emplace_back( parseMethodParameter( (*nodes)[ 0 ], i_symbolTable ) );

        const ProgramElement< lex::rule::MPARAM_LIST > assertion{ (*nodes)[1] };

        pnode = &(*nodes)[1];
        if (const auto subnodes = boost::get< node::nodes >( &pnode->value_)) {
            assert( subnodes->size() == 1);
            pnode = &(*subnodes)[ 0 ];
            const ProgramElement< lex::rule::MPARAMS > assertion( *pnode );
        } else {
            break;
        }
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

MethodDecl::MethodDecl( const node& i_node, ClassScope& i_symbolTable )
    : ProgramElement{ i_node }
    , name_{ boost::get< std::string >( boost::get< lex >(
                boost::get< node::nodes >( i_node.value_)[ 0 ].value_
                    ).value_) }
    , methodScope_( i_symbolTable.addMethod( name_) )
    , params_( parseMethodParameters( boost::get< node::nodes >( i_node.value_)[ 1 ], methodScope_) )
    , body_( parseSlines( boost::get< node::nodes >( i_node.value_)[ 2 ], methodScope_.scope()))
{
    DBG( '!' );
}

MethodDecls parseMethodDecls( const node& i_node, ClassScope& i_symbolTable )
{
    DBG( '!' );
    const ProgramElement< lex::rule::METHODS > assertion{ i_node };

    MethodDecls result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( (*nodes)[ 0 ], i_symbolTable );

        const ProgramElement< lex::rule::METHOD_LIST > assertion{ (*nodes)[1] };

        pnode = &(*nodes)[1];
        if (const auto subnodes = boost::get< node::nodes >( &pnode->value_)) {
            assert( subnodes->size() == 1);
            pnode = &(*subnodes)[ 0 ];
            const ProgramElement< lex::rule::METHODS > assertion( *pnode );
        } else {
            break;
        }
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

ClassDecl::ClassDecl( const node& i_node, GlobalScope& i_scope )
    : ProgramElement{ i_node }
    , name_{ boost::get< std::string >( boost::get< lex >(
                boost::get< node::nodes >( i_node.value_)[ 0 ].value_
                    ).value_) }
    , classScope_( i_scope.addClass( name_ ) )
    , methods_{ parseMethodDecls( boost::get< node::nodes >( i_node.value_)[1], classScope_) }
{
    DBG( '!' );
}

ClassDecls parseClassDecls( const node& i_node, GlobalScope& i_scope )
{
    DBG( '!' );
    const ProgramElement< lex::rule::CLASSES > assertion{ i_node };

    ClassDecls result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( (*nodes)[ 0 ], i_scope );

        const ProgramElement< lex::rule::CLASS_LIST > assertion{ (*nodes)[1] };
        pnode = &(*nodes)[1];
        if (const auto subnodes = boost::get< node::nodes >( &pnode->value_)) {
            assert( subnodes->size() == 1);
            pnode = &(*subnodes)[ 0 ];
            const ProgramElement< lex::rule::CLASSES > assertion( *pnode );
        } else {
            break;
        }
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

Program::Program( const node& i_node )
    : ProgramElement{ i_node }
    , scope_{}
    , classes_{ parseClassDecls( boost::get< node::nodes >( i_node.value_ )[ 0 ], scope_ ) }
    , run_{ boost::get< node::nodes >( i_node.value_ )[ 1 ], Scope{} }
{
    DBG( '!' );
}

std::ostream& operator<<( std::ostream& out, const GlobalScope& scope )
{
    for (const auto& p : scope.classes_) {
        out << "Class[[[ " << p.first << '\n';
        out << p.second;
        out << "Class]]] " << p.first << '\n';
    }
    return out;
}
std::ostream& operator<<( std::ostream& out, const ClassScope& scope )
{
    for (const auto& p : scope.methods_) {
        out << "\tMethod[[[ " << p.first << '\n';
        out << p.second;
        out << "\tMethod]]] " << p.first << '\n';
    }
    return out;
}
std::ostream& operator<<( std::ostream& out, const MethodScope& scope )
{
    out << "\t\tParameters: ";
    for (const auto& p : scope.parameters_ )
        out << p << "  ";
    out << '\n';
    out << *scope.local_;

    return out;
}
class ScopePrinter
{
public:
    void print( const Scope& root )
    {
        p_printLevel();

        out_ << "Local variables: ";
        for (const auto& p : root.local_ )
            out_ << p << "  ";
        out_ << '\n';

        for (const auto& p : root.inner_ ) {
            p_printLevel();
            out_ << "Inner block[[[ \n";
            ++level_;
            print( p );
            --level_;
            p_printLevel();
            out_ << "Inner block]]] \n";
        }
    }

    ScopePrinter( std::ostream& i_out ) : out_( i_out ) { }
private:
    uint level_ = 2;
    std::ostream& out_;

    void p_printLevel() const
    {
        for (uint i = 0; level_ != i; ++i) {
            out_ << "\t";
        }
    }
};

std::ostream& operator<<( std::ostream& out, const Scope& scope )
{
    ScopePrinter{ out }.print( scope );

    return out;
}

uint Triad::getNextCounter()
{
    static uint COUNTER = 0;

    return ++COUNTER;
}

Triad::Triad( const std::string& i_op, const std::string& i_lhs, const std::string& i_rhs )
    : no_{ getNextCounter() }
    , op_{ i_op }
    , lhs_{ i_lhs }
    , rhs_{ i_rhs }
{
}

// Triad::Triad( const std::string& i_comment1, const std::string& i_comment2, const bool )
//     : op_{ i_comment1 }
//     , lhs_{ i_comment2 }
// {
// }

bool operator<( const Triad& lhs, const Triad& rhs )
{
    return lhs.no_ < rhs.no_;
}

ProgramCode& ProgramCode::instance()
{
    static ProgramCode INSTANCE;

    return INSTANCE;
}

ProgramCode::Line
ProgramCode::addTriad( const std::string& i_op, const std::string& i_lhs, const std::string& i_rhs )
{
    const auto result = code_.emplace( i_op, i_lhs, i_rhs );

    if (!result.second)
        throw std::logic_error{ "Cannot add element to set!" };

    return result.first;
}

void
ProgramCode::addComment( const std::string& i_comment1, const std::string& i_comment2 )
{
    const auto result = code_.emplace( i_comment1, i_comment2, "<<<>>>" );

    if (!result.second)
        throw std::logic_error{ "Cannot add element to set!" };
}

uint ProgramCode::size() const
{
    return code_.size();
}

void ProgramCode::reset()
{
    code_.clear();
}

uint addTriad( const std::string& i_op, const std::string& i_lhs = "", const std::string& i_rhs = "")
{
    return ProgramCode::instance().addTriad( i_op, i_lhs, i_rhs )->no_;
}

uint OperandDouble::operator()() const
{
    if (const auto id = boost::get< Identifier >( &value_ )) {
        return addTriad( "LOAD", **id );
    } else {
        std::stringstream ss;
        ss << boost::get< double >( value_ );
        return addTriad( "GET", ss.str() );
    }
}

uint OperandBool::operator()() const
{
    if (const auto id = boost::get< Identifier >( &value_ )) {
        return addTriad( "LOAD", **id );
    } else {
        std::stringstream ss;
        ss << (boost::get< bool >( value_ ) ? "true" : "false");
        return addTriad( "GET", ss.str() );
    }
}

uint OperandString::operator()() const
{
    if (const auto id = boost::get< Identifier >( &value_ )) {
        return addTriad( "LOAD", **id );
    } else {
        std::stringstream ss;
        ss << "«" << boost::get< std::string >( value_ ) << "»";
        return addTriad( "GET", ss.str() );
    }
}

std::string getOp( ExprDouble::Op i_op )
{
    switch (i_op) {
        case ExprDouble::Op::PLUS: return "+";
        case ExprDouble::Op::MINUS: return "-";
        case ExprDouble::Op::MULT: return "*";
        case ExprDouble::Op::DIV: return "/";
        default: assert( !"No such OP!" );
    }
}

std::string createLink( const uint n )
{
    std::stringstream ss;
    ss << '<' << n << '>';

    return ss.str();
}

uint ExprDouble::operator()() const
{
    const auto lhs = lhs_();
    if ( !(op_ || rhs_) ) return lhs;

    const auto rhs = rhs_.get()();

    return addTriad( getOp( op_.get() ), createLink( lhs ), createLink( rhs ) );
}

std::string getCmp( Cmp i_cmp )
{
    switch (i_cmp) {
        case Cmp::EQ: return "==";
        case Cmp::NE: return "!=";
        case Cmp::LE: return "<";
        case Cmp::GE: return ">";
        default: assert( !"No such OP!" );
    }
}

uint LogicBool::operator()() const
{
    const auto lhs = lhs_();
    if ( !(cmp_ || rhs_) ) return lhs;

    const auto rhs = rhs_.get()();

    return addTriad( getCmp( cmp_.get() ), createLink( lhs ), createLink( rhs ) );
}

uint LogicDouble::operator()() const
{
    const auto lhs = lhs_();
    const auto rhs = rhs_();

    return addTriad( getCmp( cmp_ ), createLink( lhs ), createLink( rhs ) );
}

uint LogicString::operator()() const
{
    const auto lhs = lhs_();
    const auto rhs = rhs_();

    return addTriad( getCmp( cmp_ ), createLink( lhs ), createLink( rhs ) );
}

uint getLogic( const Logic& logic )
{
    if (const auto d = boost::get< LogicBool >( &logic )) {
        return (*d)();
    }
    if (const auto d = boost::get< LogicString >( &logic )) {
        return (*d)();
    }
    if (const auto d = boost::get< LogicDouble >( &logic )) {
        return (*d)();
    }

    assert( !"Cannot generate code for LOGIC element!" );
}

uint Constructor::operator()() const
{
    return addTriad( "CREATE", name_ );
}

uint getRightside( const Rightside& rhs )
{
    if (const auto d = boost::get< Logic >( &rhs )) {
        return getLogic( *d );
    if (const auto d = boost::get< Input >( &rhs )) {
        return (*d)();
    }
    }
    if (const auto d = boost::get< ExprDouble >( &rhs )) {
        return (*d)();
    }
    if (const auto d = boost::get< Constructor >( &rhs )) {
        return (*d)();
    }
    if (const auto d = boost::get< MethodCall >( &rhs )) {
        return (*d)(); }
    if (const auto d = boost::get< Input >( &rhs )) {
        return (*d)();
    }
    if (const auto d = boost::get< std::string >( &rhs )) {
        std::stringstream ss;
        ss << "«" << *d << "»";
        return addTriad( "GET", ss.str() );
    }

    assert( !"Cannot generate code for RIGHTSIDE element!" );
}

uint Applicable::operator()() const
{
    if (object_) {
        return addTriad( "LOAD", **object_ );
    }
    if (class_) {
        return (*class_)();
    }

    assert( !"Cannot generate code for APPLICABLE!" );
}

void getParameters( const Parameters& i_params )
{
    for (const auto& p : i_params ) {
        addTriad( "PUSH", createLink( getRightside( p ) ) );
    }
}

uint MethodCall::operator()() const
{
    const auto lhs = lhs_();
    getParameters( params_ );
    addTriad( "CALL", createLink( lhs ), method_ );

    return addTriad( "POP" );
}

uint Input::operator()() const
{
    addTriad( "CALL", "SYSTEM", "READ" );
    return addTriad( "POP" );
}

void Assignment::operator()() const
{
    const auto rhs = getRightside( rhs_ );
    addTriad( "STORE", createLink( rhs ), *lhs_.this_ );
}

ProgramCode::Line Break::operator()( const bool insideWhile ) const
{
    if (!insideWhile) throw std::logic_error{ "Cannot Use BREAK HERE!" };
    return ProgramCode::instance().addTriad( "JMP" );
}

void Return::operator()() const
{
    addTriad( "PUSH", createLink( getRightside( rhs_ ) ) );
    addTriad( "RET" );
}

void Print::operator()() const
{
    addTriad( "PUSH", createLink( getRightside( rhs_ ) ) );
    addTriad( "CALL", "SYSTEM", "WRITE" );
}

boost::optional< ProgramCode::Line > getSline( const Sline& sline, const bool inside )
{
    if (const auto d = boost::get< Assignment >( &sline )) {
        (*d)();
    } else if (const auto d = boost::get< MethodCall >( &sline )) {
        (*d)();
    } else if (const auto d = boost::get< Break >( &sline )) {
        return (*d)( inside );
    } else if (const auto d = boost::get< Return >( &sline )) {
        (*d)();
    } else if (const auto d = boost::get< Print >( &sline )) {
        (*d)();
    } else if (const auto d = boost::get< If >( &sline )) {
        (*d)();
    } else if (const auto d = boost::get< While >( &sline )) {
        (*d)();
    } else
        assert( !"Cannot generate code for Sline element!" );

    return {};
}

using Lines = std::deque< ProgramCode::Line >;
Lines getSlines( const Slines& slines, const bool insideWhile = false)
{
    Lines result;
    for (const auto& p : slines ) {
        const auto line = getSline( p, insideWhile );
        if (line) result.push_back( *line );
    }

    return result;
}

void If::operator()() const
{
    const auto logic = getLogic( logic_ );
    auto thenAddr = ProgramCode::instance().addTriad( "JNE", createLink( logic ) );
    getSlines( then_ );
    auto elseAddr = ProgramCode::instance().addTriad( "JMP" );
    getSlines( else_ );
    thenAddr->rhs_ = createLink( elseAddr->no_ + 1 );
    elseAddr->lhs_ = createLink( ProgramCode::instance().size() + 1 );
}

void While::operator()() const
{
    const auto logicAddr = ProgramCode::instance().size() + 1;
    const auto logic = getLogic( logic_ );
    auto bodyAddr = ProgramCode::instance().addTriad( "JNE", createLink( logic ) );
    auto bodyLines = getSlines( body_ );
    const auto loopAddr = addTriad( "JMP", createLink( logicAddr ) );
    bodyAddr->rhs_ = createLink( loopAddr + 1 );

    for (auto line : bodyLines ) {
        line->lhs_ = createLink( loopAddr + 1 );
    }
}

void getMethodParameter( const Identifier id )
{
    const auto n = addTriad( "POP" );
    addTriad( "STORE", createLink( n ), *id );
}

void getMethodParameters( const MethodParameters& ids )
{
    for (const auto id : ids )
        getMethodParameter( id );
}

void MethodDecl::operator()() const
{
    ProgramCode::instance().addComment( "Method: ", name_ );

    getMethodParameters( params_ );
    getSlines( body_ );
    addTriad( "RET" );
}

void getMethodDecls( const MethodDecls& methods )
{
    for (const auto& method : methods)
        method();
}

void ClassDecl::operator()() const
{
    ProgramCode::instance().addComment( "Class: ", name_ );
    getMethodDecls( methods_ );
}

void getClassDecls( const ClassDecls& classes )
{
    for (const auto& c : classes)
        c();
}

void Program::operator()() const
{
    getClassDecls( classes_ );
    run_();
}

std::ostream& operator<<( std::ostream& out, const ProgramCode& lhs )
{
    for (const auto& line : lhs.code_)
        out << line << '\n';

    return out;
}

std::ostream& operator<<( std::ostream& out, const Triad& lhs )
{
    out << lhs.no_ << '\t' << lhs.op_ << '\t' << lhs.lhs_ << '\t' << lhs.rhs_;
    return out;
}
} // namespace sap
