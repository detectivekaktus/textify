#include "renderer.h"

const char *get_file_extension(char *filename)
{
  // From man page:
  // The strrchr() function returns a pointer to the last
  // ocurrence of the character c in the string s.
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename) return "";
  return dot + 1;
}

char *read_entire_file(const char *filename)
{
  FILE *f = fopen(filename, "r");
  if (f == NULL) return NULL;
  fseek(f, 0, SEEK_END);
  int len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *str = malloc(sizeof(char) * (len + 1));
  if (str == NULL) CRASH("buy more ram, lol\n");
  char c;
  int i = 0;
  while ((c = fgetc(f)) != EOF) {
    str[i++] = c;
  }
  str[i] = '\0';
  fclose(f);
  return str;
}

int render(char *filename)
{
  const char *fext = get_file_extension(filename);
  if (strcmp(fext, "md") == 0) return render_md(filename);
  else return 1;
}

int render_md(char *filename)
{
  char *content = read_entire_file(filename);
  size_t cur = 0;
  while (cur != strlen(content) + 1) {
    int c = content[cur];
    switch (c) {
      case '#': {
        c = content[cur++];
        // Invalid syntax. No space between # and header.
        if (c != ' ' && c != '#') {
          putchar('#');
          while (c != '\n') {
            c = content[cur++];
            putchar(c);
          }
        }
        else if (c == '#') {
          unsigned char level = 0;
          while (c == '#') {
            level++;
            c = content[cur++];
          }
          // Invalid syntax. No space between # and header.
          if (c != ' ') {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            putchar(c);
            while (c != '\n') {
              c = content[cur++];
              putchar(c);
            }
            break;
          }
          // Invalid syntax. There are only 6 header levels in markdown.
          else if (level > 6) {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            putchar(' ');
            while (c != '\n') {
              c = content[cur++];
              putchar(c);
            }
            break;
          }
        }

        bold();
        while (c != '\n') {
          c = content[cur++];
          putchar(toupper(c));
        }
        reset();
      } break;
      default: {
        putchar(c);
        cur++;
      }
    }
  }
  free(content);
  content = NULL;
  return 0;
}
