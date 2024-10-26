#ifndef TXTFY_RENDERER
#define TXTFY_RENDERER

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CRASH(msg)        \
  do {                    \
    fprintf(stderr, msg); \
    exit(1);              \
  } while (0)

#define bold() fputs("\033[1m", stdout);
#define dim() fputs("\033[2m", stdout);
#define italic() fputs("\033[3m", stdout);
#define underline() fputs("\033[4m", stdout);
#define blink() fputs("\033[5m", stdout);
#define reverse() fputs("\033[7m", stdout);
#define hide() fputs("\033[8m", stdout);
#define strikethrough() fputs("\033[9m", stdout);

#define black() fputs("\033[30m", stdout)
#define red() fputs("\033[31m", stdout)
#define green() fputs("\033[32m", stdout)
#define yellow() fputs("\033[33m", stdout)
#define blue() fputs("\033[34m", stdout)
#define magenta() fputs("\033[35m", stdout)
#define cyan() fputs("\033[36m", stdout)
#define white() fputs("\033[37m", stdout)

#define reset() fputs("\033[0m", stdout);

int render(char *filename);
int render_md(char *filename);
int render_html(char *filename);
int render_txt(char *filename);

#endif
