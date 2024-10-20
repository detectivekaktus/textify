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
    switch (content[cur]) {
      case '#': {
        cur++;
        // Invalid syntax. No space between # and header.
        if (content[cur] != ' ' && content[cur] != '#') {
          putchar('#');
          while (content[cur] != '\n') {
            putchar(content[cur]);
            cur++;
          }
          break;
        }
        else if (content[cur] == '#') {
          unsigned char level = 0;
          while (content[cur] == '#') { level++; cur++; }
          // Invalid syntax. No space between # and header.
          if (content[cur] != ' ') {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            while (content[cur] != '\n') {
              putchar(content[cur]);
              cur++;
            }
            break;
          }
          // Invalid syntax. There are only 6 header levels in markdown.
          else if (level > 6) {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            putchar(' ');
            while (content[cur] != '\n') {
              putchar(content[cur]);
              cur++;
            }
            break;
          }
        }

        bold();
        cur++;
        while (content[cur] != '\n') {
          putchar(toupper(content[cur]));
          cur++;
        }
        reset();
      } break;

      case '*': {
        cur++;
        // Bold.
        if (content[cur] == '*') {
          size_t start = ++cur;
          while (content[cur] != '\0' && content[cur] != '\n' && content[cur] != '*') cur++;
          // Invalid syntax. Bold must be closed by a pair of *. Render italic instead.
          if (content[++cur] != '*') {
            putchar('*');
            italic();
            cur = start;
            while (content[cur] != '*') {
              putchar(content[cur]);
              cur++;
            }
            reset();
            cur++;
            break;
          }
          bold();
          cur = start;
          while (content[cur] != '*') {
            putchar(content[cur]);
            cur++;
          }
          reset();
          cur++;
        }
        // Italic.
        else {
          size_t start = cur;
          while (content[cur] != '\0' && content[cur] != '\n' && content[cur] != '*') cur++;
          // A simple dot.
          if (content[cur] == '\0' || content[cur] == '\n') {
            cur = start;
            putchar('*');
            break;
          }
          italic();
          cur = start;
          while (content[cur] != '*') {
            putchar(content[cur]);
            cur++;
          }
          reset();
          cur++;
        }
      } break;
      default: {
        putchar(content[cur]);
        cur++;
      }
    }
  }
  free(content);
  content = NULL;
  return 0;
}
