# Equivalence of Ordered Completions

We consider two simple program, program 1

```
p.
q :- p.
```
and program 2

```
q.
p :- q.
```
It is clear that the two programs have the same stable model: the set `p` and `q`.
We want to check whether their ordered completions are equivalent.

## Usage

```
anthem verify --equivalence ordered-completion right.lp left.lp
```

## Output
Using ordered completion it is not possible to verify this claim as already the direction `OC(P1) -> OC(P2)` results in a timeout:

```
...
> Proving forward_3...
Axioms:
    not less_p_p
    not less_q_q
    less_p_p and less_p_p -> less_p_p
    less_p_p and less_p_q -> less_p_q
    less_p_q and less_q_p -> less_p_p
    less_p_q and less_q_q -> less_p_q
    less_q_p and less_p_p -> less_q_p
    less_q_p and less_p_q -> less_q_q
    less_q_q and less_q_p -> less_q_p
    less_q_q and less_q_q -> less_q_q
    p
    p -> q
    #true
    q -> p and less_p_q
    q
    q -> p
    #true

Conjectures:
    p -> q and less_q_p

> Proving forward_3 ended with a SZS status
Status: Timeout (59962 ms)

> Failure! Anthem was unable to find a proof of the forward theorem. (60043 ms)
```

However, if we instead use normal completion (by adding the flag `--bypass-tightness`) the proof is successful:

```
> Proving forward_0...
Axioms:
    p
    (q -> p) and (p -> q)

Conjectures:
    q

> Proving forward_0 ended with a SZS status
Status: Theorem (34 ms)

> Proving forward_1...
Axioms:
    p
    (q -> p) and (p -> q)
    q

Conjectures:
    (p -> q) and (q -> p)

> Proving forward_1 ended with a SZS status
Status: Theorem (22 ms)

> Success! Anthem found a proof of the forward theorem. (60 ms)
```
