# aliases
A simple console tool that prints all custom bash/zsh aliases.

![screenshot](https://i.imgur.com/zi8NUl6.png)

---

## how to install _aliases_
To install _aliases_, simply get the [latest version](https://github.com/kokkonisd/aliases/releases/latest), decompress it, `cd` into it, then run:

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

If you also want to include _function definitions_, you can use the `-f` flag:

```bash
$ aliases -f myaliases1 myaliases2
```

It also works when specifying no file arguments (_aliases_ will look in the default files `~/.bashrc` and `~/.zshrc`):

```bash
$ aliases -f
```

To update _aliases_, you can run:

```bash
$ aliases -u
```

and it will automatically look for the latest version, compare it to the current one you're using and install a newer version if one is available.

To get the version number, you can run:

```bash
$ aliases -v
```

To get a comprehensive list of options, you can run:

```bash
$ aliases -h
```
