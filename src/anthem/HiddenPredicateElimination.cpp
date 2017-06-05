#include <anthem/HiddenPredicateElimination.h>

#include <anthem/ASTCopy.h>
#include <anthem/ASTUtils.h>
#include <anthem/ASTVisitors.h>
#include <anthem/Exception.h>
#include <anthem/Simplification.h>
#include <anthem/output/AST.h>

namespace anthem
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// HiddenPredicateElimination
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PredicateReplacement
{
	const ast::Predicate &predicate;
	ast::Formula replacement;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

// Replaces all occurrences of a variable in a given term with another variable
struct ReplaceVariableInTermVisitor : public ast::RecursiveTermVisitor<ReplaceVariableInTermVisitor>
{
	static void accept(ast::Variable &variable, ast::Term &, const ast::VariableDeclaration &original, ast::VariableDeclaration &replacement)
	{
		if (variable.declaration == &original)
			// No dangling variables can result from this operation, and hence, fixing them is not necessary
			variable.declaration = &replacement;
	}

	// Ignore all other types of expressions
	template<class T>
	static void accept(T &, ast::Term &, const ast::VariableDeclaration &, ast::VariableDeclaration &)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

// Replaces all occurrences of a variable in a given formula with another variable
struct ReplaceVariableInFormulaVisitor : public ast::RecursiveFormulaVisitor<ReplaceVariableInFormulaVisitor>
{
	static void accept(ast::Comparison &comparison, ast::Formula &, const ast::VariableDeclaration &original, ast::VariableDeclaration &replacement)
	{
		comparison.left.accept(ReplaceVariableInTermVisitor(), comparison.left, original, replacement);
		comparison.right.accept(ReplaceVariableInTermVisitor(), comparison.right, original, replacement);
	}

	static void accept(ast::In &in, ast::Formula &, const ast::VariableDeclaration &original, ast::VariableDeclaration &replacement)
	{
		in.element.accept(ReplaceVariableInTermVisitor(), in.element, original, replacement);
		in.set.accept(ReplaceVariableInTermVisitor(), in.set, original, replacement);
	}

	static void accept(ast::Predicate &predicate, ast::Formula &, const ast::VariableDeclaration &original, ast::VariableDeclaration &replacement)
	{
		for (auto &argument : predicate.arguments)
			argument.accept(ReplaceVariableInTermVisitor(), argument, original, replacement);
	}

	// Ignore all other types of expressions
	template<class T>
	static void accept(T &, ast::Formula &, const ast::VariableDeclaration &, ast::VariableDeclaration &)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

// Replace a predicate in a term with a formula
struct ReplacePredicateInFormulaVisitor : public ast::RecursiveFormulaVisitor<ReplacePredicateInFormulaVisitor>
{
	static void accept(ast::Predicate &predicate, ast::Formula &formula, const PredicateReplacement &predicateReplacement)
	{
		if (!ast::matches(predicate, predicateReplacement.predicate))
			return;

		auto formulaReplacement = ast::prepareCopy(predicateReplacement.replacement);

		for (size_t i = 0; i < predicate.arguments.size(); i++)
		{
			assert(predicateReplacement.predicate.arguments[i].is<ast::Variable>());
			const auto &original = *predicateReplacement.predicate.arguments[i].get<ast::Variable>().declaration;

			assert(predicate.arguments[i].is<ast::Variable>());
			auto &replacement = *predicate.arguments[i].get<ast::Variable>().declaration;

			formulaReplacement.accept(ReplaceVariableInFormulaVisitor(), formulaReplacement, original, replacement);
		}

		formula = std::move(formulaReplacement);
	}

	// Ignore all other types of expressions
	template<class T>
	static void accept(T &, ast::Formula &, const PredicateReplacement &)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

// Detect whether a formula contains a circular dependency on a given predicate
struct DetectCircularDependcyVisitor : public ast::RecursiveFormulaVisitor<DetectCircularDependcyVisitor>
{
	static void accept(ast::Predicate &predicate, ast::Formula &, const ast::PredicateSignature &predicateSignature, bool &hasCircularDependency)
	{
		if (ast::matches(predicate, predicateSignature))
			hasCircularDependency = true;
	}

	// Ignore all other types of expressions
	template<class T>
	static void accept(T &, ast::Formula &, const ast::PredicateSignature &, bool &)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

// Finds the replacement for predicates of the form “forall X1, ..., Xn (p(X1, ..., Xn) <-> ...)”
PredicateReplacement findReplacement(const ast::PredicateSignature &predicateSignature, const ast::ForAll &forAll)
{
	// Declare variable used, only used in debug mode
	(void)(predicateSignature);

	// Form: “forall X1, ..., Xn p(X1, ..., Xn)”
	// Replace with “#true”
	if (forAll.argument.is<ast::Predicate>())
	{
		assert(ast::matches(forAll.argument.get<ast::Predicate>(), predicateSignature));

		return {forAll.argument.get<ast::Predicate>(), ast::Formula::make<ast::Boolean>(true)};
	}

	// Form: “forall X1, ..., Xn (p(X1, ..., Xn) <-> ...)”
	// Replace with “#false”
	if (forAll.argument.is<ast::Not>())
	{
		auto &notArgument = forAll.argument.get<ast::Not>().argument;

		assert(notArgument.is<ast::Predicate>());
		assert(ast::matches(notArgument.get<ast::Predicate>(), predicateSignature));

		return {notArgument.get<ast::Predicate>(), ast::Formula::make<ast::Boolean>(false)};
	}

	assert(forAll.argument.is<ast::Biconditional>());

	const auto &biconditional = forAll.argument.get<ast::Biconditional>();

	assert(biconditional.left.is<ast::Predicate>());
	assert(ast::matches(biconditional.left.get<ast::Predicate>(), predicateSignature));

	// TODO: avoid copy
	return {biconditional.left.get<ast::Predicate>(), ast::prepareCopy(biconditional.right)};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Finds a replacement for a predicate that should be hidden
PredicateReplacement findReplacement(const ast::PredicateSignature &predicateSignature, const ast::Formula &completedPredicateDefinition)
{
	if (completedPredicateDefinition.is<ast::ForAll>())
		return findReplacement(predicateSignature, completedPredicateDefinition.get<ast::ForAll>());
	else if (completedPredicateDefinition.is<ast::Predicate>())
		return {completedPredicateDefinition.get<ast::Predicate>(), ast::Formula::make<ast::Boolean>(true)};
	else if (completedPredicateDefinition.is<ast::Not>())
	{
		const auto &notArgument = completedPredicateDefinition.get<ast::Not>().argument;
		assert(notArgument.is<ast::Predicate>());

		return {notArgument.get<ast::Predicate>(), ast::Formula::make<ast::Boolean>(false)};
	}

	throw CompletionException("invalid completed predicate definition for predicate “" + predicateSignature.name + "/" +  std::to_string(predicateSignature.arity) + "”");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void eliminateHiddenPredicates(const std::vector<ast::PredicateSignature> &predicateSignatures, std::vector<ast::Formula> &completedFormulas, Context &context)
{
	if (!context.visiblePredicateSignatures)
	{
		context.logger.log(output::Priority::Debug) << "no predicates to be eliminated";
		return;
	}

	const auto &visiblePredicateSignatures = context.visiblePredicateSignatures.value();

	// Check for undeclared predicates that are requested to be shown
	for (const auto &visiblePredicateSignature : visiblePredicateSignatures)
	{
		const auto matchesPredicateSignature =
			[&](const auto &predicateSignature)
			{
				return ast::matches(predicateSignature, visiblePredicateSignature);
			};

		const auto matchingPredicateSignature =
			std::find_if(predicateSignatures.cbegin(), predicateSignatures.cend(), matchesPredicateSignature);

		if (matchingPredicateSignature == predicateSignatures.cend())
			context.logger.log(output::Priority::Warning) << "cannot show undeclared predicate “" << visiblePredicateSignature.name << "/" << visiblePredicateSignature.arity <<"”";
	}

	// Replace all occurrences of hidden predicates
	for (size_t i = 0; i < predicateSignatures.size(); i++)
	{
		auto &predicateSignature = predicateSignatures[i];

		const auto matchesVisiblePredicateSignature =
			[&](const auto &visiblePredicateSignature)
			{
				return ast::matches(predicateSignature, visiblePredicateSignature);
			};

		const auto matchingPredicateSignature =
			std::find_if(visiblePredicateSignatures.cbegin(), visiblePredicateSignatures.cend(), matchesVisiblePredicateSignature);

		// If the predicate ought to be visible, don’t eliminate it
		if (matchingPredicateSignature != visiblePredicateSignatures.cend())
			continue;

		context.logger.log(output::Priority::Debug) << "eliminating “" << predicateSignature.name << "/" << predicateSignature.arity << "”";

		const auto &completedPredicateDefinition = completedFormulas[i];
		auto replacement = findReplacement(predicateSignature, completedPredicateDefinition);

		bool hasCircularDependency = false;
		replacement.replacement.accept(DetectCircularDependcyVisitor(), replacement.replacement, predicateSignature, hasCircularDependency);

		if (hasCircularDependency)
		{
			context.logger.log(output::Priority::Warning) << "cannot hide predicate “" << predicateSignature.name << "/" << predicateSignature.arity << "” due to circular dependency";
			continue;
		}

		for (size_t j = 0; j < completedFormulas.size(); j++)
			if (j != i)
				completedFormulas[j].accept(ReplacePredicateInFormulaVisitor(), completedFormulas[j], replacement);

		// TODO: refactor
		completedFormulas[i] = ast::Formula::make<ast::Boolean>(true);
	}

	const auto canBeRemoved =
		[&](const ast::Formula &completedFormula)
		{
			if (!completedFormula.is<ast::Boolean>())
				return false;

			return completedFormula.get<ast::Boolean>().value == true;
		};

	auto removedFormulas = std::remove_if(completedFormulas.begin(), completedFormulas.end(), canBeRemoved);
	completedFormulas.erase(removedFormulas, completedFormulas.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}