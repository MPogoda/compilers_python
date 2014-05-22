#include <boost/variant.hpp>

#include <unordered_map>
#include <vector>
namespace sap
{
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


} // namespace sap
