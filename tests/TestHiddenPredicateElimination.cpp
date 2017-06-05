#include <catch.hpp>

#include <sstream>

#include <anthem/AST.h>
#include <anthem/Context.h>
#include <anthem/Translation.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[hidden predicate elimination] Hidden predicates are correctly eliminated", "[hidden predicate elimination]")
{
	std::stringstream input;
	std::stringstream output;
	std::stringstream errors;

	anthem::output::Logger logger(output, errors);
	anthem::Context context(std::move(logger));
	context.simplify = true;
	context.complete = true;

	SECTION("simple example (positive 0-ary predicate)")
	{
		input <<
			"p :- q.\n"
			"q.\n"
			"#show p/0.";
		anthem::translate("input", input, context);

		CHECK(output.str() ==
			"(p <-> #true)\n");
	}

	SECTION("simple example (negative 0-ary predicate)")
	{
		input <<
			"p :- q.\n"
			"#show p/0.";
		anthem::translate("input", input, context);

		CHECK(output.str() ==
			"(p <-> #false)\n");
	}

	SECTION("simple example (1-ary predicates, hide all but one)")
	{
		input <<
			"a(X) :- b(X), c(X).\n"
			"b(X) :- not d(X).\n"
			"c(1).\n"
			"c(2).\n"
			"#show a/1.";
		anthem::translate("input", input, context);

		CHECK(output.str() ==
			"forall V1 (a(V1) <-> (not #false and (V1 = 1 or V1 = 2)))\n");
	}

	SECTION("simple example (1-ary predicates, show all explicitly)")
	{
		input <<
			"a(X) :- b(X), c(X).\n"
			"b(X) :- not d(X).\n"
			"c(1).\n"
			"c(2).\n"
			"#show a/1."
			"#show b/1."
			"#show c/1."
			"#show d/1.";
		anthem::translate("input", input, context);

		CHECK(output.str() ==
			"forall V1 (a(V1) <-> (b(V1) and c(V1)))\n"
			"forall V2 (b(V2) <-> not d(V2))\n"
			"forall V3 (c(V3) <-> (V3 = 1 or V3 = 2))\n"
			"forall V4 not d(V4)\n");
	}

	SECTION("simple example (1-ary predicates, show all implicitly)")
	{
		input <<
			"a(X) :- b(X), c(X).\n"
			"b(X) :- not d(X).\n"
			"c(1).\n"
			"c(2).";
		anthem::translate("input", input, context);

		CHECK(output.str() ==
			"forall V1 (a(V1) <-> (b(V1) and c(V1)))\n"
			"forall V2 (b(V2) <-> not d(V2))\n"
			"forall V3 (c(V3) <-> (V3 = 1 or V3 = 2))\n"
			"forall V4 not d(V4)\n");
	}

	SECTION("circular dependencies cannot be fully removed")
	{
		input <<
			"a(X) :- b(X).\n"
			"b(X) :- not c(X).\n"
			"c(X) :- d(X).\n"
			"d(X) :- not b(X).\n"
			"#show a/1.";
		anthem::translate("input", input, context);

		CHECK(output.str() ==
			"forall V1 (a(V1) <-> not d(V1))\n"
			"forall V2 (d(V2) <-> not not d(V2))\n");
	}
}
