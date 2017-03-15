#ifndef __ANTHEM__AST_FORWARD_H
#define __ANTHEM__AST_FORWARD_H

#include <memory>
#include <experimental/optional>
#include <mapbox/variant.hpp>
#include <vector>

namespace anthem
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AST Forward Declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ast
{

////////////////////////////////////////////////////////////////////////////////////////////////////

struct And;
struct BinaryOperation;
struct Biconditional;
struct Boolean;
struct Comparison;
struct Constant;
struct Exists;
struct ForAll;
struct Function;
struct Implies;
struct In;
struct Integer;
struct Interval;
struct Not;
struct Or;
struct Predicate;
struct SpecialInteger;
struct String;
struct Variable;

using AndPointer = std::unique_ptr<And>;
using BinaryOperationPointer = std::unique_ptr<BinaryOperation>;
using BiconditionalPointer = std::unique_ptr<Biconditional>;
using BooleanPointer = std::unique_ptr<Boolean>;
using ComparisonPointer = std::unique_ptr<Comparison>;
using ConstantPointer = std::unique_ptr<Constant>;
using ExistsPointer = std::unique_ptr<Exists>;
using ForAllPointer = std::unique_ptr<ForAll>;
using FunctionPointer = std::unique_ptr<Function>;
using ImpliesPointer = std::unique_ptr<Implies>;
using InPointer = std::unique_ptr<In>;
using IntegerPointer = std::unique_ptr<Integer>;
using IntervalPointer = std::unique_ptr<Interval>;
using NotPointer = std::unique_ptr<Not>;
using OrPointer = std::unique_ptr<Or>;
using PredicatePointer = std::unique_ptr<Predicate>;
using SpecialIntegerPointer = std::unique_ptr<SpecialInteger>;
using StringPointer = std::unique_ptr<String>;
using VariablePointer = std::unique_ptr<Variable>;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Variants
////////////////////////////////////////////////////////////////////////////////////////////////////

using FormulaT = mapbox::util::variant<
	AndPointer,
	BiconditionalPointer,
	BooleanPointer,
	ComparisonPointer,
	ExistsPointer,
	ForAllPointer,
	ImpliesPointer,
	InPointer,
	NotPointer,
	OrPointer,
	PredicatePointer>;

class Formula : public FormulaT
{
	using FormulaT::FormulaT;
};

using TermT = mapbox::util::variant<
	BinaryOperationPointer,
	BooleanPointer,
	ConstantPointer,
	FunctionPointer,
	IntegerPointer,
	IntervalPointer,
	SpecialIntegerPointer,
	StringPointer,
	VariablePointer>;

class Term : public TermT
{
	using TermT::TermT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif