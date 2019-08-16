#include <catch2/catch.hpp>

#include <sstream>

#include <anthem/AST.h>
#include <anthem/Context.h>
#include <anthem/Translation.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[pools as argument in head] Rules containing pools in head are translated correctly", "[pools as argument in head]")
{
	std::stringstream input;
	std::stringstream output;
	std::stringstream errors;

	anthem::output::Logger logger(output, errors);
	anthem::Context context(std::move(logger));
	context.translationMode = anthem::TranslationMode::HereAndThere;
	context.performSimplification = false;
	context.performCompletion = false;

	SECTION("simple example with integers")
	{
		input << "p((1;2;3)).";
		anthem::translate("input", input, context);

		CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3, X4 (X2 = 1 and X3 = 2 and X4 = 3 and (X1 = X2 or X1 = X3 or X1 = X4))) -> p(X1)))\n");
	}

    SECTION("simple example with symbolics")
    {
        input << "p((a;b;c)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3, X4 (X2 = a and X3 = b and X4 = c and (X1 = X2 or X1 = X3 or X1 = X4))) -> p(X1)))\n");
    }

    SECTION("simple example with integers and symbolics")
    {
        input << "p((1;2;a;b)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3, X4, X5 (X2 = 1 and X3 = 2 and X4 = a and X5 = b and (X1 = X2 or X1 = X3 or X1 = X4 or X1 = X5))) -> p(X1)))\n");
    }

    SECTION("interval as argument")
    {
        input << "p((1..5;7;a)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3, X4 (exists N1, N2, N3 (N1 = 1 and N2 = 5 and N1 <= N3 and N3 <= N2 and X2 = N3) and X3 = 7 and X4 = a and "
                                              "(X1 = X2 or X1 = X3 or X1 = X4))) -> p(X1)))\n");
    }

    SECTION("nested pool")
    {
        input << "p(((1;2;3);(a;b;c))).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3 (exists X4, X5, X6 (X4 = 1 and X5 = 2 and X6 = 3 and (X2 = X4 or X2 = X5 or X2 = X6)) and "
                                                                   "exists X7, X8, X9 (X7 = a and X8 = b and X9 = c and (X3 = X7 or X3 = X8 or X3 = X9)) and "
                                              "(X1 = X2 or X1 = X3))) -> p(X1)))\n");
    }

    SECTION("arithmetic in arguments")
    {
        input << "p((1*5;2+2;4/3;-8;9\\2)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3, X4, X5, X6 (exists N1, N2 (X2 = (N1 * N2) and N1 = 1 and N2 = 5) and "
                                                                               "exists N3, N4 (X3 = (N3 + N4) and N3 = 2 and N4 = 2) and "
                                                                               "exists N5, N6, N7, N8 (N5 = ((N6 * N7) + N8) and N5 = 4 and N6 = 3 and N6 != 0 and N8 >= 0 and N8 < N7 and X4 = N7) and "
                                                                               "exists N9 (X5 = -N9 and N9 = 8) and "
                                                                               "exists N10, N11, N12, N13 (N10 = ((N11 * N12) + N13) and N10 = 9 and N11 = 2 and N11 != 0 and N13 >= 0 and N13 < N12 and X6 = N13) and "
                                              "(X1 = X2 or X1 = X3 or X1 = X4 or X1 = X5 or X1 = X6))) -> p(X1)))\n");
    }

    SECTION("symbolic function in argument")
    {
        input << "p((a(1);b(c))).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3 (exists X4 (X2 = a(X4) and X4 = 1) and exists X5 (X3 = b(X5) and X5 = c) and (X1 = X2 or X1 = X3))) -> p(X1)))\n");
    }

    SECTION("tuples in arguments")
    {
        input << "p((1,2;(3,4);(a,))).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> forall X1 ((exists X2, X3, X4 (exists X5, X6 (X2 = (X5, X6) and X5 = 1 and X6 = 2) and "
                                                                       "exists X7, X8 (X3 = (X7, X8) and X7 = 3 and X8 = 4) and "
                                                                       "exists X9 (X4 = (X9,) and X9 = a) and "
                                              "(X1 = X2 or X1 = X3 or X1 = X4))) -> p(X1)))\n");
    }
}

TEST_CASE("[pools as argument in body] Rules containing pools in head are translated correctly", "[pools as argument in body]")
{
    std::stringstream input;
    std::stringstream output;
    std::stringstream errors;

    anthem::output::Logger logger(output, errors);
    anthem::Context context(std::move(logger));
    context.translationMode = anthem::TranslationMode::HereAndThere;
    context.performSimplification = false;
    context.performCompletion = false;

    SECTION("simple example with integers")
    {
        input << "a :- p((1;2;3)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3, X4 (X2 = 1 and X3 = 2 and X4 = 3 and (X1 = X2 or X1 = X3 or X1 = X4)) and p(X1)) -> a)\n");
    }

    SECTION("simple example with symbolics")
    {
        input << "a :- p((a;b;c)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3, X4 (X2 = a and X3 = b and X4 = c and (X1 = X2 or X1 = X3 or X1 = X4)) and p(X1)) -> a)\n");
    }

    SECTION("simple example with integers and symbolics")
    {
        input << "a :- p((1;2;a;b)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3, X4, X5 (X2 = 1 and X3 = 2 and X4 = a and X5 = b and (X1 = X2 or X1 = X3 or X1 = X4 or X1 = X5)) and p(X1)) -> a)\n");
    }

    SECTION("interval as argument")
    {
        input << "a :- p((1..5;7;a)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3, X4 (exists N1, N2, N3 (N1 = 1 and N2 = 5 and N1 <= N3 and N3 <= N2 and X2 = N3) and X3 = 7 and X4 = a and "
                                     "(X1 = X2 or X1 = X3 or X1 = X4)) and p(X1)) -> a)\n");
    }

    SECTION("nested pool")
    {
        input << "a :- p(((1;2;3);(a;b;c))).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3 (exists X4, X5, X6 (X4 = 1 and X5 = 2 and X6 = 3 and (X2 = X4 or X2 = X5 or X2 = X6)) and "
                                                         "exists X7, X8, X9 (X7 = a and X8 = b and X9 = c and (X3 = X7 or X3 = X8 or X3 = X9)) and "
                                     "(X1 = X2 or X1 = X3)) and p(X1)) -> a)\n");
    }

    SECTION("arithmetic in arguments")
    {
        input << "a :- p((1*5;2+2;4/3;-8;9\\2)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3, X4, X5, X6 (exists N1, N2 (X2 = (N1 * N2) and N1 = 1 and N2 = 5) and "
                                                                     "exists N3, N4 (X3 = (N3 + N4) and N3 = 2 and N4 = 2) and "
                                                                     "exists N5, N6, N7, N8 (N5 = ((N6 * N7) + N8) and N5 = 4 and N6 = 3 and N6 != 0 and N8 >= 0 and N8 < N7 and X4 = N7) and "
                                                                     "exists N9 (X5 = -N9 and N9 = 8) and "
                                                                     "exists N10, N11, N12, N13 (N10 = ((N11 * N12) + N13) and N10 = 9 and N11 = 2 and N11 != 0 and N13 >= 0 and N13 < N12 and X6 = N13) and "
                                     "(X1 = X2 or X1 = X3 or X1 = X4 or X1 = X5 or X1 = X6)) and p(X1)) -> a)\n");

    }

    SECTION("symbolic function in arguments")
    {
        input << "a :- p((a(1);b(c))).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3 (exists X4 (X2 = a(X4) and X4 = 1) and exists X5 (X3 = b(X5) and X5 = c) and (X1 = X2 or X1 = X3)) and p(X1)) -> a)\n");
    }

    SECTION("tuples in arguments")
    {
        input << "a :- p((1,2;(3,4);(a,))).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(exists X1 (exists X2, X3, X4 (exists X5, X6 (X2 = (X5, X6) and X5 = 1 and X6 = 2) and "
                                                             "exists X7, X8 (X3 = (X7, X8) and X7 = 3 and X8 = 4) and "
                                                             "exists X9 (X4 = (X9,) and X9 = a) and "
                                     "(X1 = X2 or X1 = X3 or X1 = X4)) and p(X1)) -> a)\n");
    }

    SECTION("pool in comparison")
    {
        input << "p(X) :- X = (a;b;c), (1;2) < (3;4).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "forall U1 ((exists X1, X2 (X1 = U1 and exists X3, X4, X5 (X3 = a and X4 = b and X5 = c and (X2 = X3 or X2 = X4 or X2 = X5)) and X1 = X2) and "
                                                                     "exists X6, X7 (exists X8, X9 (X8 = 1 and X9 = 2 and (X6 = X8 or X6 = X9)) and "
                                                                                    "exists X10, X11 (X10 = 3 and X11 = 4 and (X7 = X10 or X7 = X11)) and X6 < X7)) "
                              "-> forall X12 ((X12 = U1) -> p(X12)))\n");
    }

    SECTION("pool in operation")
    {
        input << "p(X) :- X = (1;2) + (3;4).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "forall U1 (exists X1, X2 (X1 = U1 and exists N1, N2 (X2 = (N1 + N2) and exists X3, X4 (X3 = 1 and X4 = 2 and (N1 = X3 or N1 = X4)) and "
                                                                                                      "exists X5, X6 (X5 = 3 and X6 = 4 and (N2 = X5 or N2 = X6))) and X1 = X2) "
                              "-> forall X7 ((X7 = U1) -> p(X7)))\n");
    }
}

TEST_CASE("[pools in head] Rules containing pools in head are translated correctly", "[pools in head]")
{
    std::stringstream input;
    std::stringstream output;
    std::stringstream errors;

    anthem::output::Logger logger(output, errors);
    anthem::Context context(std::move(logger));
    context.translationMode = anthem::TranslationMode::HereAndThere;
    context.performSimplification = false;
    context.performCompletion = false;

    SECTION("simple example with integers")
    {
        input << "p(1;2;3).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1 ((X1 = 1) -> p(X1)) and "
                                         "forall X2 ((X2 = 2) -> p(X2)) and "
                                         "forall X3 ((X3 = 3) -> p(X3))))\n");
    }

    SECTION("simple example with symbolics")
    {
        input << "p(a;b;c).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1 ((X1 = a) -> p(X1)) and "
                                         "forall X2 ((X2 = b) -> p(X2)) and "
                                         "forall X3 ((X3 = c) -> p(X3))))\n");
    }

    SECTION("simple example with integers and symbolics")
    {
        input << "p(1;2;a;b).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1 ((X1 = 1) -> p(X1)) and "
                                         "forall X2 ((X2 = 2) -> p(X2)) and "
                                         "forall X3 ((X3 = a) -> p(X3)) and "
                                         "forall X4 ((X4 = b) -> p(X4))))\n");
    }

    SECTION("interval as argument")
    {
        input << "p(1..5;7;a).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1 ((exists N1, N2, N3 (N1 = 1 and N2 = 5 and N1 <= N3 and N3 <= N2 and X1 = N3)) -> p(X1)) and "
                                         "forall X2 ((X2 = 7) -> p(X2)) and "
                                         "forall X3 ((X3 = a) -> p(X3))))\n");
    }

    SECTION("nested pool")
    {
        input << "p((1;2;3);(a;b;c)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1 ((exists X2, X3, X4 (X2 = 1 and X3 = 2 and X4 = 3 and (X1 = X2 or X1 = X3 or X1 = X4))) -> p(X1)) and "
                                         "forall X5 ((exists X6, X7, X8 (X6 = a and X7 = b and X8 = c and (X5 = X6 or X5 = X7 or X5 = X8))) -> p(X5))))\n");
    }

    SECTION("arithmetic in arguments")
    {
        input << "p(1*5;2+2;4/3;-8;9\\2).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1 ((exists N1, N2 (X1 = (N1 * N2) and N1 = 1 and N2 = 5)) -> p(X1)) and "
                                         "forall X2 ((exists N3, N4 (X2 = (N3 + N4) and N3 = 2 and N4 = 2)) -> p(X2)) and "
                                         "forall X3 ((exists N5, N6, N7, N8 (N5 = ((N6 * N7) + N8) and N5 = 4 and N6 = 3 and N6 != 0 and N8 >= 0 and N8 < N7 and X3 = N7)) -> p(X3)) and "
                                         "forall X4 ((exists N9 (X4 = -N9 and N9 = 8)) -> p(X4)) and "
                                         "forall X5 ((exists N10, N11, N12, N13 (N10 = ((N11 * N12) + N13) and N10 = 9 and N11 = 2 and N11 != 0 and N13 >= 0 and N13 < N12 and X5 = N13)) -> p(X5))))\n");
    }

    SECTION("symbolic function in argument")
    {
        input << "p(a(1);b(c)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1 ((exists X2 (X1 = a(X2) and X2 = 1)) -> p(X1)) and "
                                         "forall X3 ((exists X4 (X3 = b(X4) and X4 = c)) -> p(X3))))\n");
    }

    SECTION("tuples in arguments")
    {
        input << "p(1,2;(3,4);(a,);).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "(#true -> (forall X1, X2 ((X1 = 1 and X2 = 2) -> p(X1, X2)) and "
                                         "forall X3 ((exists X4, X5 (X3 = (X4, X5) and X4 = 3 and X5 = 4)) -> p(X3)) and "
                                         "forall X6 ((exists X7 (X6 = (X7,) and X7 = a)) -> p(X6)) and p))\n");
    }
}

TEST_CASE("[pools in body] Rules containing pools in head are translated correctly", "[pools in body]")
{
    std::stringstream input;
    std::stringstream output;
    std::stringstream errors;

    anthem::output::Logger logger(output, errors);
    anthem::Context context(std::move(logger));
    context.translationMode = anthem::TranslationMode::HereAndThere;
    context.performSimplification = false;
    context.performCompletion = false;

    SECTION("simple example with integers")
    {
        input << "a :- p(1;2;3).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1 (X1 = 1 and p(X1)) or "
                                "exists X2 (X2 = 2 and p(X2)) or "
                                "exists X3 (X3 = 3 and p(X3))) -> a)\n");
    }

    SECTION("simple example with symbolics")
    {
        input << "a :- p(a;b;c).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1 (X1 = a and p(X1)) or "
                                "exists X2 (X2 = b and p(X2)) or "
                                "exists X3 (X3 = c and p(X3))) -> a)\n");
    }

    SECTION("simple example with integers and symbolics")
    {
        input << "a :- p(1;2;a;b).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1 (X1 = 1 and p(X1)) or "
                                "exists X2 (X2 = 2 and p(X2)) or "
                                "exists X3 (X3 = a and p(X3)) or "
                                "exists X4 (X4 = b and p(X4))) -> a)\n");
    }

    SECTION("interval as argument")
    {
        input << "a :- p(1..5;7;a).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1 (exists N1, N2, N3 (N1 = 1 and N2 = 5 and N1 <= N3 and N3 <= N2 and X1 = N3) and p(X1)) or "
                                "exists X2 (X2 = 7 and p(X2)) or "
                                "exists X3 (X3 = a and p(X3))) -> a)\n");
    }

    SECTION("nested pool")
    {
        input << "a :- p((1;2;3);(a;b;c)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1 (exists X2, X3, X4 (X2 = 1 and X3 = 2 and X4 = 3 and (X1 = X2 or X1 = X3 or X1 = X4)) and p(X1)) or "
                                "exists X5 (exists X6, X7, X8 (X6 = a and X7 = b and X8 = c and (X5 = X6 or X5 = X7 or X5 = X8)) and p(X5))) -> a)\n");
    }

    SECTION("arithmetic in arguments")
    {
        input << "a :- p(1*5;2+2;4/3;-8;9\\2).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1 (exists N1, N2 (X1 = (N1 * N2) and N1 = 1 and N2 = 5) and p(X1)) or "
                                "exists X2 (exists N3, N4 (X2 = (N3 + N4) and N3 = 2 and N4 = 2) and p(X2)) or "
                                "exists X3 (exists N5, N6, N7, N8 (N5 = ((N6 * N7) + N8) and N5 = 4 and N6 = 3 and N6 != 0 and N8 >= 0 and N8 < N7 and X3 = N7) and p(X3)) or "
                                "exists X4 (exists N9 (X4 = -N9 and N9 = 8) and p(X4)) or "
                                "exists X5 (exists N10, N11, N12, N13 (N10 = ((N11 * N12) + N13) and N10 = 9 and N11 = 2 and N11 != 0 and N13 >= 0 and N13 < N12 and X5 = N13) and p(X5))) -> a)\n");

    }

    SECTION("symbolic function in arguments")
    {
        input << "a :- p(a(1);b(c)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1 (exists X2 (X1 = a(X2) and X2 = 1) and p(X1)) or "
                                "exists X3 (exists X4 (X3 = b(X4) and X4 = c) and p(X3))) -> a)\n");
    }

    SECTION("tuples in arguments")
    {
        input << "a :- p(1,2;(3,4);(a,)).";
        anthem::translate("input", input, context);

        CHECK(output.str() == "((exists X1, X2 (X1 = 1 and X2 = 2 and p(X1, X2)) or "
                                "exists X3 (exists X4, X5 (X3 = (X4, X5) and X4 = 3 and X5 = 4) and p(X3)) or "
                                "exists X6 (exists X7 (X6 = (X7,) and X7 = a) and p(X6))) -> a)\n");
    }
}