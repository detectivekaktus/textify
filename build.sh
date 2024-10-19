#!/usr/bin/env bash
CC="gcc"
CFLAGS="-Wall -Wextra -std=c99 -pedantic -ggdb"
SRCS=("src/main.c")
OBJS=("main.o")

BDIR="bin"
EXEC="textify"

build() {
  mkdir -p $BDIR
  for i in "${!SRCS[@]}"; do
    $CC $CFLAGS -c "${SRCS[$i]}" -o $BDIR/"${OBJS[$i]}"
  done
  $CC $CFLAGS $BDIR/$OBJS -o $EXEC
}

clean() {
  if [ -f "$EXEC" ]; then
    rm $EXEC
  else
    echo "No executable found. Skipped..."
  fi

  if [ -d "$BDIR" ]; then
    rm -r $BDIR
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
