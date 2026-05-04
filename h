#!/bin/bash

# Map a host alias to an IP, read from ~/.config/h/h.conf (INI).
# Usage: h <alias>   |   h --list

CONFIG="${H_CONFIG:-$HOME/.config/h/h.conf}"
COMPDIR="${H_COMPDIR:-$HOME/.zsh/completions}"

bootstrap() {
    echo "h: first-run setup"
    echo "  config:     $CONFIG"
    echo "  completion: $COMPDIR/_h"
    printf "Create them? [Y/n] "
    read -r ans
    case "$ans" in n|N) exit 1 ;; esac

    mkdir -p "$(dirname "$CONFIG")" "$COMPDIR"

    cat > "$CONFIG" <<'EOF'
[servers]
# alias = 1.2.3.4
EOF

    cat > "$COMPDIR/_h" <<'EOF'
#compdef h

_h() {
    local -a aliases
    aliases=(${(f)"$(command h --list 2>/dev/null)"})
    _describe 'host alias' aliases
}

_h "$@"
EOF

    cat <<EOF

Done. Next steps:
  1. Edit $CONFIG with your hosts.
  2. Add to your .zshrc (if not already there):
       fpath+=$COMPDIR
       autoload -Uz compinit && compinit
  3. Open a new shell.
EOF
    exit 0
}

if [ ! -f "$CONFIG" ]; then
    bootstrap
fi

parse() {
    awk -F= '
        /^[[:space:]]*[#;]/ { next }
        /^[[:space:]]*\[/   { next }
        NF < 2              { next }
        {
            key = $1; sub(/^[[:space:]]+/, "", key); sub(/[[:space:]]+$/, "", key)
            val = $2; sub(/^[[:space:]]+/, "", val); sub(/[[:space:]]+$/, "", val)
            if (key != "") print key "\t" val
        }
    ' "$CONFIG"
}

case "$#" in
    0)
        echo "Usage: $0 <alias>" >&2
        exit 1
        ;;
    1)
        case "$1" in
            -l|--list)
                parse | awk -F'\t' '{print $1}'
                exit 0
                ;;
            -h|--help)
                echo "Usage: $0 <alias>        print IP for alias"
                echo "       $0 --list        print all aliases"
                exit 0
                ;;
        esac
        ;;
    *)
        echo "Usage: $0 <alias>" >&2
        exit 1
        ;;
esac

parse | awk -v k="$1" -F'\t' '$1==k {print $2; found=1} END{exit !found}'
rc=$?
if [ "$rc" -ne 0 ]; then
    echo "Unknown alias: $1" >&2
    exit 1
fi
