#include <stdio.h>
#define SIZE 65535

int main(int argc, char **argv) {
  unsigned char prog[SIZE];
  unsigned char tape[SIZE] = {0};
  size_t ip = 0;
  size_t head = 0;
  FILE *f;
  size_t len;
  size_t depth;
  int c;

  if (argc < 2)
    return 1;

  if ((f = fopen(argv[1], "rb")) == NULL)
    return 1;

  len = fread(prog, 1, SIZE, f);
  if (ferror(f) != 0 || feof(f) == 0 || fclose(f) != 0)
    return 1;

  for (; ip < len; ++ip) {
    switch (prog[ip]) {
      case '>':
        ++head;
        break;
      case '<':
        --head;
        break;
      case '+':
        ++tape[head];
        break;
      case '-':
        --tape[head];
        break;
      case '[':
        if (tape[head] == 0) {
        jump:
          ++ip;
          for (;;) {
            c = prog[ip++];
            if (depth == 0 && c == ']')
              break;
            depth += c == '[';
            depth -= c == ']';
          }
        }
        break;
      case ']':
        if (tape[head] != 0) {
          --ip;
          for (;;) {
            c = prog[ip--];
            if (depth == 0 && c == '[')
              break;
            depth -= c == '[';
            depth += c == ']';
          }
        }
        break;
      case ',':
        c = getchar();
        if (ferror(stdin) != 0)
          return 1;
        if (feof(stdin) != 0)
          goto jump;
        tape[head] = c;
        break;
      case '.':
        putchar(tape[head]);
        fflush(stdout);
        if (ferror(stdout) != 0)
          return 1;
        break;
      case ';':
        while (prog[ip] != '\n')
          ++ip;
        break;
      default:
        break;
    }
  }

  return 0;
}
