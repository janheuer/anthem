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
tff(predicate_0, type, less_q_q: (general * general) > $o).
tff(predicate_1, type, less_p_p: (general * general) > $o).
tff(predicate_2, type, less_r_r: (general * general) > $o).
tff(predicate_3, type, less_q_p: (general * general) > $o).
tff(predicate_4, type, less_q_r: (general * general) > $o).
tff(predicate_5, type, less_p_q: (general * general) > $o).
tff(predicate_6, type, less_p_r: (general * general) > $o).
tff(predicate_7, type, less_r_q: (general * general) > $o).
tff(predicate_8, type, less_r_p: (general * general) > $o).
tff(predicate_9, type, q: (general) > $o).
tff(predicate_10, type, r: (general) > $o).
tff(predicate_11, type, p: (general) > $o).
tff(formula_0_oc_axiom_0, axiom, ![X1_g: general]: (~less_q_q(X1_g, X1_g))).
tff(formula_1_oc_axiom_1, axiom, ![X1_g: general]: (~less_p_p(X1_g, X1_g))).
tff(formula_2_oc_axiom_2, axiom, ![X1_g: general]: (~less_r_r(X1_g, X1_g))).
tff(formula_3_oc_axiom_3, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_q(X1_g, X2_g) & less_q_q(X2_g, X3_g)) => less_q_q(X1_g, X3_g))).
tff(formula_4_oc_axiom_4, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_q(X1_g, X2_g) & less_q_p(X2_g, X3_g)) => less_q_p(X1_g, X3_g))).
tff(formula_5_oc_axiom_5, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_q(X1_g, X2_g) & less_q_r(X2_g, X3_g)) => less_q_r(X1_g, X3_g))).
tff(formula_6_oc_axiom_6, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_p(X1_g, X2_g) & less_p_q(X2_g, X3_g)) => less_q_q(X1_g, X3_g))).
tff(formula_7_oc_axiom_7, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_p(X1_g, X2_g) & less_p_p(X2_g, X3_g)) => less_q_p(X1_g, X3_g))).
tff(formula_8_oc_axiom_8, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_p(X1_g, X2_g) & less_p_r(X2_g, X3_g)) => less_q_r(X1_g, X3_g))).
tff(formula_9_oc_axiom_9, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_r(X1_g, X2_g) & less_r_q(X2_g, X3_g)) => less_q_q(X1_g, X3_g))).
tff(formula_10_oc_axiom_10, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_r(X1_g, X2_g) & less_r_p(X2_g, X3_g)) => less_q_p(X1_g, X3_g))).
tff(formula_11_oc_axiom_11, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_q_r(X1_g, X2_g) & less_r_r(X2_g, X3_g)) => less_q_r(X1_g, X3_g))).
tff(formula_12_oc_axiom_12, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_q(X1_g, X2_g) & less_q_q(X2_g, X3_g)) => less_p_q(X1_g, X3_g))).
tff(formula_13_oc_axiom_13, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_q(X1_g, X2_g) & less_q_p(X2_g, X3_g)) => less_p_p(X1_g, X3_g))).
tff(formula_14_oc_axiom_14, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_q(X1_g, X2_g) & less_q_r(X2_g, X3_g)) => less_p_r(X1_g, X3_g))).
tff(formula_15_oc_axiom_15, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_p(X1_g, X2_g) & less_p_q(X2_g, X3_g)) => less_p_q(X1_g, X3_g))).
tff(formula_16_oc_axiom_16, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_p(X1_g, X2_g) & less_p_p(X2_g, X3_g)) => less_p_p(X1_g, X3_g))).
tff(formula_17_oc_axiom_17, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_p(X1_g, X2_g) & less_p_r(X2_g, X3_g)) => less_p_r(X1_g, X3_g))).
tff(formula_18_oc_axiom_18, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_r(X1_g, X2_g) & less_r_q(X2_g, X3_g)) => less_p_q(X1_g, X3_g))).
tff(formula_19_oc_axiom_19, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_r(X1_g, X2_g) & less_r_p(X2_g, X3_g)) => less_p_p(X1_g, X3_g))).
tff(formula_20_oc_axiom_20, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_p_r(X1_g, X2_g) & less_r_r(X2_g, X3_g)) => less_p_r(X1_g, X3_g))).
tff(formula_21_oc_axiom_21, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_q(X1_g, X2_g) & less_q_q(X2_g, X3_g)) => less_r_q(X1_g, X3_g))).
tff(formula_22_oc_axiom_22, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_q(X1_g, X2_g) & less_q_p(X2_g, X3_g)) => less_r_p(X1_g, X3_g))).
tff(formula_23_oc_axiom_23, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_q(X1_g, X2_g) & less_q_r(X2_g, X3_g)) => less_r_r(X1_g, X3_g))).
tff(formula_24_oc_axiom_24, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_p(X1_g, X2_g) & less_p_q(X2_g, X3_g)) => less_r_q(X1_g, X3_g))).
tff(formula_25_oc_axiom_25, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_p(X1_g, X2_g) & less_p_p(X2_g, X3_g)) => less_r_p(X1_g, X3_g))).
tff(formula_26_oc_axiom_26, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_p(X1_g, X2_g) & less_p_r(X2_g, X3_g)) => less_r_r(X1_g, X3_g))).
tff(formula_27_oc_axiom_27, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_r(X1_g, X2_g) & less_r_q(X2_g, X3_g)) => less_r_q(X1_g, X3_g))).
tff(formula_28_oc_axiom_28, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_r(X1_g, X2_g) & less_r_p(X2_g, X3_g)) => less_r_p(X1_g, X3_g))).
tff(formula_29_oc_axiom_29, axiom, ![X1_g: general, X2_g: general, X3_g: general]: ((less_r_r(X1_g, X2_g) & less_r_r(X2_g, X3_g)) => less_r_r(X1_g, X3_g))).
tff(formula_30_left_0, axiom, ![V1_g: general]: ((q(V1_g) | (r(V1_g) => $false)) => p(V1_g))).
tff(formula_31_left_1, axiom, ![V1_g: general]: (V1_g = f__integer__(1) => r(V1_g))).
tff(formula_32_left_2, axiom, ![V1_g: general]: (V1_g = f__integer__(1) => q(V1_g))).
tff(formula_33_left_3, axiom, ![V1_g: general]: (p(V1_g) => ((q(V1_g) & less_q_p(V1_g, V1_g)) | (r(V1_g) => $false)))).
tff(formula_34_left_4, axiom, ![V1_g: general]: (r(V1_g) => V1_g = f__integer__(1))).
tff(formula_35_left_5, axiom, ![V1_g: general]: (q(V1_g) => V1_g = f__integer__(1))).
tff(formula_36_right_0, conjecture, ![X_g: general]: (p(X_g))).
