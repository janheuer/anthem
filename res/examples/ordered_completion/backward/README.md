# Backward Direction

We consider the logic program
```
p.
q :- p.
```
It is easy to proof that the following claim is a consequence of the ordered completion of our program
```
p and q.
```
However, we are instead interested in proving the reverse statement:
```
F -> OC(P)
```
I.e. that the ordered completion of our program follows from the above formula.

## Usage

```
anthem verify --equivalence ordered-completion left.lp right.spec --direction backward
```

## Output
However this proof is unsuccessful as the last subproblem results in a timeout:
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
    p and q
    p
    p -> q
    #true

Conjectures:
    q -> p and less_p_q

> Proving forward_3 ended with a SZS status
Status: Timeout (59954 ms)

> Failure! Anthem was unable to find a proof of the backward theorem. (60035 ms)
```

If we instead just use normal completion (by adding the flag `--bypass-tightness`) we can verify our claim:
```
> Proving forward_0...
Axioms:
    p and q

Conjectures:
    p

> Proving forward_0 ended with a SZS status
Status: Theorem (33 ms)

> Proving forward_1...
Axioms:
    p and q
    p

Conjectures:
    (q -> p) and (p -> q)

> Proving forward_1 ended with a SZS status
Status: Theorem (22 ms)

> Success! Anthem found a proof of the backward theorem. (59 ms)
```
