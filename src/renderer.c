#include "renderer.h"

const char *get_file_extension(char *filename)
{
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
  if (strcmp(fext, "html") == 0) return render_html(parse_html(filename));
  else return render_txt(filename);
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
        unsigned char level = 1;
        up++;
        if (content[up] != ' ' && content[up] != '#') {
          putchar('#');
          while (content[up] != '\n') putchar(content[up++]);
          break;
        }
        else if (content[up] == '#') {
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
        switch (level) {
          case 1: case 4: { red(); } break;
          case 2: case 5: { magenta(); } break;
          case 3: case 6: { yellow(); } break;
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
        if (start_stars == 1 && end_stars == 0) { cyan(); fputs("  *", stdout); reset(); }
        size_t end = up;
        up = start;
        while (up != end - end_stars) putchar(content[up++]);
        reset();
        up = end;
      } break;

      case '>': {
        if (content[up - 1] != '\\') {
          cyan();
          up++;
          fputs(" |", stdout);
          reset();
        }
        else putchar(content[up++]);
      } break;

      case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case '0': {
        if (content[up + 1] == '.' && col == 0) { cyan(); fputs("  ", stdout); putchar(content[up++]); reset(); }
        putchar(content[up++]);
      } break;

      case '-': case '+': {
        if (content[up + 1] == ' ' && col == 0) { cyan(); fputs("  *", stdout); up++; reset(); }
        putchar(content[up++]);
      } break;

      case '`': {
        unsigned char start_backticks = 0;
        while (content[up] == '`' && content[up] != '\n' && content[up] != '\0') { start_backticks++; up++; }
        size_t start = up;
        while (content[up] != '`' && content[up] != '\n' && content[up] != '\0') up++;
        unsigned char end_backticks = 0;
        while (content[up] == '`' && content[up] != '\n' && content[up] != '\0') { end_backticks++; up++; }
        if (start_backticks == 1 && end_backticks == 0) {
          up = start;
          putchar('`');
          putchar(content[up++]);
          break;
        }
        if (start_backticks == end_backticks) { 
          up = start;
          reverse();
          while (content[up] != '`') putchar(content[up++]);
          reset();
          up += end_backticks;
        }
      } break;

      case '~': {
        unsigned char start_tildas = 0;
        while (content[up] == '~' && content[up] != '\n' && content[up] != '\0') { start_tildas++; up++; }
        size_t start = up;
        while (content[up] != '~' && content[up] != '\n' && content[up] != '\0') up++;
        unsigned char end_tildas = 0;
        while (content[up] == '~' && content[up] != '\n' && content[up] != '\0') { end_tildas++; up++; }
        if (start_tildas == end_tildas && start_tildas > 0 && start_tildas < 3) {
          strikethrough();
          up = start;
          while (content[up] != '~') putchar(content[up++]);
          reset();
          up += end_tildas;
        }
        else {
          size_t end = up;
          up = start;
          for (unsigned char i = 0; i < start_tildas; i++) putchar('~');
          while (up != end) putchar(content[up++]);
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
        blue();
        up = link_start;
        while (up != link_end) putchar(content[up++]);
        reset();
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

const struct Html_Tag_Property props[] = {
  {"h1", HEADER1, true},
  {"h2", HEADER2, true},
  {"h3", HEADER3, true},
  {"h4", HEADER4, true},
  {"h5", HEADER5, true},
  {"h6", HEADER6, true},

  {"p", PARAGRAPH, true},
  {"strong", STRONG, true}
};

void resolve_tag(const char *name, Html_Tag *tag)
{
  if (name[0] == '/') name++;
  for (size_t i = 0; i < sizeof(props) / sizeof(props[0]); i++) {
    if (strcmp(name, props[i].name) == 0) {
      tag->type = props[i].type;
      tag->autocomplete = props[i].autocomplete;
      return;
    }
  }
  tag->type = (Tag_Type)UNKNOWN;
}

Tags *parse_html(char *filename)
{
  Tags *tags = da_heap_alloc(Tags);
  char *content = read_entire_file(filename);
  size_t up = 0;
  while (up != strlen(content)) {
    switch (content[up]) {
      case '<': {
        up++;
        CString str = {0};
        while (content[up] != '\0' && content[up] != '>') da_append(&str, content[up++]);
        da_append(&str, '\0');

        Html_Tag tag = {0};
        resolve_tag(str.items, &tag);
        if (tag.type == UNKNOWN) CRASH("Unknown or unsupported tag found during parsing.\n");
        tag.closing = str.items[0] == '/';
        da_append(tags, tag);
        up++;
      } break;

      default: {
        CString str = {0};
        while (content[up] != '\0' && content[up] != '<') da_append(&str, content[up++]);
        da_append(&str, '\0');

        Html_Tag tag = {0};
        tag.type = NONE;
        tag.autocomplete = false;
        tag.content = str.items;
        tag.closing = false;
        da_append(tags, tag);
      } break;
    }
  }
  free(content);
  content = NULL;
  return tags;
}

char *str2upr(const char *str)
{
  char *upr = malloc(strlen(str) + 1);
  for (size_t i = 0; i < strlen(str) + 1; i++) upr[i] = toupper(str[i]);
  return upr;
}

void interpret_html_tag(Tags *tags, size_t *up)
{
  switch (tags->items[*up].type) {
    case NONE: {
      printf("%s", tags->items[*up].content);
      (*up)++;
    } break;

    case HEADER1: case HEADER4: {
      red();
      bold();
      while ((tags->items[*up].type != HEADER1 || tags->items[*up].type != HEADER4) && !tags->items[*up].closing) {
        if (tags->items[*up].content == NULL) { (*up)++; continue; }
        char *str = str2upr(tags->items[(*up)++].content);
        printf("%s", str);
        free(str);
        str = NULL;
      }
      reset();
      (*up)++;
    } break;

    case HEADER2: case HEADER5: {
      magenta();
      bold();
      while ((tags->items[*up].type != HEADER2 || tags->items[*up].type != HEADER5) && !tags->items[*up].closing) {
        if (tags->items[*up].content == NULL) { (*up)++; continue; }
        char *str = str2upr(tags->items[(*up)++].content);
        printf("%s", str);
        free(str);
        str = NULL;
      }
      reset();
      (*up)++;
    } break;

    case HEADER3: case HEADER6: {
      yellow();
      bold();
      while ((tags->items[*up].type != HEADER3 || tags->items[*up].type != HEADER6) && !tags->items[*up].closing) {
        if (tags->items[*up].content == NULL) { (*up)++; continue; }
        char *str = str2upr(tags->items[(*up)++].content);
        printf("%s", str);
        free(str);
        str = NULL;
      }
      reset();
      (*up)++;
    } break;

    case PARAGRAPH: {
      (*up)++;
      while (tags->items[*up].type != PARAGRAPH && !tags->items[*up].closing) {
        if (tags->items[*up].content == NULL) { (*up)++; continue; }
        printf("%s", tags->items[(*up)++].content);
      }
      (*up)++;
    } break;

    case STRONG: {
      (*up)++;
      bold();
      while (tags->items[*up].type != STRONG && !tags->items[*up].closing) {
        if (tags->items[*up].content == NULL) { (*up)++; continue; }
        printf("%s", tags->items[(*up)++].content);
      }
      reset();
      (*up)++;
    } break;

    default: {
      assert(0 && "Unreachable");
    } break;
  }
}

int render_html(Tags *tags)
{
  size_t up = 0;
  while (up != tags->size) interpret_html_tag(tags, &up);
  da_heap_free(tags);
  return 0;
}

int render_txt(char *filename)
{
  char *content = read_entire_file(filename);
  printf("%s\n", content);
  free(content);
  content = NULL;
  return 0;
}
