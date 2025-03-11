# Simple Locally-Tight Program

We consider a simple locally tight program
```
p(X+1) :- p(X), X > 0.
p(1).
```
We want to proof that the predicate `p` holds for every integer value greater or equal to one
```
forall X$ (X$ >= 1 -> p(X$)).
```

## Usage
```
anthem verify --equivalence ordered-completion left.lp right.spec
```

## Output
Using ordered completion we can easily verify our claim:
```
> Proving forward_0...
Axioms:
    forall X1 not less_p_p(X1, X1)
    forall X1 X2 X3 (less_p_p(X1, X2) and less_p_p(X2, X3) -> less_p_p(X1, X3))
    forall V1 (exists X (exists I$i (V1 = I$i + 1 and I$i = X) and (p(X) and X > 0)) or V1 = 1 -> p(V1))
    forall V1 (p(V1) -> exists X (exists I$i (V1 = I$i + 1 and I$i = X) and (p(X) and less_p_p(X, V1) and X > 0)) or V1 = 1)

Conjectures:
    forall X$i (X$i >= 1 -> p(X$i))

> Proving forward_0 ended with a SZS status
Status: Theorem (31970 ms)

> Success! Anthem found a proof of the forward direction of the equivalence. (31977 ms)
```

As the program is locally tight it is actually sufficient to just use normal completion instead (by adding the flag `--bypass-tightness`):
```
> Proving forward_0...
Axioms:
    forall V1 ((p(V1) -> exists X (exists I$i (V1 = I$i + 1 and I$i = X) and (p(X) and X > 0)) or V1 = 1) and (exists X (exists I$i (V1 = I$i + 1 and I$i = X) and (p(X) and X > 0)) or V1 = 1 -> p(V1)))

Conjectures:
    forall X$i (X$i >= 1 -> p(X$i))

> Proving forward_0 ended with a SZS status
Status: Theorem (31132 ms)

> Success! Anthem found a proof of the forward theorem. (31140 ms)
```
