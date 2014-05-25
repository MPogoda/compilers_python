#pragma once

#include "lexeme.h"
#include "parse.h"
#include "syntax.h"

#include <vector>
#include <iosfwd>

#include <boost/variant.hpp>

namespace sap
{
struct node
{
    lex::rule rule_;
    using nodes = std::vector< node >;
    using value = boost::variant< lex, nodes >;
    value value_;

    node( Queue& i_queue, LIterator& i_it );

    node() = delete;
};

std::ostream& operator<<( std::ostream& out, const node& root);
} // namespace sap
