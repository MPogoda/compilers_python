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
SymbolTable::SymbolTable( const SymbolTable& other )
    : variables_{ other.variables_ }
    , classNames_{ other.classNames_ }
    // , methods_{ other.methods_ }
{
    // for (const auto cm : other.classMethods_) {
    //     const auto classNameIndex = std::distance( other.classNames_.cbegin(), cm.first );
    //     const auto methodIndex = std::distance( other.methods_.cbegin(), cm.second );
    //
    //     classMethods_.insert( { std::next( classNames_.cbegin(), classNameIndex )
    //                           , std::next( methods_.cbegin(), methodIndex ) } );
    // }
}


// boost::optional< SymbolTable::Method >
// SymbolTable::getMethod_o( const Identifier i_className, const std::string& i_methodName ) const
// {
//     auto eq_range = classMethods_.equal_range( i_className );
//
//     for (; eq_range.second != eq_range.first; ++eq_range.first ) {
//         if (eq_range.first->second->first == i_methodName)
//             return eq_range.first->second;
//     }
//
//     return boost::optional< Method >{};
// }
//
// SymbolTable::Method
// SymbolTable::getMethod( const Identifier i_className, const std::string& i_methodName ) const
// {
//     auto result = getMethod_o( i_className, i_methodName );
//     if (!result)
//         throw std::logic_error{ "No such method!" };
//
//     return result.get();
// }

boost::optional< SymbolTable::Identifier >
SymbolTable::getClassName_o( const std::string& i_name ) const
{
    const Identifier result = std::find( classNames_.cbegin(), classNames_.cend(), i_name );

    if (classNames_.cend() == result)
        return boost::optional< Identifier >{};
    return result;
}

SymbolTable::Identifier
SymbolTable::getClassName( const std::string& i_name ) const
{
    const auto result = getClassName_o( i_name );
    if (!result)
        throw std::logic_error{ "No such class!" };

    return result.get();
}

boost::optional< SymbolTable::Identifier >
SymbolTable::getVariable_o( const std::string& i_name ) const
{
    const Identifier result = std::find( variables_.cbegin(), variables_.cend(), i_name );

    if (variables_.cend() == result)
        return boost::optional< Identifier >{};
    return result;
}

SymbolTable::Identifier
SymbolTable::getVariable( const std::string& i_name ) const
{
    auto result = getVariable_o( i_name );
    if (!result)
        throw std::logic_error{ "No such variable!" };

    return result.get();
}

OperandDouble::OperandDouble( const node& i_node, const SymbolTable& i_symbolTable )
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

OperandBool::OperandBool( const node& i_node, const SymbolTable& i_symbolTable )
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

OperandString::OperandString( const node& i_node, const SymbolTable& i_symbolTable )
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

ExprDouble::Op parseOp( const node& op_node )
{
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

ExprDouble::ExprDouble( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement( i_node )
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
{
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

LogicBool::LogicBool( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement( i_node )
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
{
    const auto& nodes = boost::get< node::nodes >( i_node.value_ );

    if (nodes.size() == 3) {
        cmp_ = parseCmp( nodes[ 1 ] );

        rhs_ = OperandBool{  nodes[ 2 ], i_symbolTable };
    } else if (nodes.size() != 1) {
        throw std::logic_error{ "Wrong number of lexemes in expression!" };
    }
}

LogicDouble::LogicDouble( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement( i_node )
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
    , cmp_{ parseCmp( boost::get< node::nodes >( i_node.value_ ).at( 1 ) ) }
    , rhs_{ boost::get< node::nodes >( i_node.value_ ).at( 2 ), i_symbolTable }
{
    if ( boost::get< node::nodes >( i_node.value_ ).size() != 3) {
        throw std::logic_error{ "Wrong number of lexemes in expression!" };
    }
}

LogicString::LogicString( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement( i_node )
    , lhs_{ boost::get< node::nodes >( i_node.value_ ).at( 0 ), i_symbolTable }
    , cmp_{ parseCmp( boost::get< node::nodes >( i_node.value_ ).at( 1 ) ) }
    , rhs_{ boost::get< node::nodes >( i_node.value_ ).at( 2 ), i_symbolTable }
{
    if ( boost::get< node::nodes >( i_node.value_ ).size() != 3) {
        throw std::logic_error{ "Wrong number of lexemes in expression!" };
    }
}

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

Constructor parseConstructor( const node& i_node, const SymbolTable& i_symbolTable );

Rightside parseRightside( const node& i_node, const SymbolTable& i_symbolTable )
{
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
                return parseConstructor( child, i_symbolTable );
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
        return boost::get< std::string >( i_node.value_ );
    }
}

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

Applicable::Applicable( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement( i_node )
{
    if (const auto nodes = boost::get< node::nodes >( &i_node.value_ )) {
        // it's a constructor
        assert( 1 == nodes->size() );
        class_ = parseConstructor( (*nodes)[ 0 ], i_symbolTable );
    } else {
        const lex id = boost::get< lex >( i_node.value_ );
        assert( lex::type::IDENTIFIER == id.type_ );

        object_ = i_symbolTable.getVariable( boost::get< std::string >( id.value_ ) );
    }
}

MethodCall::MethodCall( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement( i_node )
    , lhs_{ boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable }
{
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
    assert( lex::type::EPS == boost::get< lex >( i_node.value_ ).type_ );
}

SymbolTable::Identifier
SymbolTable::addVariable( const std::string& i_name )
{
    if (getClassName_o( i_name )) {
        throw std::logic_error{ "Name is used by class!" };
    }

    auto result = getVariable_o( i_name );
    if (result) return result.get();

    variables_.push_back( i_name );
    return std::next( variables_.cbegin(), variables_.size() - 1 );
}

NewVariable::NewVariable( const node& i_node, SymbolTable& i_symbolTable )
    : ProgramElement{ i_node }
{
    const auto& child = boost::get< lex >( i_node.value_ );
    assert( lex::type::IDENTIFIER == child.type_ );
    const auto name = boost::get< std::string >( child.value_ );

    this_ = i_symbolTable.addVariable( name );
}

Assignment::Assignment( const node& i_node, SymbolTable& i_symbolTable )
    : ProgramElement{ i_node }
    , lhs_{ boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable }
    , rhs_{ parseRightside( boost::get< node::nodes >( i_node.value_ )[ 1 ], i_symbolTable ) }
{
}

Break::Break( const node& i_node )
    : ProgramElement{ i_node }
{
    assert( boost::get< lex >( i_node.value_ ).type_ == lex::type::EPS );
}

Return::Return( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement{ i_node }
    , rhs_{ parseRightside( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
{
}

Print::Print( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement{ i_node }
    , rhs_{ parseRightside( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
{
}

Sline parseSline( const node& i_node, SymbolTable& i_symbolTable )
{
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
            // TODO: Add more
        default:
            DEBUG( child.rule_ );
            assert( !"Wrong child node!" );
    }
}

Slines parseSlines( const node& i_node, SymbolTable& i_symbolTable )
{
    const ProgramElement< lex::rule::SLINE > assertion{ i_node };

    Slines result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( parseSline( (*nodes)[ 0 ], i_symbolTable ) );

        const ProgramElement< lex::rule::SLINE_LIST > assertion{ (*nodes)[1] };
        pnode = &(*nodes)[1];
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

Slines parseElse( const node& i_node, SymbolTable& i_symbolTable )
{
    const ProgramElement< lex::rule::ELSELINE > assertion{ i_node };

    if (const auto nodes = boost::get< node::nodes >( &i_node.value_ )) {
        assert( 1 == nodes->size() );
        return parseSlines( (*nodes)[ 0 ], i_symbolTable );
    }

    assert( lex::type::EPS == boost::get< lex >( i_node.value_ ).type_ );

    return {};
}

If::If( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement{ i_node }
    , logic_{ parseLogic( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
    , thenTable_{ i_symbolTable }
    , then_( parseSlines( boost::get< node::nodes >( i_node.value_ )[ 1 ], thenTable_ ) )
    , elseTable_{ i_symbolTable }
    , else_( parseElse( boost::get< node::nodes >( i_node.value_ )[ 2 ], elseTable_ ) )
{
}

While::While( const node& i_node, const SymbolTable& i_symbolTable )
    : ProgramElement{ i_node }
    , logic_{ parseLogic( boost::get< node::nodes >( i_node.value_ )[ 0 ], i_symbolTable ) }
    , localTable_{ i_symbolTable }
    , body_( parseSlines( boost::get< node::nodes >( i_node.value_ )[ 1 ], localTable_ ) )
{
}

SymbolTable::Identifier parseMethodParameter( const node& i_node, SymbolTable& i_symbolTable )
{
    ProgramElement< lex::rule::NEW_IDENTIFIER > assertion{ i_node };

    const auto lexeme = boost::get< lex >( i_node.value_);
    assert( lex::type::IDENTIFIER == lexeme.type_ );
    const auto name = boost::get< std::string >( lexeme.value_ );
    if (i_symbolTable.getVariable_o( name )) throw std::logic_error{ "Name is already used!" };

    return i_symbolTable.addVariable( name );
}

MethodParameters parseMethodParameters( const node& i_node, SymbolTable& i_symbolTable )
{
    const ProgramElement< lex::rule::MPARAMS > assertion{ i_node };

    MethodParameters result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( parseMethodParameter( (*nodes)[ 0 ], i_symbolTable ) );

        const ProgramElement< lex::rule::MPARAM_LIST > assertion{ (*nodes)[1] };
        pnode = &(*nodes)[1];
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

boost::optional< SymbolTable::Identifier >
SymbolTable::getMethod_o( const std::string& i_name ) const
{
    const Identifier result = std::find( methods_.cbegin(), methods_.cend(), i_name );

    if (methods_.cend() == result)
        return boost::optional< Identifier >{};
    return result;
}

SymbolTable::Identifier
SymbolTable::addMethod( const std::string& i_name )
{
    if (getClassName_o( i_name )) {
        throw std::logic_error{ "Name is used by class!" };
    }

    auto result = getMethod_o( i_name );
    if (result) throw std::logic_error{ "Method is already declared!" };

    methods_.push_back( i_name );
    return std::next( methods_.cbegin(), methods_.size() - 1 );
}

MethodDecl::MethodDecl( const node& i_node, SymbolTable& i_symbolTable )
    : ProgramElement{ i_node }
    , name_{ i_symbolTable.addMethod( boost::get< std::string >(
                                boost::get< node::nodes >( i_node.value_)[ 0 ].value_)) }
    , local_{ i_symbolTable }
    , params_( parseMethodParameters( boost::get< node::nodes >( i_node.value_)[ 1 ], local_) )
    , body_( parseSlines( boost::get< node::nodes >( i_node.value_)[ 2 ], local_ ) )
{
}

MethodDecls parseMethodDecls( const node& i_node, SymbolTable& i_symbolTable )
{
    const ProgramElement< lex::rule::METHODS > assertion{ i_node };

    MethodDecls result;

    const node* pnode = &i_node;
    while (const auto* nodes = boost::get< node::nodes >( &pnode->value_)) {
        assert( 2 == nodes->size() );

        result.emplace_back( (*nodes)[ 0 ], i_symbolTable );

        const ProgramElement< lex::rule::METHOD_LIST > assertion{ (*nodes)[1] };
        pnode = &(*nodes)[1];
    }

    assert( lex::type::EPS == boost::get< lex >( pnode->value_ ).type_ );

    return result;
}

} // namespace sap
