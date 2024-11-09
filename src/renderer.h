#ifndef TXTFY_RENDERER
#define TXTFY_RENDERER

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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
int render_txt(char *filename);

typedef enum {
  HEADER1, HEADER2, HEADER3, HEADER4, HEADER5, HEADER6,
  PARAGRAPH,

  NONE,
  UNKNOWN
} Tag_Type;

typedef struct {
  Tag_Type type;
  char *content;
  bool autocomplete;
  bool closing;
} Html_Tag;

typedef struct {
  Html_Tag *items;
  size_t size;
  size_t capacity;
} Tags;

struct Html_Tag_Property {
  const char *name;
  Tag_Type type;
  bool autocomplete;
};

#define DA_INIT_CAPACITY 64
#define da_heap_alloc(type) (type*) calloc(1, sizeof(type))
#define da_append(da, item)                                                         \
  do {                                                                              \
    if ((da)->size >= (da)->capacity) {                                             \
      (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAPACITY : (da)->capacity * 2; \
      (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));    \
      if ((da)->items == NULL) CRASH("buy more ram, lol\n");                        \
    }                                                                               \
    (da)->items[(da)->size++] = (item);                                             \
  } while (0)

#define da_free(da) free((da)->items);
#define da_heap_free(da)  \
  do {                    \
    free((da)->items);    \
    free(da);             \
  } while (0)

typedef struct {
  char *items;
  size_t size;
  size_t capacity;
} CString;

typedef struct {
  char *items;
  size_t size;
  size_t capacity;
} CString_Builder;

#define sb_append_str(sb, str)                                                        \
  do {                                                                                \
    if ((sb)->size + strlen(str) + 1 >= (sb)->capacity) {                             \
      while ((sb)->size + strlen(str) + 1>= (sb)->capacity)                           \
        (sb)->capacity = (sb)->capacity == 0 ? DA_INIT_CAPACITY : (sb)->capacity * 2; \
      (sb)->items = realloc((sb)->items, (sb)->capacity);                             \
      if ((sb)->items == NULL) CRASH("buy more ram, lol\n");                          \
    }                                                                                 \
    (sb)->size -= (sb)->size == 0 ? 0 : 1;                                            \
    for (int i = 0; i < strlen(str) + 1; i++)                                         \
      (sb)->items[(sb)->size++] = str[i];                                             \
  } while (0)

char *str2upr(const char *str);
Tags *parse_html(char *filename);
int render_html(Tags *tags);

#endif
