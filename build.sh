#!/bin/zsh

cd "$(dirname "$0")" || exit 1
ninja -t compdb > compile_commands.json
ninja

cd ./build
./PWMake

