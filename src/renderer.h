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

#define reset() fputs("\033[0m", stdout);

int render(char *filename);
int render_md(char *filename);
int render_html(char *filename);
int render_txt(char *filename);

#endif
