#ifndef __ANTHEM__TERM_H
#define __ANTHEM__TERM_H

#include <algorithm>

#include <anthem/AST.h>
#include <anthem/Utils.h>
#include <anthem/output/Formatting.h>

namespace anthem
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Term
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::BinaryOperation::Operator translate(Clingo::AST::BinaryOperator binaryOperator, const Clingo::AST::Term &term, Context &context)
{
	switch (binaryOperator)
	{
		case Clingo::AST::BinaryOperator::Plus:
			return ast::BinaryOperation::Operator::Plus;
		case Clingo::AST::BinaryOperator::Minus:
			return ast::BinaryOperation::Operator::Minus;
		case Clingo::AST::BinaryOperator::Multiplication:
			return ast::BinaryOperation::Operator::Multiplication;
		case Clingo::AST::BinaryOperator::Division:
			return ast::BinaryOperation::Operator::Division;
		case Clingo::AST::BinaryOperator::Modulo:
			return ast::BinaryOperation::Operator::Modulo;
		default:
			throwErrorAtLocation(term.location, "“binary operation” terms currently unsupported", context);
	}

	return ast::BinaryOperation::Operator::Plus;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::Term translate(const Clingo::AST::Term &term, Context &context);

////////////////////////////////////////////////////////////////////////////////////////////////////

struct TermTranslateVisitor
{
	std::experimental::optional<ast::Term> visit(const Clingo::Symbol &symbol, const Clingo::AST::Term &term, Context &context)
	{
		switch (symbol.type())
		{
			case Clingo::SymbolType::Number:
				return ast::Term::make<ast::Integer>(symbol.number());
			case Clingo::SymbolType::Infimum:
				return ast::Term::make<ast::SpecialInteger>(ast::SpecialInteger::Type::Infimum);
			case Clingo::SymbolType::Supremum:
				return ast::Term::make<ast::SpecialInteger>(ast::SpecialInteger::Type::Supremum);
			case Clingo::SymbolType::String:
				return ast::Term::make<ast::String>(std::string(symbol.string()));
			case Clingo::SymbolType::Function:
			{
				auto function = ast::Term::make<ast::Function>(symbol.name());
				// TODO: remove workaround
				auto &functionRaw = function.get<ast::Function>();
				functionRaw.arguments.reserve(symbol.arguments().size());

				for (const auto &argument : symbol.arguments())
				{
					auto translatedArgument = visit(argument, term, context);

					if (!translatedArgument)
						throwErrorAtLocation(term.location, "could not translate argument", context);

					functionRaw.arguments.emplace_back(std::move(translatedArgument.value()));
				}

				return std::move(function);
			}
		}

		return std::experimental::nullopt;
	}

	std::experimental::optional<ast::Term> visit(const Clingo::AST::Variable &variable, const Clingo::AST::Term &, Context &context)
	{
		if (strcmp(variable.name, "_") == 0)
		{
			std::string variableName = AnonymousVariablePrefix + std::to_string(context.anonymousVariableID);
			context.anonymousVariableID++;

			return ast::Term::make<ast::Variable>(std::move(variableName), ast::Variable::Type::Reserved);
		}

		return ast::Term::make<ast::Variable>(std::string(variable.name), ast::Variable::Type::UserDefined);
	}

	std::experimental::optional<ast::Term> visit(const Clingo::AST::UnaryOperation &, const Clingo::AST::Term &term, Context &context)
	{
		throwErrorAtLocation(term.location, "“unary operation” terms currently unsupported", context);
		return std::experimental::nullopt;
	}

	std::experimental::optional<ast::Term> visit(const Clingo::AST::BinaryOperation &binaryOperation, const Clingo::AST::Term &term, Context &context)
	{
		const auto operator_ = translate(binaryOperation.binary_operator, term, context);
		auto left = translate(binaryOperation.left, context);
		auto right = translate(binaryOperation.right, context);

		return ast::Term::make<ast::BinaryOperation>(operator_, std::move(left), std::move(right));
	}

	std::experimental::optional<ast::Term> visit(const Clingo::AST::Interval &interval, const Clingo::AST::Term &, Context &context)
	{
		auto left = translate(interval.left, context);
		auto right = translate(interval.right, context);

		return ast::Term::make<ast::Interval>(std::move(left), std::move(right));
	}

	std::experimental::optional<ast::Term> visit(const Clingo::AST::Function &function, const Clingo::AST::Term &term, Context &context)
	{
		if (function.external)
			throwErrorAtLocation(term.location, "external functions currently unsupported", context);

		std::vector<ast::Term> arguments;
		arguments.reserve(function.arguments.size());

		for (const auto &argument : function.arguments)
			arguments.emplace_back(translate(argument, context));

		return ast::Term::make<ast::Function>(function.name, std::move(arguments));
	}

	std::experimental::optional<ast::Term> visit(const Clingo::AST::Pool &, const Clingo::AST::Term &term, Context &context)
	{
		throwErrorAtLocation(term.location, "“pool” terms currently unsupported", context);
		return std::experimental::nullopt;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::Term translate(const Clingo::AST::Term &term, Context &context)
{
	auto translatedTerm = term.data.accept(TermTranslateVisitor(), term, context);

	if (!translatedTerm)
		throwErrorAtLocation(term.location, "could not translate term", context);

	return std::move(translatedTerm.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
