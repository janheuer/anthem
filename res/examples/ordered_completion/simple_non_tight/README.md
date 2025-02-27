# Simple Non-Tight Program

We consider a simple non-(locally) tight program
```
p(X) :- q(X).
q(X) :- p(X).
```
We want to proof that in every model of this program `p` and `q` are false for every value
```
forall X (not p(X) and not q(X)).
```

## Usage
```
anthem verify --equivalence ordered-completion --no-simplify left.lp right.spec
```

## Output
Using ordered completion we can easily verify our claim:
```
> Proving forward_0...
Axioms:
    forall X1 not less_q_q(X1, X1)
    forall X1 not less_p_p(X1, X1)
    forall X1 X2 X3 (less_q_q(X1, X2) and less_q_q(X2, X3) -> less_q_q(X1, X3))
    forall X1 X2 X3 (less_q_q(X1, X2) and less_q_p(X2, X3) -> less_q_p(X1, X3))
    forall X1 X2 X3 (less_q_p(X1, X2) and less_p_q(X2, X3) -> less_q_q(X1, X3))
    forall X1 X2 X3 (less_q_p(X1, X2) and less_p_p(X2, X3) -> less_q_p(X1, X3))
    forall X1 X2 X3 (less_p_q(X1, X2) and less_q_q(X2, X3) -> less_p_q(X1, X3))
    forall X1 X2 X3 (less_p_q(X1, X2) and less_q_p(X2, X3) -> less_p_p(X1, X3))
    forall X1 X2 X3 (less_p_p(X1, X2) and less_p_q(X2, X3) -> less_p_q(X1, X3))
    forall X1 X2 X3 (less_p_p(X1, X2) and less_p_p(X2, X3) -> less_p_p(X1, X3))
    forall V1 (p(V1) <- exists X (V1 = X and exists Z (Z = X and q(Z))))
    forall V1 (q(V1) <- exists X (V1 = X and exists Z (Z = X and p(Z))))
    forall V1 (p(V1) -> exists X (V1 = X and exists Z (Z = X and (q(Z) and less_q_p(Z, V1)))))
    forall V1 (q(V1) -> exists X (V1 = X and exists Z (Z = X and (p(Z) and less_p_q(Z, V1)))))

Conjectures:
    forall X (not p(X) and not q(X))

> Proving forward_0 ended with a SZS status
Status: Theorem (42 ms)

> Success! Anthem found a proof of equivalence. (48 ms)
```

If we only use ordinary completion instead of ordered completion (by adding the flag `--bypass-tightness`) the conjecture can not be proven:
```
> Proving forward_0...
Axioms:
    forall V1 (p(V1) -> exists X (V1 = X and exists Z (Z = X and q(Z))))
    forall V1 (p(V1) <- exists X (V1 = X and exists Z (Z = X and q(Z))))
    forall V1 (q(V1) -> exists X (V1 = X and exists Z (Z = X and p(Z))))
    forall V1 (q(V1) <- exists X (V1 = X and exists Z (Z = X and p(Z))))

Conjectures:
    forall X (not p(X) and not q(X))

> Proving forward_0 ended with a SZS status
Status: Timeout (59996 ms)

> Failure! Anthem was unable to find a proof of equivalence. (60002 ms)
```


