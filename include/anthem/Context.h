#ifndef __ANTHEM__CONTEXT_H
#define __ANTHEM__CONTEXT_H

#include <optional>

#include <anthem/AST.h>
#include <anthem/output/Logger.h>
#include <anthem/output/ParenthesisStyle.h>

namespace anthem
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Context
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PredicateDeclarationMeta
{
	ast::PredicateDeclaration *declaration;
	bool used{false};
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Context
{
	Context() = default;

	explicit Context(output::Logger &&logger)
	:	logger{std::move(logger)}
	{
	}

	ast::PredicateDeclaration *findOrCreatePredicateDeclaration(const char *name, size_t arity)
	{
		const auto matchesExistingPredicateDeclaration =
			[&](const auto &predicateDeclaration)
			{
				return (predicateDeclaration->arity == arity
					&& strcmp(predicateDeclaration->name.c_str(), name) == 0);
			};

		auto matchingPredicateDeclaration = std::find_if(predicateDeclarations.begin(),
			predicateDeclarations.end(), matchesExistingPredicateDeclaration);

		if (matchingPredicateDeclaration != predicateDeclarations.end())
			return matchingPredicateDeclaration->get();

		predicateDeclarations.emplace_back(std::make_unique<ast::PredicateDeclaration>(name, arity));

		return predicateDeclarations.back().get();
	}

	ast::FunctionDeclaration *findOrCreateFunctionDeclaration(const char *name, size_t arity)
	{
		const auto matchesExistingFunctionDeclaration =
			[&](const auto &functionDeclarations)
			{
				return (functionDeclarations->arity == arity
					&& strcmp(functionDeclarations->name.c_str(), name) == 0);
			};

		auto matchingFunctionDeclaration = std::find_if(functionDeclarations.begin(),
			functionDeclarations.end(), matchesExistingFunctionDeclaration);

		if (matchingFunctionDeclaration != functionDeclarations.end())
			return matchingFunctionDeclaration->get();

		functionDeclarations.emplace_back(std::make_unique<ast::FunctionDeclaration>(name, arity));

		return functionDeclarations.back().get();
	}

	output::Logger logger;

	bool performSimplification = false;
	bool performCompletion = false;

	std::vector<std::unique_ptr<ast::PredicateDeclaration>> predicateDeclarations;
	ast::PredicateDeclaration::Visibility defaultPredicateVisibility{ast::PredicateDeclaration::Visibility::Visible};

	std::vector<std::unique_ptr<ast::FunctionDeclaration>> functionDeclarations;

	bool externalStatementsUsed{false};
	bool showStatementsUsed{false};

	ast::ParenthesisStyle parenthesisStyle = ast::ParenthesisStyle::Normal;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
