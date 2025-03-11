# anthem

`anthem` is a command line application for translating answer set programs in the mini-gringo dialect of [clingo](https://potassco.org/clingo/) to first-order theories.
Using an automated theorem prover, these theories can then be used to verify properties of the original programs, such as strong and external equivalence.

Check out the [Manual](https://potassco.org/anthem/) to learn how to install and use `anthem`.

If you want to use `anthem` as a library to build your own application, you can do so.
Check out the [API documentation](https://docs.rs/anthem/) for the available functionalities.

## Ordered Completion Prototype
This branch includes prototypical implementation of the ordered completion.
For details see the master's thesis of Jan Heuer entitled "Ordered Completion for Non-Locally Tight mini-gringo Programs".
Ordered completion is an extension of ordinary completion in order to also capture the semantics of programs that are not (locally) tight.

The `translate` command of `anthem` includes a new option for the `with` argument to choose ordered completion.
To use ordered completion run
```
anthem translate --with ordered-completion theory.spec
```

Note that ordered completion, as is the case with ordinary completion, is applied to a theory.
If you want to obtain the ordered completion of a logic program you should use the `tau-star` translation first as in the following command
```
anthem translate --with tau-star program.lp | anthem translate --with ordered-completion
```

Some example logic programs as well as their ordered completion produced by anthem are included in [`res/examples/orderd_completion/translating`](res/examples/ordered_completion/translating).

This branch also includes a prototypical implementation for verifying the ordered completion of a program.
The inputs for this verification problem are a logic program `Π` and a theory `T`.
Given these inputs anthem tries to verify that the formula `OC(Π) -> T` holds.
This is done with the command
```
anthem verify --equivalence ordered-completion p.lp t.spec
```
This verification mode only supports the `forward` direction.
Passing the argument `--bypass-tightness` changes the verification problem to `Comp(Π) -> T`.

For some example verification problems see the directories `tight`, `non_tight`, `locally_tight` and `transitive_finite` in [`res/examples/ordered_completion/`](res/examples/ordered_completion). See the respective `README.md` files for details on the problems.

## Where's anthem 1?

You're currently looking at version 2 of `anthem`, which is the latest version and the only one that is actively developed.
This is a complete reimplementation crafted by Zach Hansen and Tobias Stolzmann.

Until recently, you would have found Patrick Lühne's version 1 here, which was discontinued and therefore moved to [anthem-1](https://github.com/potassco/anthem-1).
We'd like to thank Patrick for the effort he put into his implementation and the kindness of resolving the naming conflict with us.

## License

`anthem` is distributed under the terms of the MIT license.
See [LICENSE](LICENSE) for details!

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in `anthem` by you shall be licensed as above, without any additional terms or conditions.
