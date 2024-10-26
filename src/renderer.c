#include "renderer.h"

const char *get_file_extension(char *filename)
{
  // From man page:
  // The strrchr() function returns a pointer to the last
  // ouprence of the character c in the string s.
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
  size_t up = 0;
  size_t col = 0;
  size_t line = 0;
  while (up != strlen(content) + 1) {
    switch (content[up]) {
      case '#': {
        up++;
        if (content[up] != ' ' && content[up] != '#') {
          putchar('#');
          while (content[up] != '\n') putchar(content[up++]);
          break;
        }
        else if (content[up] == '#') {
          unsigned char level = 0;
          while (content[up] == '#') { level++; up++; }
          if (content[up] != ' ') {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            while (content[up] != '\n') putchar(content[up++]);
            break;
          }
          else if (level > 6) {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            putchar(' ');
            while (content[up] != '\n') putchar(content[up++]);
            break;
          }
        }
        bold();
        up++;
        while (content[up] != '\n') putchar(toupper(content[up++]));
        reset();
      } break;

      case '_':
      case '*': {
        unsigned char start_stars = 0;
        while ((content[up] == '*' || content[up] == '_') && content[up] != '\n' && content[up] != '\0') { start_stars++; up++; }
        size_t start = up;
        while ((content[up] != '*' && content[up] != '_') && content[up] != '\n' && content[up] != '\0') up++;
        unsigned char end_stars = 0;
        while ((content[up] == '*' || content[up] == '_') && content[up] != '\n' && content[up] != '\0') { end_stars++; up++; }
        if (start_stars == end_stars) {
          switch (end_stars) {
            case 0: break;
            case 1: { italic(); } break;
            case 2: { bold(); } break;
            case 3: { italic(); bold(); } break;
            default: { assert(0 && "Unreachable"); }
          }
        }
        if (start_stars == 1 && end_stars == 0) fputs("  *", stdout);
        size_t end = up;
        up = start;
        while (up != end - end_stars) putchar(content[up++]);
        reset();
        up = end;
      } break;

      case '>': {
        if (col == 0) {
          up++;
          fputs(" |", stdout);
        }
        putchar(content[up++]);
      } break;

      case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case '0': {
        if (content[up + 1] == '.' && col == 0) { fputs("  ", stdout); putchar(content[up++]); }
        putchar(content[up++]);
      } break;

      case '-': case '+': {
        if (content[up + 1] == ' ' && col == 0) { fputs("  *", stdout); up++; }
        putchar(content[up++]);
      } break;

      case '`': {
        unsigned char start_tildas = 0;
        while (content[up] == '`' && content[up] != '\n' && content[up] != '\0') { start_tildas++; up++; }
        size_t start = up;
        while (content[up] != '`' && content[up] != '\n' && content[up] != '\0') up++;
        unsigned char end_tildas = 0;
        while (content[up] == '`' && content[up] != '\n' && content[up] != '\0') { end_tildas++; up++; }
        if (start_tildas == 1 && end_tildas == 0) {
          up = start;
          putchar('`');
          putchar(content[up++]);
          break;
        }
        if (start_tildas == end_tildas) { 
          up = start;
          reverse();
          while (content[up] != '`') putchar(content[up++]);
          reset();
          up += end_tildas;
        }
      } break;

      case '[': {
        size_t name_start = ++up;
        while (content[up] != ']' && content[up] != '\n' && content[up] != '\0') up++;
        if (content[++up] != '(') {
          up = --name_start;
          putchar(content[up++]);
          break;
        }
        size_t name_end = up - 1;
        size_t link_start = ++up;
        while (content[up] != ')' && content[up] != '\n' && content[up] != '\0') up++;
        size_t link_end = up;

        italic();
        underline();
        up = name_start;
        while (up != name_end) putchar(content[up++]);
        reset();
        
        fputs(" (", stdout);
        up = link_start;
        while (up != link_end) putchar(content[up++]);
        putchar(')');
        up++;
      } break;

      case '\n': { col = 0; line++; putchar(content[up++]); } break;

      default: { putchar(content[up++]); } break;
    }
  }
  putchar('\n');
  free(content);
  content = NULL;
  return 0;
}
