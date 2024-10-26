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
        if (content[cur] != ' ' && content[cur] != '#') {
          putchar('#');
          while (content[cur] != '\n') putchar(content[cur++]);
          break;
        }
        else if (content[cur] == '#') {
          unsigned char level = 0;
          while (content[cur] == '#') { level++; cur++; }
          if (content[cur] != ' ') {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            while (content[cur] != '\n') putchar(content[cur++]);
            break;
          }
          else if (level > 6) {
            for (unsigned char i = 0; i < level; i++) putchar('#');
            putchar(' ');
            while (content[cur] != '\n') putchar(content[cur++]);
            break;
          }
        }
        bold();
        cur++;
        while (content[cur] != '\n') putchar(toupper(content[cur++]));
        reset();
      } break;

      case '_':
      case '*': {
        unsigned char start_stars = 0;
        while ((content[cur] == '*' || content[cur] == '_') && content[cur] != '\n' && content[cur] != '\0') { start_stars++; cur++; }
        size_t start = cur;
        while ((content[cur] != '*' && content[cur] != '_') && content[cur] != '\n' && content[cur] != '\0') cur++;
        unsigned char end_stars = 0;
        while ((content[cur] == '*' || content[cur] == '_') && content[cur] != '\n' && content[cur] != '\0') { end_stars++; cur++; }
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
        size_t end = cur;
        cur = start;
        while (cur != end - end_stars) putchar(content[cur++]);
        reset();
        cur = end;
      } break;

      case '>': {
        cur++;
        fputs(" |", stdout);
      } break;

      case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case '0': {
        if (content[cur + 1] == '.') { fputs("  ", stdout); putchar(content[cur++]); }
      } break;

      case '-': case '+': {
        if (content[cur + 1] == ' ') { fputs("  *", stdout); cur++; }
      } break;

      case '`': {
        unsigned char start_tildas = 0;
        while (content[cur] == '`' && content[cur] != '\n' && content[cur] != '\0') { start_tildas++; cur++; }
        size_t start = cur;
        while (content[cur] != '`' && content[cur] != '\n' && content[cur] != '\0') cur++;
        unsigned char end_tildas = 0;
        while (content[cur] == '`' && content[cur] != '\n' && content[cur] != '\0') { end_tildas++; cur++; }
        if (start_tildas == 1 && end_tildas == 0) {
          cur = start;
          putchar('`');
          putchar(content[cur++]);
          break;
        }
        if (start_tildas == end_tildas) { 
          cur = start;
          reverse();
          while (content[cur] != '`') putchar(content[cur++]);
          reset();
          cur += end_tildas;
        }
      } break;

      case '[': {
        size_t name_start = ++cur;
        while (content[cur] != ']' && content[cur] != '\n' && content[cur] != '\0') cur++;
        if (content[++cur] != '(') {
          cur = --name_start;
          putchar(content[cur++]);
          break;
        }
        size_t name_end = cur - 1;
        size_t link_start = ++cur;
        while (content[cur] != ')' && content[cur] != '\n' && content[cur] != '\0') cur++;
        size_t link_end = cur;

        italic();
        underline();
        cur = name_start;
        while (cur != name_end) putchar(content[cur++]);
        reset();
        
        fputs(" (", stdout);
        cur = link_start;
        while (cur != link_end) putchar(content[cur++]);
        putchar(')');
        cur++;
      } break;

      default: {
        putchar(content[cur]);
        cur++;
      } break;
    }
  }
  free(content);
  content = NULL;
  return 0;
}
