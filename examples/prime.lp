#show prime/1.

composite(I * J) :- I=2..n, J=2..n.
prime(N) :- N=2..n, not composite(N).
