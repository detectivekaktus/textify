#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"

void usage(char *name)
{
  fprintf(stderr, "%s [ file ] [ -i input ] [ -o output ]\n", name);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  file             see contents of a file\n");
  fprintf(stderr, "  -i input_file    specify the input file for conversion\n");
  fprintf(stderr, "  -o output_file   specify the input file for conversion\n");
  fprintf(stderr, "\nSupported file extensions:\n");
  fprintf(stderr, "  .md  Markdown\n");
}

int main(int argc, char **argv)
{
  switch (argc) {
    case 1: {
      fprintf(stderr, "Not enough arguments\n");
      usage(argv[0]);
      exit(1);
    } break;
    case 2: {
      int res = render(argv[1]);
      if (res == 1) {
        fprintf(stderr, "Unsupported file extension.\n");
        usage(argv[0]);
        exit(1);
      }
    } break;
    case 3: {
      assert(0 && "Not implemeted\n");
    } break;
    default: {
      assert(0 && "Unreachable\n");
    } break;
  }
  return 0;
}
