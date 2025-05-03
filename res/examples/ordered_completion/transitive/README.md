# Transitive Closure on a Finite Graph

We consider the following logic program that computes the transitive closure of a graph given by its edges using the `e/2` predicate
```
t(X,Y) :- e(X,Y).
t(X,Y) :- e(X,Z), t(Z,Y).
```
We add the following facts to represent a simple finite graph
```
e(a1,a2).  e(a2,a1).
```
We want to proof that the stable model of this program consists of the following transitive edges
```
t(a1,a2). t(a1,a1). t(a2,a1). t(a2,a2).
```
In particular if our graph includes some other unconnected node `b` there will be no edges form the `a` nodes to the `b` node.
We combine all this in the following conjecture
```
t(a1,a2) and t(a1,a1) and t(a2,a1) and t(a2,a2) and not t(a1,b) and not t(a2,b).
```

## Usage
```
anthem verify --equivalence ordered-completion --direction forward transitive.lp transitive.spec
```
## Output
Using ordered completion as our program is not (locally) tight we can successfully verify our claim:
```
> Proving forward_0...
Axioms:
    forall X1 X2 not less_e_e(X1, X2, X1, X2)
    forall X1 X2 not less_t_t(X1, X2, X1, X2)
    forall X1 X2 X3 X4 X5 X6 (less_e_e(X1, X2, X3, X4) and less_e_e(X3, X4, X5, X6) -> less_e_e(X1, X2, X5, X6))
    forall X1 X2 X3 X4 X5 X6 (less_e_e(X1, X2, X3, X4) and less_e_t(X3, X4, X5, X6) -> less_e_t(X1, X2, X5, X6))
    forall X1 X2 X3 X4 X5 X6 (less_e_t(X1, X2, X3, X4) and less_t_e(X3, X4, X5, X6) -> less_e_e(X1, X2, X5, X6))
    forall X1 X2 X3 X4 X5 X6 (less_e_t(X1, X2, X3, X4) and less_t_t(X3, X4, X5, X6) -> less_e_t(X1, X2, X5, X6))
    forall X1 X2 X3 X4 X5 X6 (less_t_e(X1, X2, X3, X4) and less_e_e(X3, X4, X5, X6) -> less_t_e(X1, X2, X5, X6))
    forall X1 X2 X3 X4 X5 X6 (less_t_e(X1, X2, X3, X4) and less_e_t(X3, X4, X5, X6) -> less_t_t(X1, X2, X5, X6))
    forall X1 X2 X3 X4 X5 X6 (less_t_t(X1, X2, X3, X4) and less_t_e(X3, X4, X5, X6) -> less_t_e(X1, X2, X5, X6))
    forall X1 X2 X3 X4 X5 X6 (less_t_t(X1, X2, X3, X4) and less_t_t(X3, X4, X5, X6) -> less_t_t(X1, X2, X5, X6))
    forall V1 V2 (e(V1, V2) or exists Z (e(V1, Z) and t(Z, V2)) -> t(V1, V2))
    forall V1 V2 (V1 = a1 and V2 = a2 or V1 = a2 and V2 = a1 -> e(V1, V2))
    forall V1 V2 (t(V1, V2) -> e(V1, V2) and less_e_t(V1, V2, V1, V2) or exists Z (e(V1, Z) and less_e_t(V1, Z, V1, V2) and (t(Z, V2) and less_t_t(Z, V2, V1, V2))))
    forall V1 V2 (e(V1, V2) -> V1 = a1 and V2 = a2 or V1 = a2 and V2 = a1)

Conjectures:
    t(a1, a1) and t(a1, a2) and t(a2, a2) and t(a2, a1) and (t(a1, b) -> #false) and (t(a2, b) -> #false)

> Proving forward_0 ended with a SZS status
Status: Theorem (3838 ms)

> Success! Anthem found a proof of the forward theorem. (3839 ms)
```

If we only use ordinary completion instead of ordered completion (by adding the flag `--bypass-tightness`) the conjecture can not be proven:
```
> Proving forward_0...
Axioms:
    forall V1 V2 (t(V1, V2) -> exists X Y (V1 = X and V2 = Y and exists Z Z1 (Z = X and Z1 = Y and e(Z, Z1))) or exists X Y Z (V1 = X and V2 = Y and (exists Z1 Z2 (Z1 = X and Z2 = Z and e(Z1, Z2)) and exists Z1 Z2 (Z1 = Z and Z2 = Y and t(Z1, Z2)))))
    forall V1 V2 (t(V1, V2) <- exists X Y (V1 = X and V2 = Y and exists Z Z1 (Z = X and Z1 = Y and e(Z, Z1))) or exists X Y Z (V1 = X and V2 = Y and (exists Z1 Z2 (Z1 = X and Z2 = Z and e(Z1, Z2)) and exists Z1 Z2 (Z1 = Z and Z2 = Y and t(Z1, Z2)))))
    forall V1 V2 (e(V1, V2) -> V1 = a1 and V2 = a2 and #true or V1 = a2 and V2 = a1 and #true)
    forall V1 V2 (e(V1, V2) <- V1 = a1 and V2 = a2 and #true or V1 = a2 and V2 = a1 and #true)

Conjectures:
    t(a1, a1) and t(a1, a2) and t(a2, a2) and t(a2, a1) and not t(a1, b) and not t(a2, b)

> Proving forward_0 ended with a SZS status
Status: Timeout (59943 ms)

> Failure! Anthem was unable to find a proof of equivalence. (59950 ms)
```
