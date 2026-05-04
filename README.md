# h - host IP lookup

Tiny CLI to map host aliases to IPs, so you don't have to remember them.

```sh
$ h nk-ams1
195.133.79.129

$ ssh root@$(h nk-ams1)
```

Config lives at `~/.config/h/h.conf` in INI format:

```ini
[servers]
nk-ams1  = 195.133.79.129
nk-ams2  = 185.136.243.124
```

## Install

```sh
mkdir -p ~/bin
curl -fsSL https://raw.githubusercontent.com/minya/h/master/h -o ~/bin/h
chmod +x ~/bin/h
```

Make sure `~/bin` is on your `$PATH`.

On the **first run**, `h` will offer to create:

- `~/.config/h/h.conf` — example config to edit
- `~/.zsh/completions/_h` — zsh completion that pulls aliases from `h --list`

After that, add this to your `.zshrc` once (z.sh-style — `h` won't touch it for you):

```sh
fpath+=~/.zsh/completions
autoload -Uz compinit && compinit
```

Open a new shell, fill in `~/.config/h/h.conf`, and `h <Tab>` will complete your aliases.

## Usage

```
h <alias>     print IP for alias
h --list      print all aliases
h --help      show help
```

## Env vars

- `H_CONFIG`  — override config path (default `~/.config/h/h.conf`)
- `H_COMPDIR` — override completion dir (default `~/.zsh/completions`)
