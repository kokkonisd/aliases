# aliases
A simple console tool that prints all custom bash/zsh aliases.

---

## how to install _aliases_
To install _aliases_, simply get the [latest version](), decompress it, `cd` into it, then run:

```bash
$ make install
```

## how to uninstall _aliases_
To uninstall _aliases_, cd into its previously downloaded package, then run:

```bash
$ make uninstall
```

---

## how to use _aliases_

By default, _aliases_ searches for custom `alias` definitions in `~/.bashrc` and `~/.zshrc`.
However, you can specify as many files as you want and it will look through them.
For example, if you want to check files `myaliases1` and `myaliases2`, you simply run:

```bash
$ aliases myaliases1 myaliases2
```

and _aliases_ will print all the `alias` definitions in those files.

To get the version number, you can run:

```bash
$ aliases -v
```

To get a comprehensive list of options, you can run:

```bash
$ aliases -h
```
