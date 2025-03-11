tff(general_type, type, general: $tType).
tff(symbol_type, type, symbol: $tType).
tff(f__integer___decl, type, f__integer__: ($int) > general).
tff(f__symbolic___decl, type, f__symbolic__: (symbol) > general).
tff(inf_type, type, c__infimum__: general).
tff(sup_type, type, c__supremum__: general).
tff(p__is_integer__decl, type, p__is_integer__: (general) > $o).
tff(p__is_symbolic__decl, type, p__is_symbolic__: (general) > $o).
tff(p__less_equal__decl, type, p__less_equal__: (general * general) > $o).
tff(p__less__decl, type, p__less__: (general * general) > $o).
tff(p__greater_equal__decl, type, p__greater_equal__: (general * general) > $o).
tff(p__greater__decl, type, p__greater__: (general * general) > $o).
tff(p__is_integer__def_ax, axiom, ![X: general]: (p__is_integer__(X) <=> (?[N: $int]: (X = f__integer__(N))))).
tff(p__is_symbolic__def_ax, axiom, ![X1: general]: (p__is_symbolic__(X1) <=> (?[X2: symbol]: (X1 = f__symbolic__(X2))))).
tff(general_universe_ax, axiom, ![X: general]: ((X = c__infimum__) | p__is_integer__(X) | p__is_symbolic__(X) | (X = c__supremum__))).
tff(f__integer__def_ax, axiom, ![N1: $int, N2: $int]: ((f__integer__(N1) = f__integer__(N2)) <=> (N1 = N2))).
tff(f__symbolic__def_ax, axiom, ![S1: symbol, S2: symbol]: ((f__symbolic__(S1) = f__symbolic__(S2)) <=> (S1 = S2))).
tff(numeral_ordering_ax, axiom, ![N1: $int, N2: $int]: (p__less_equal__(f__integer__(N1), f__integer__(N2)) <=> $lesseq(N1, N2))).
tff(antisymmetric_ordering_ax, axiom, ![X1: general, X2: general]: ((p__less_equal__(X1, X2) & p__less_equal__(X2, X1)) => (X1 = X2))).
tff(transitive_ordering_ax, axiom, ![X1: general, X2: general, X3: general]: ((p__less_equal__(X1, X2) & p__less_equal__(X2, X3)) => p__less_equal__(X1, X3))).
tff(strongly_connected_ordering_ax, axiom, ![X1: general, X2: general]: (p__less_equal__(X1, X2) | p__less_equal__(X2, X1))).
tff(p__less__def_ax, axiom, ![X1: general, X2: general]: (p__less__(X1, X2) <=> (p__less_equal__(X1, X2) & (X1 != X2)))).
tff(p__greater_equal__def_ax, axiom, ![X1: general, X2: general]: (p__greater_equal__(X1, X2) <=> p__less_equal__(X2, X1))).
tff(p__greater__def_ax, axiom, ![X1: general, X2: general]: (p__greater__(X1, X2) <=> (p__less_equal__(X2, X1) & (X1 != X2)))).
tff(minimal_element_ax, axiom, ![N: $int]: p__less__(c__infimum__, f__integer__(N))).
tff(numerals_less_than_symbols_ax, axiom, ![N: $int, S: symbol]: p__less__(f__integer__(N), f__symbolic__(S))).
tff(maximal_element_ax, axiom, ![S: symbol]: p__less__(f__symbolic__(S), c__supremum__)).
tff(predicate_0, type, less_p_p: (general * general) > $o).
tff(predicate_1, type, p: (general) > $o).
tff(formula_0_oc_axiom_0, axiom, ![X1_g: general]: (~less_p_p(X1_g, X1_g))).
tff(formula_1_oc_axiom_1, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_p(X1_g, X2_g) & less_p_p(X2_g, X3_g)) => less_p_p(X1_g, X3_g))).
tff(formula_2_left_0, axiom, ![V1_g: general]: ((?[X_g: general]: (?[I_i: $int]: (V1_g = f__integer__($sum(I_i, 1)) & f__integer__(I_i) = X_g) & (p(X_g) & p__greater__(X_g, f__integer__(0)))) | V1_g = f__integer__(1)) => p(V1_g))).
tff(formula_3_left_1, axiom, ![V1_g: general]: (p(V1_g) => (?[X_g: general]: (?[I_i: $int]: (V1_g = f__integer__($sum(I_i, 1)) & f__integer__(I_i) = X_g) & ((p(X_g) & less_p_p(X_g, V1_g)) & p__greater__(X_g, f__integer__(0)))) | V1_g = f__integer__(1)))).
tff(formula_4_right_0, conjecture, ![X_i: $int]: ($greatereq(X_i, 1) => p(f__integer__(X_i)))).
