#ifndef __ANTHEM__HIDDEN_PREDICATE_ELIMINATION_H
#define __ANTHEM__HIDDEN_PREDICATE_ELIMINATION_H

#include <anthem/AST.h>
#include <anthem/Context.h>

namespace anthem
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// HiddenPredicateElimination
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void eliminateHiddenPredicates(std::vector<ast::Formula> &completedFormulas, Context &context);

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
