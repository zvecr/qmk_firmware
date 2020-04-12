#!/bin/sh
# NOTE: This script uses tabs for indentation

errcho() {
	echo "$@" >&2
}

USAGE="Usage: $0 [keyboard[:keymap[:target]]]"

# Check preconditions
for arg; do
	if [ "$arg" = "--help" ]; then
		echo "$USAGE"
		exit 0
	fi
done
if [ $# -gt 1 ]; then
	errcho "$USAGE"
	exit 1
elif ! command -v docker >/dev/null 2>&1; then
	errcho "Error: docker not found"
	errcho "See https://docs.docker.com/install/#supported-platforms for installation instructions"
	exit 2
fi

# Determine arguments
if [ $# -eq 0 ]; then
	printf "keyboard=" && read -r keyboard
	[ -n "$keyboard" ] && printf "keymap=" && read -r keymap
	[ -n "$keymap"   ] && printf "target=" && read -r target
else
	IFS=':' read -r keyboard keymap target x <<-EOF
	$1
	EOF
	if [ -n "$x" ]; then
		errcho "$USAGE"
		exit 1
	fi
fi

# Defaults
if [ -z "$keyboard" ]; then
	keyboard=all
fi
if [ -z "$keymap" ]; then
	keymap=all
fi

# GOoooooooo
if [ -z "$target" ]; then
	bin/qmk compile -d -kb=$keyboard -km=$keymap
else
	bin/qmk flash -d -kb=$keyboard -km=$keymap -bl=$target
fi
