# Simple Tight Program

We consider a simple tight program
```
p(X) :- q(X).
p(X) :- not r(X).

r(1).
p(1).
```
We want to proof that the predicate `p` holds for all possible values
```
forall X p(X).
```

## Usage
```
anthem verify --equivalence ordered-completion --direction forward tight.lp tight.spec
```

## Output
Using ordered completion we can easily verify our claim:
```
> Proving forward_0...
Axioms:
    forall X1 not less_q_q(X1, X1)
    forall X1 not less_p_p(X1, X1)
    forall X1 not less_r_r(X1, X1)
    forall X1 X2 X3 (less_q_q(X1, X2) and less_q_q(X2, X3) -> less_q_q(X1, X3))
    forall X1 X2 X3 (less_q_q(X1, X2) and less_q_p(X2, X3) -> less_q_p(X1, X3))
    forall X1 X2 X3 (less_q_q(X1, X2) and less_q_r(X2, X3) -> less_q_r(X1, X3))
    forall X1 X2 X3 (less_q_p(X1, X2) and less_p_q(X2, X3) -> less_q_q(X1, X3))
    forall X1 X2 X3 (less_q_p(X1, X2) and less_p_p(X2, X3) -> less_q_p(X1, X3))
    forall X1 X2 X3 (less_q_p(X1, X2) and less_p_r(X2, X3) -> less_q_r(X1, X3))
    forall X1 X2 X3 (less_q_r(X1, X2) and less_r_q(X2, X3) -> less_q_q(X1, X3))
    forall X1 X2 X3 (less_q_r(X1, X2) and less_r_p(X2, X3) -> less_q_p(X1, X3))
    forall X1 X2 X3 (less_q_r(X1, X2) and less_r_r(X2, X3) -> less_q_r(X1, X3))
    forall X1 X2 X3 (less_p_q(X1, X2) and less_q_q(X2, X3) -> less_p_q(X1, X3))
    forall X1 X2 X3 (less_p_q(X1, X2) and less_q_p(X2, X3) -> less_p_p(X1, X3))
    forall X1 X2 X3 (less_p_q(X1, X2) and less_q_r(X2, X3) -> less_p_r(X1, X3))
    forall X1 X2 X3 (less_p_p(X1, X2) and less_p_q(X2, X3) -> less_p_q(X1, X3))
    forall X1 X2 X3 (less_p_p(X1, X2) and less_p_p(X2, X3) -> less_p_p(X1, X3))
    forall X1 X2 X3 (less_p_p(X1, X2) and less_p_r(X2, X3) -> less_p_r(X1, X3))
    forall X1 X2 X3 (less_p_r(X1, X2) and less_r_q(X2, X3) -> less_p_q(X1, X3))
    forall X1 X2 X3 (less_p_r(X1, X2) and less_r_p(X2, X3) -> less_p_p(X1, X3))
    forall X1 X2 X3 (less_p_r(X1, X2) and less_r_r(X2, X3) -> less_p_r(X1, X3))
    forall X1 X2 X3 (less_r_q(X1, X2) and less_q_q(X2, X3) -> less_r_q(X1, X3))
    forall X1 X2 X3 (less_r_q(X1, X2) and less_q_p(X2, X3) -> less_r_p(X1, X3))
    forall X1 X2 X3 (less_r_q(X1, X2) and less_q_r(X2, X3) -> less_r_r(X1, X3))
    forall X1 X2 X3 (less_r_p(X1, X2) and less_p_q(X2, X3) -> less_r_q(X1, X3))
    forall X1 X2 X3 (less_r_p(X1, X2) and less_p_p(X2, X3) -> less_r_p(X1, X3))
    forall X1 X2 X3 (less_r_p(X1, X2) and less_p_r(X2, X3) -> less_r_r(X1, X3))
    forall X1 X2 X3 (less_r_r(X1, X2) and less_r_q(X2, X3) -> less_r_q(X1, X3))
    forall X1 X2 X3 (less_r_r(X1, X2) and less_r_p(X2, X3) -> less_r_p(X1, X3))
    forall X1 X2 X3 (less_r_r(X1, X2) and less_r_r(X2, X3) -> less_r_r(X1, X3))
    forall V1 (q(V1) or (r(V1) -> #false) -> p(V1))
    forall V1 (V1 = 1 -> r(V1))
    forall V1 (V1 = 1 -> q(V1))
    forall V1 (p(V1) -> q(V1) and less_q_p(V1, V1) or (r(V1) -> #false))
    forall V1 (r(V1) -> V1 = 1)
    forall V1 (q(V1) -> V1 = 1)

Conjectures:
    forall X p(X)

> Proving forward_0 ended with a SZS status
Status: Theorem (32 ms)

> Success! Anthem found a proof of the forward direction of the equivalence. (40 ms)
```

As our example program is tight it is sufficient to just use the normal completion instead (by adding the flag `--bypass-tightness`):
```
> Proving forward_0...
Axioms:
    forall V1 ((p(V1) -> q(V1) or (r(V1) -> #false)) and (q(V1) or (r(V1) -> #false) -> p(V1)))
    forall V1 ((r(V1) -> V1 = 1) and (V1 = 1 -> r(V1)))
    forall V1 ((q(V1) -> V1 = 1) and (V1 = 1 -> q(V1)))

Conjectures:
    forall X p(X)

> Proving forward_0 ended with a SZS status
Status: Theorem (34 ms)

> Success! Anthem found a proof of the forward direction of the equivalence. (40 ms)
```
