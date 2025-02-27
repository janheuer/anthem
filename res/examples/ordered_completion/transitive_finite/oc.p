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
tff(predicate_0, type, less_e_e: (general * general * general * general) > $o).
tff(predicate_1, type, less_t_t: (general * general * general * general) > $o).
tff(predicate_2, type, less_e_t: (general * general * general * general) > $o).
tff(predicate_3, type, less_t_e: (general * general * general * general) > $o).
tff(predicate_4, type, t: (general * general) > $o).
tff(predicate_5, type, e: (general * general) > $o).
tff(type_symbol_0, type, a1: symbol).
tff(type_symbol_1, type, a2: symbol).
tff(type_symbol_2, type, b: symbol).
tff(symbol_order_0, axiom, p__less__(f__symbolic__(a1), f__symbolic__(a2))).
tff(symbol_order_1, axiom, p__less__(f__symbolic__(a2), f__symbolic__(b))).
tff(formula_0_oc_axiom_0, axiom, ![X1_g: general, X2_g: general]: (~less_e_e(X1_g, X2_g, X1_g, X2_g))).
tff(formula_1_oc_axiom_1, axiom, ![X1_g: general, X2_g: general]: (~less_t_t(X1_g, X2_g, X1_g, X2_g))).
tff(formula_2_oc_axiom_2, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_e_e(X1_g, X2_g, X3_g, X4_g) & less_e_e(X3_g, X4_g, X5_g, X6_g)) => less_e_e(X1_g, X2_g, X5_g, X6_g))).
tff(formula_3_oc_axiom_3, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_e_e(X1_g, X2_g, X3_g, X4_g) & less_e_t(X3_g, X4_g, X5_g, X6_g)) => less_e_t(X1_g, X2_g, X5_g, X6_g))).
tff(formula_4_oc_axiom_4, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_e_t(X1_g, X2_g, X3_g, X4_g) & less_t_e(X3_g, X4_g, X5_g, X6_g)) => less_e_e(X1_g, X2_g, X5_g, X6_g))).
tff(formula_5_oc_axiom_5, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_e_t(X1_g, X2_g, X3_g, X4_g) & less_t_t(X3_g, X4_g, X5_g, X6_g)) => less_e_t(X1_g, X2_g, X5_g, X6_g))).
tff(formula_6_oc_axiom_6, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_t_e(X1_g, X2_g, X3_g, X4_g) & less_e_e(X3_g, X4_g, X5_g, X6_g)) => less_t_e(X1_g, X2_g, X5_g, X6_g))).
tff(formula_7_oc_axiom_7, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_t_e(X1_g, X2_g, X3_g, X4_g) & less_e_t(X3_g, X4_g, X5_g, X6_g)) => less_t_t(X1_g, X2_g, X5_g, X6_g))).
tff(formula_8_oc_axiom_8, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_t_t(X1_g, X2_g, X3_g, X4_g) & less_t_e(X3_g, X4_g, X5_g, X6_g)) => less_t_e(X1_g, X2_g, X5_g, X6_g))).
tff(formula_9_oc_axiom_9, axiom, ![X1_g: general, X2_g: general, X3_g: general, X4_g: general, X5_g: general, X6_g: general]: ((less_t_t(X1_g, X2_g, X3_g, X4_g) & less_t_t(X3_g, X4_g, X5_g, X6_g)) => less_t_t(X1_g, X2_g, X5_g, X6_g))).
tff(formula_10_left_0, axiom, ![V1_g: general, V2_g: general]: (t(V1_g, V2_g) <= (?[X_g: general, Y_g: general]: ((V1_g = X_g & V2_g = Y_g) & ?[Z_g: general, Z1_g: general]: ((Z_g = X_g & Z1_g = Y_g) & e(Z_g, Z1_g))) | ?[X_g: general, Y_g: general, Z_g: general]: ((V1_g = X_g & V2_g = Y_g) & (?[Z1_g: general, Z2_g: general]: ((Z1_g = X_g & Z2_g = Z_g) & e(Z1_g, Z2_g)) & ?[Z1_g: general, Z2_g: general]: ((Z1_g = Z_g & Z2_g = Y_g) & t(Z1_g, Z2_g))))))).
tff(formula_11_left_1, axiom, ![V1_g: general, V2_g: general]: (e(V1_g, V2_g) <= (((V1_g = f__symbolic__(a1) & V2_g = f__symbolic__(a2)) & $true) | ((V1_g = f__symbolic__(a2) & V2_g = f__symbolic__(a1)) & $true)))).
tff(formula_12_left_2, axiom, ![V1_g: general, V2_g: general]: (t(V1_g, V2_g) => (?[X_g: general, Y_g: general]: ((V1_g = X_g & V2_g = Y_g) & ?[Z_g: general, Z1_g: general]: ((Z_g = X_g & Z1_g = Y_g) & (e(Z_g, Z1_g) & less_e_t(Z_g, Z1_g, V1_g, V2_g)))) | ?[X_g: general, Y_g: general, Z_g: general]: ((V1_g = X_g & V2_g = Y_g) & (?[Z1_g: general, Z2_g: general]: ((Z1_g = X_g & Z2_g = Z_g) & (e(Z1_g, Z2_g) & less_e_t(Z1_g, Z2_g, V1_g, V2_g))) & ?[Z1_g: general, Z2_g: general]: ((Z1_g = Z_g & Z2_g = Y_g) & (t(Z1_g, Z2_g) & less_t_t(Z1_g, Z2_g, V1_g, V2_g)))))))).
tff(formula_13_left_3, axiom, ![V1_g: general, V2_g: general]: (e(V1_g, V2_g) => (((V1_g = f__symbolic__(a1) & V2_g = f__symbolic__(a2)) & $true) | ((V1_g = f__symbolic__(a2) & V2_g = f__symbolic__(a1)) & $true)))).
tff(formula_14_right_0, conjecture, ((((t(f__symbolic__(a1), f__symbolic__(a1)) & t(f__symbolic__(a1), f__symbolic__(a2))) & t(f__symbolic__(a2), f__symbolic__(a2))) & t(f__symbolic__(a2), f__symbolic__(a1))) & (~t(f__symbolic__(a1), f__symbolic__(b)))) & (~t(f__symbolic__(a2), f__symbolic__(b)))).
