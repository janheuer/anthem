#include <anthem/Translation.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <clingo.hh>

#include <anthem/Completion.h>
#include <anthem/Context.h>
#include <anthem/IntegerVariableDetection.h>
#include <anthem/MapDomains.h>
#include <anthem/Simplification.h>
#include <anthem/StatementVisitor.h>
#include <anthem/output/FormatterHumanReadable.h>
#include <anthem/output/FormatterTPTP.h>

namespace anthem
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Translation
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class FormulaType
{
	Axiom,
	Conjecture,
};

////////////////////////////////////////////////////////////////////////////////////////////////////

const auto printFormula =
	[](const auto &value, FormulaType formulaType, Context &context, output::PrintContext &printContext)
	{
		auto &stream = context.logger.outputStream();

		switch (context.outputFormat)
		{
			case OutputFormat::HumanReadable:
				output::print<output::FormatterHumanReadable>(stream, value, printContext);
				break;
			case OutputFormat::TPTP:
			{
				const char *ruleType = "";
				std::string formulaName;

				switch (formulaType)
				{
					case FormulaType::Axiom:
						ruleType = "axiom";
						formulaName = std::string(ruleType) + "_" + std::to_string(printContext.currentFormulaID + 1);
						break;
					case FormulaType::Conjecture:
						ruleType = "conjecture";
						formulaName = std::string(ruleType);
						break;
				}

				stream
					<< output::Keyword("tff")
					<< "(" << output::Function(formulaName.c_str())
					<< ", ";

				switch (formulaType)
				{
					case FormulaType::Axiom:
						stream << output::Keyword(ruleType);
						break;
					case FormulaType::Conjecture:
						stream << output::Keyword(ruleType);
						break;
				}

				stream << ", ";
				output::print<output::FormatterTPTP>(stream, value, printContext);
				stream << ").";

				break;
			}
		}

		printContext.currentFormulaID++;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class SymbolDeclaration>
struct PrintReturnTypeTrait
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
struct PrintReturnTypeTrait<ast::PredicateDeclaration>
{
	static output::ColorStream &print(output::ColorStream &stream,
		const ast::PredicateDeclaration &)
	{
		return (stream << output::Keyword("$o"));
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
struct PrintReturnTypeTrait<ast::FunctionDeclaration>
{
	static output::ColorStream &print(output::ColorStream &stream,
		const ast::FunctionDeclaration &functionDeclaration)
	{
		switch (functionDeclaration.domain)
		{
			case Domain::Integer:
				// TODO: clean up
				return (stream << output::Keyword("object"));
			case Domain::Symbolic:
				return (stream << output::Keyword("$i"));
			default:
				throw TranslationException("only functions with integer return type supported with TPTP currently");
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

const auto printTypeAnnotation =
	[](const auto &symbolDeclaration, Context &context, output::PrintContext &printContext)
	{
		auto &stream = context.logger.outputStream();

		// TODO: clean up
		if (strcmp(symbolDeclaration.name.c_str(), AuxiliaryFunctionNameInteger) == 0
			|| strcmp(symbolDeclaration.name.c_str(), AuxiliaryFunctionNameSymbolic) == 0
			|| strcmp(symbolDeclaration.name.c_str(), AuxiliaryPredicateNameIsInteger) == 0)
		{
			return;
		}

		switch (context.outputFormat)
		{
			case OutputFormat::HumanReadable:
				for (size_t i = 0; i < symbolDeclaration.parameters.size(); i++)
				{
					const auto &parameter = symbolDeclaration.parameters[i];

					if (parameter.domain != Domain::Integer)
						continue;

					stream
						<< output::Keyword("int")
						<< "(" << symbolDeclaration.name
						<< "/" << output::Number(symbolDeclaration.arity())
						<< "@" << output::Number(i + 1)
						<< ")" << std::endl;
				}
				break;
			case OutputFormat::TPTP:
			{
				const auto typeName = std::string("type_") + std::to_string(printContext.currentTypeID + 1);

				stream
					<< output::Keyword("tff")
					<< "(" << output::Function(typeName.c_str())
					<< ", " << output::Keyword("type")
					<< ", (" << symbolDeclaration.name << ": ";

				if (symbolDeclaration.parameters.size() > 1)
					stream << "(";

				for (size_t i = 0; i < symbolDeclaration.parameters.size(); i++)
				{
					if (i > 0)
						stream << " * ";

					// For TPTP, all program variable values are mapped to odd integer numbers, while integer
					// values n are mapped to 2 * n. This trick is necessary to translate variables that can take
					// values of both program and integer variables
					stream << output::Keyword("object");
				}

				if (!symbolDeclaration.parameters.empty()){
                    if (symbolDeclaration.parameters.size() == 1)
                        stream << " > ";
                    else
                        stream << ") > ";
				}

				using PrintReturnTypeTrait = PrintReturnTypeTrait<typename std::remove_cv<
					typename std::remove_reference<decltype(symbolDeclaration)>::type>::type>;
				PrintReturnTypeTrait::print(stream, symbolDeclaration);
				stream << "))." << std::endl;

				break;
			}
		}

		printContext.currentTypeID++;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

void translateCompletion(std::vector<ast::ScopedFormula> &&scopedFormulas, Context &context)
{
	assert(context.semantics == Semantics::ClassicalLogic);

	output::PrintContext printContext(context);

	const auto performSimplification = (context.performSimplification && context.semantics == Semantics::ClassicalLogic);

	if (!context.performCompletion)
	{
		// Simplify output if specified
		if (performSimplification)
			for (auto &scopedFormula : scopedFormulas)
				simplify(scopedFormula.formula);

		if (context.showStatementsUsed)
			context.logger.log(output::Priority::Warning) << "#show statements are ignored because completion is not enabled";

		if (context.externalStatementsUsed)
			context.logger.log(output::Priority::Warning) << "#external statements are ignored because completion is not enabled";

		for (const auto &scopedFormula : scopedFormulas)
		{
			printFormula(scopedFormula.formula, FormulaType::Axiom, context, printContext);
			context.logger.outputStream() << std::endl;
		}

		return;
	}

	// Perform completion
	auto completedFormulas = complete(std::move(scopedFormulas), context);

	for (const auto &predicateDeclaration : context.predicateDeclarations)
	{
		if (predicateDeclaration->isUsed)
			continue;

		// Check for #show statements with undeclared predicates
		if (predicateDeclaration->visibility != ast::PredicateDeclaration::Visibility::Default)
			context.logger.log(output::Priority::Warning)
				<< "#show declaration of “"
				<< predicateDeclaration->name
				<< "/"
				<< predicateDeclaration->arity()
				<< "” does not match any declared predicate";

		// Check for #external statements with undeclared predicates
		if (predicateDeclaration->isExternal && !predicateDeclaration->isUsed)
			context.logger.log(output::Priority::Warning)
				<< "#external declaration of “"
				<< predicateDeclaration->name
				<< "/"
				<< predicateDeclaration->arity()
				<< "” does not match any declared predicate";
	}

	// Detect integer variables
	if (context.performIntegerDetection)
		detectIntegerVariables(completedFormulas);

	// Simplify output if specified
	if (performSimplification)
		for (auto &completedFormula : completedFormulas)
			simplify(completedFormula);

	// Print specifiers for integer predicate parameters
	for (auto &predicateDeclaration : context.predicateDeclarations)
	{
		// Check that the predicate is used and not declared #external
		if (!predicateDeclaration->isUsed || predicateDeclaration->isExternal)
			continue;

		const auto isPredicateVisible =
			(predicateDeclaration->visibility == ast::PredicateDeclaration::Visibility::Visible)
			|| (predicateDeclaration->visibility == ast::PredicateDeclaration::Visibility::Default
				&& context.defaultPredicateVisibility == ast::PredicateDeclaration::Visibility::Visible);

		// If the predicate ought to be visible, don’t eliminate it
		if (!isPredicateVisible)
			continue;

		printTypeAnnotation(*predicateDeclaration, context, printContext);
	}

	// TODO: remove variables that are not referenced after simplification

	for (const auto &completedFormula : completedFormulas)
	{
		printFormula(completedFormula, FormulaType::Axiom, context, printContext);
		context.logger.outputStream() << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// replace every predicate by primed version
struct ReplacePredicatesVisitor
{
    static void visit(ast::And &and_, ast::Formula &, Context &context)
    {
        for (auto &argument : and_.arguments)
        {
            argument.accept(ReplacePredicatesVisitor(), argument, context);
        }
    }

    static void visit(ast::Exists &exists, ast::Formula &, Context &context)
    {
        exists.argument.accept(ReplacePredicatesVisitor(), exists.argument, context);
    }

    static void visit(ast::ForAll &forAll, ast::Formula &, Context &context)
    {
        forAll.argument.accept(ReplacePredicatesVisitor(), forAll.argument, context);
    }

    static void visit(ast::Implies &implies, ast::Formula &, Context &context)
    {
        implies.antecedent.accept(ReplacePredicatesVisitor(), implies.antecedent, context);
        implies.consequent.accept(ReplacePredicatesVisitor(), implies.consequent, context);
    }

    static void visit(ast::Not &not_, ast::Formula &, Context &context)
    {
        not_.argument.accept(ReplacePredicatesVisitor(), not_.argument, context);
    }

    static void visit(ast::Or &or_, ast::Formula &, Context &context)
    {
        for (auto &argument : or_.arguments)
        {
            argument.accept(ReplacePredicatesVisitor(), argument, context);
        }
    }

    static void visit(ast::Predicate &predicate, ast::Formula &formula, Context &context)
    {
        ast::Predicate primePredicate(context.findOrCreatePrimePredicateDeclaration(&(predicate.declaration->name)[0], predicate.declaration->parameters.size(), context));
        primePredicate.arguments = prepareCopy(predicate.arguments);
        formula = std::move(primePredicate);
    }

    static void visit(ast::Biconditional &, ast::Formula &, Context &)
    {
    }

    static void visit(ast::Boolean &, ast::Formula &, Context &)
    {
    }

    static void visit(ast::Comparison &, ast::Formula &, Context &)
    {
    }

    static void visit(ast::In &, ast::Formula &, Context &)
    {
    }

};

// replace negated predicates by primed version
struct ReplaceNegatedPredicatesVisitor
{
    static void visit(ast::And &and_, ast::Formula &, Context &context)
    {
        for (auto &argument : and_.arguments)
        {
            argument.accept(ReplaceNegatedPredicatesVisitor(), argument, context);
        }
    }

    static void visit(ast::Exists &exists, ast::Formula &, Context &context)
    {
        exists.argument.accept(ReplaceNegatedPredicatesVisitor(), exists.argument, context);
    }

    static void visit(ast::ForAll &forAll, ast::Formula &, Context &context)
    {
        forAll.argument.accept(ReplaceNegatedPredicatesVisitor(), forAll.argument, context);
    }

    static void visit(ast::Implies &implies, ast::Formula &, Context &context)
    {
        implies.antecedent.accept(ReplaceNegatedPredicatesVisitor(), implies.antecedent, context);
        implies.consequent.accept(ReplaceNegatedPredicatesVisitor(), implies.consequent, context);
    }

    static void visit(ast::Not &not_, ast::Formula &, Context &context)
    {
        if (not_.argument.is<ast::Predicate>())
            not_.argument.accept(ReplacePredicatesVisitor(), not_.argument, context);
        not_.argument.accept(ReplaceNegatedPredicatesVisitor(), not_.argument, context);
    }

    static void visit(ast::Or &or_, ast::Formula &, Context &context)
    {
        for (auto &argument : or_.arguments)
        {
            argument.accept(ReplaceNegatedPredicatesVisitor(), argument, context);
        }
    }

    static void visit(ast::Predicate &, ast::Formula &, Context &)
    {
    }

    static void visit(ast::Biconditional &, ast::Formula &, Context &)
    {
    }

    static void visit(ast::Boolean &, ast::Formula &, Context &)
    {
    }

    static void visit(ast::Comparison &, ast::Formula &, Context &)
    {
    }

    static void visit(ast::In &, ast::Formula &, Context &)
    {
    }

};

////////////////////////////////////////////////////////////////////////////////////////////////////

void translateHereAndThere(std::vector<ast::ScopedFormula> &&scopedFormulasA,
	std::optional<std::vector<ast::ScopedFormula>> &&scopedFormulasB, Context &context)
{
	output::PrintContext printContext(context);
	auto &stream = context.logger.outputStream();

    std::vector<ast::ScopedFormula> primeAxioms;

	switch (context.semantics)
	{
		case Semantics::ClassicalLogic:
			context.logger.log(output::Priority::Info) << "output semantics: classical logic";

			break;
		case Semantics::LogicOfHereAndThere:
			context.logger.log(output::Priority::Info) << "mapped to output semantics: classical logic";

			auto size = context.predicateDeclarations.size();
			for (int i = 0; i < static_cast<int>(size); i++)
            {
			    // create primed version of predicate
                auto primePredicateDeclaration = context.findOrCreatePrimePredicateDeclaration(&(context.predicateDeclarations[i]->name)[0], context.predicateDeclarations[i]->parameters.size(), context);

                // create prime axioms (p -> p')

                // get original predicate and prime predicate
                ast::Predicate predicate(context.predicateDeclarations[i].get());
                ast::Predicate primePredicate(primePredicateDeclaration);

                // two cases: 1) predicate with no parameters
                //            2) predicate with parameters
                if (primePredicateDeclaration->parameters.empty())
                {
                    // without parameters just simple implication p -> p'
                    ast::Implies primeAxiom = ast::Implies(std::move(predicate), std::move(primePredicate));
                    ast::ScopedFormula scopedFormula = ast::ScopedFormula(std::move(primeAxiom), {});
                    primeAxioms.emplace_back(std::move(scopedFormula));
                }
                else
                {
                    // when p has parameters we need the universal closure over all parameters
                    // forall X1,..,XN. p(X1,..,XN) -> p'(X1,..,XN)
                    ast::VariableDeclarationPointers parameters;
                    parameters.reserve(primePredicateDeclaration->parameters.size());

                    for (int j = 0; j < static_cast<int>(primePredicateDeclaration->parameters.size()); j++) {
                        parameters.emplace_back(std::make_unique<ast::VariableDeclaration>(ast::VariableDeclaration::Type::Body));
                        parameters.back()->domain = Domain::Symbolic;
                        predicate.arguments.emplace_back(ast::Variable(parameters[j].get()));
                        primePredicate.arguments.emplace_back(ast::Variable(parameters[j].get()));
                    }

                    ast::Implies primeAxiom = ast::Implies(std::move(predicate), std::move(primePredicate));
                    ast::ForAll forAll_ = ast::ForAll(std::move(parameters), std::move(primeAxiom));
                    ast::ScopedFormula scopedFormula = ast::ScopedFormula(std::move(forAll_), {});
                    primeAxioms.emplace_back(std::move(scopedFormula));
                }
            }

			break;
	}

	const auto buildUniversallyClosedFormulas =
		[](std::vector<ast::ScopedFormula> &&scopedFormulas)
		{
			std::vector<ast::Formula> universallyClosedFormulas;
			universallyClosedFormulas.reserve(scopedFormulas.size());

			// Build the universal closure
			for (auto &scopedFormula : scopedFormulas)
			{
				const auto makeUniversallyClosedFormula =
					[&]() -> ast::Formula
					{
						if (scopedFormula.freeVariables.empty())
							return std::move(scopedFormula.formula);

						return ast::ForAll(std::move(scopedFormula.freeVariables), std::move(scopedFormula.formula));
					};

				universallyClosedFormulas.emplace_back(makeUniversallyClosedFormula());
			}

			return universallyClosedFormulas;
		};

    const auto mapToClassicalLogic =
            [](std::vector<ast::Formula> &&formulas, Context &context)
            {
                // duplicate each formula and replace
                //  1) every negated predicate in one copy
                //  2) every predicate in the other copy

                std::vector<ast::Formula> mappedFormulas;
                mappedFormulas.reserve(2*formulas.size());

                for (auto &formula : formulas)
                {
                    // create copy of formula
                    ast::Formula formulaCopy = prepareCopy(formula);

                    // replace negated predicates
                    formula.accept(ReplaceNegatedPredicatesVisitor(), formula, context);
                    mappedFormulas.emplace_back(std::move(formula));
                    // replace all predicates
                    formulaCopy.accept(ReplacePredicatesVisitor(), formulaCopy, context);
                    mappedFormulas.emplace_back(std::move(formulaCopy));
                }

                return mappedFormulas;
            };

	const auto buildFinalFormulas =
		[&]()
		{
			// If we’re just given one program, translate it to individual axioms
			if (!scopedFormulasB)
			{
                if (context.semantics == Semantics::LogicOfHereAndThere)
                    // Additionally map all formulas to classical logic
                    return mapToClassicalLogic(buildUniversallyClosedFormulas(std::move(scopedFormulasA)), context);
                else
                    return buildUniversallyClosedFormulas(std::move(scopedFormulasA));
            }

			// If we’re given two programs A and B, translate them to a conjecture of the form “A <=> B”
			auto universallyClosedFormulasA = buildUniversallyClosedFormulas(std::move(scopedFormulasA));
			auto universallyClosedFormulasB = buildUniversallyClosedFormulas(std::move(scopedFormulasB.value()));

			// Map all formulas to classical logic if needed
			std::vector<ast::Formula> mappedUniversallyClosedFormulasA;
            std::vector<ast::Formula> mappedUniversallyClosedFormulasB;

			if (context.semantics == Semantics::LogicOfHereAndThere)
            {
                mappedUniversallyClosedFormulasA = mapToClassicalLogic(std::move(universallyClosedFormulasA), context);
                mappedUniversallyClosedFormulasB = mapToClassicalLogic(std::move(universallyClosedFormulasB), context);
            }
			else
            {
			    mappedUniversallyClosedFormulasA = std::move(universallyClosedFormulasA);
                mappedUniversallyClosedFormulasB = std::move(universallyClosedFormulasB);
            }

			// Build the conjunctions of all formulas resulting from each program respectively
			ast::And conjunctionA(std::move(mappedUniversallyClosedFormulasA));
			ast::And conjunctionB(std::move(mappedUniversallyClosedFormulasB));

			std::vector<ast::Formula> finalFormulas;
			finalFormulas.reserve(1);
			finalFormulas.emplace_back(ast::Biconditional(std::move(conjunctionA), std::move(conjunctionB)));

			return finalFormulas;
		};

    auto finalPrimeAxioms = buildUniversallyClosedFormulas(std::move(primeAxioms));

	auto finalFormulas = buildFinalFormulas();

	const auto performDomainMapping =
		[&]()
		{
			switch (context.mapToIntegersPolicy)
			{
				case MapToIntegersPolicy::Always:
					return true;
				case MapToIntegersPolicy::Auto:
					return (context.outputFormat == OutputFormat::TPTP);
			}

			throw TranslationException("supposedly unreachable code, please report to the bug tracker");
		};

	// If requested, map both program and integer variables to integers
	if (performDomainMapping())
	{
        for (auto &finalFormula : finalFormulas)
            mapDomains(finalFormula, context);

        for (auto &finalPrimeAxiom : finalPrimeAxioms)
            mapDomains(finalPrimeAxiom, context);
    }

	// Print auxiliary definitions for mapping program and integer variables to even and odd integers
	if (context.outputFormat == OutputFormat::TPTP)
	{
		stream
			<< R"(%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  types
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tff(types, type, object: $tType).
)";
	}

	// Print type annotations for predicate signatures
	for (const auto &predicateDeclaration : context.predicateDeclarations)
		printTypeAnnotation(*predicateDeclaration, context, printContext);

	// Print type annotations for function signatures
	for (const auto &functionDeclaration : context.functionDeclarations)
		printTypeAnnotation(*functionDeclaration, context, printContext);

    for (auto &finalPrimeAxiom : finalPrimeAxioms)
    {
        printFormula(finalPrimeAxiom, FormulaType::Axiom, context, printContext);
        context.logger.outputStream() << std::endl;
    }

	if (context.outputFormat == OutputFormat::TPTP)
	{
		stream
			<< R"(
tff(types, type, (f__integer__: $int > object)).
tff(types, type, (f__symbolic__: $i > object)).

tff(types, type, (f__sum__: (object * object) > object)).
tff(types, type, (f__unary_minus__: object > object)).
tff(types, type, (f__difference__: (object * object) > object)).
tff(types, type, (f__product__: (object * object) > object)).

tff(types, type, (p__is_integer__: object > $o)).
tff(types, type, (p__is_symbolic__: object > $o)).
tff(types, type, (p__less_equal__: (object * object) > $o)).
tff(types, type, (p__less__: (object * object) > $o)).
tff(types, type, (p__greater_equal__: (object * object) > $o)).
tff(types, type, (p__greater__: (object * object) > $o)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  objects: integers vs. symbolics
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tff(type_check, axiom, (![X: object]: (p__is_integer__(X) <=> (?[Y: $int]: (X = f__integer__(Y)))))).
tff(type_check, axiom, (![X: object]: (p__is_symbolic__(X) <=> (?[Y: $i]: (X = f__symbolic__(Y)))))).
tff(type_check, axiom, (![X: object]: (p__is_integer__(X) <~> p__is_symbolic__(X)))).
tff(type_check, axiom, (![X: $int, Y: $int]: ((f__integer__(X) = f__integer__(Y)) => (X = Y)))).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  integer operations
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tff(operations, axiom, (![X1: $int, X2: $int]: (f__sum__(f__integer__(X1), f__integer__(X2)) = f__integer__($sum(X1, X2))))).
tff(operations, axiom, (![X: $int]: (f__unary_minus__(f__integer__(X)) = f__integer__($uminus(X))))).
tff(operations, axiom, (![X1: $int, X2: $int]: (f__difference__(f__integer__(X1), f__integer__(X2)) = f__integer__($difference(X1, X2))))).
tff(operations, axiom, (![X1: $int, X2: $int]: (f__product__(f__integer__(X1), f__integer__(X2)) = f__integer__($product(X1, X2))))).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  object comparisons
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tff(less_equal, axiom, (![X1: $int, X2: $int]: (p__less_equal__(f__integer__(X1), f__integer__(X2)) <=> $lesseq(X1, X2)))).
tff(less_equal, axiom, (![X1: $i, X2: $int]: ~p__less_equal__(f__symbolic__(X1), f__integer__(X2)))).
tff(less_equal, axiom, (![X1: $int, X2: $i]: p__less_equal__(f__integer__(X1), f__symbolic__(X2)))).

tff(less, axiom, (![X1: $int, X2: $int]: (p__less__(f__integer__(X1), f__integer__(X2)) <=> $less(X1, X2)))).
tff(less, axiom, (![X1: $i, X2: $int]: ~p__less__(f__symbolic__(X1), f__integer__(X2)))).
tff(less, axiom, (![X1: $int, X2: $i]: p__less__(f__integer__(X1), f__symbolic__(X2)))).

tff(greater_equal, axiom, (![X1: $int, X2: $int]: (p__greater_equal__(f__integer__(X1), f__integer__(X2)) <=> $greatereq(X1, X2)))).
tff(greater_equal, axiom, (![X1: $i, X2: $int]: p__greater_equal__(f__symbolic__(X1), f__integer__(X2)))).
tff(greater_equal, axiom, (![X1: $int, X2: $i]: ~p__greater_equal__(f__integer__(X1), f__symbolic__(X2)))).

tff(greater, axiom, (![X1: $int, X2: $int]: (p__greater__(f__integer__(X1), f__integer__(X2)) <=> $greater(X1, X2)))).
tff(greater, axiom, (![X1: $i, X2: $int]: p__greater__(f__symbolic__(X1), f__integer__(X2)))).
tff(greater, axiom, (![X1: $int, X2: $i]: ~p__greater__(f__integer__(X1), f__symbolic__(X2)))).
)"
			<< std::endl;
	}

	if (scopedFormulasB)
		assert(finalFormulas.size() == 1);

	const auto formulaType =
		[&scopedFormulasB]()
		{
			if (scopedFormulasB)
				return FormulaType::Conjecture;

			return FormulaType::Axiom;
		};

	// Print translated formulas
	for (auto &finalFormula : finalFormulas)
	{
		printFormula(finalFormula, formulaType(), context, printContext);
		context.logger.outputStream() << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<ast::ScopedFormula> translateSingleStream(const char *fileName, std::istream &stream, Context &context)
{
	context.logger.log(output::Priority::Info) << "reading " << fileName;

	auto fileContent = std::string(std::istreambuf_iterator<char>(stream), {});

	std::vector<ast::ScopedFormula> scopedFormulas;

	const auto translateStatement =
		[&scopedFormulas, &context](const Clingo::AST::Statement &statement)
		{
			statement.data.accept(StatementVisitor(), statement, scopedFormulas, context);
		};

	const auto logger =
		[&context](const Clingo::WarningCode, const char *text)
		{
			context.logger.log(output::Priority::Error) << text;
		};

	Clingo::parse_program(fileContent.c_str(), translateStatement, logger);

	return scopedFormulas;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void translate(const std::vector<std::string> &fileNames, Context &context)
{
	if (fileNames.empty())
		throw TranslationException("no input files specified");

	const auto translateSingleFile =
		[&](const auto &fileName)
		{
			std::ifstream file(fileName, std::ios::in);

			if (!file.is_open())
				throw LogicException("could not read file “" + fileName + "”");

			return translateSingleStream(fileName.c_str(), file, context);
		};

	switch (context.translationMode)
	{
		case TranslationMode::Completion:
		{
			if (fileNames.size() > 1)
				throw TranslationException("only one file may me translated at a time in completion mode");

			auto scopedFormulas = translateSingleFile(fileNames.front());

			translateCompletion(std::move(scopedFormulas), context);
			break;
		}
		case TranslationMode::HereAndThere:
		{
			if (fileNames.size() > 2)
				throw TranslationException("only one or two files may me translated at a time in here-and-there mode");

			auto scopedFormulasA = translateSingleFile(fileNames.front());
			auto scopedFormulasB = (fileNames.size() > 1)
				? std::optional<std::vector<ast::ScopedFormula>>(translateSingleFile(fileNames[1]))
				: std::nullopt;

			translateHereAndThere(std::move(scopedFormulasA), std::move(scopedFormulasB), context);
			break;
		}
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void translate(const char *fileName, std::istream &stream, Context &context)
{
	auto scopedFormulas = translateSingleStream(fileName, stream, context);

	switch (context.translationMode)
	{
		case TranslationMode::Completion:
		{
			translateCompletion(std::move(scopedFormulas), context);
			break;
		}
		case TranslationMode::HereAndThere:
		{
			translateHereAndThere(std::move(scopedFormulas), std::nullopt, context);
			break;
		}
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
