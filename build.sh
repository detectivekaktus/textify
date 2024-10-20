#!/usr/bin/env bash
CC="gcc"
CFLAGS="-Wall -Wextra -std=c99 -pedantic -ggdb"
SRCS=("src/main.c" "src/renderer.c")
OBJS=("bin/main.o" "bin/renderer.o")

EXEC="textify"

build() {
  mkdir -p bin/
  for i in "${!SRCS[@]}"; do
    $CC $CFLAGS -c "${SRCS[$i]}" -o "${OBJS[$i]}"
  done
  $CC $CFLAGS "${OBJS[@]}" -o $EXEC
}

clean() {
  if [ -f "$EXEC" ]; then
    rm $EXEC
  else
    echo "No executable found. Skipped..."
  fi

  if [ -d "bin" ]; then
    rm -r bin
  else
    echo "No \`bin\` directory found. Skipped..."
  fi
}

case "$1" in
  "run")
    build
    ./$EXEC ;;
  "clean")
    clean ;;
  *)
    if [ -z "$1" ]; then
      build
    else
      echo "Unknown command $1"
    fi ;;
esac
