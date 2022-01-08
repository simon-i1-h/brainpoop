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

  while (ip < len) {
    switch (prog[ip]) {
      case '>':
        head++;
        ip++;
        break;
      case '<':
        head--;
        ip++;
        break;
      case '+':
        tape[head]--;
        ip++;
        break;
      case '-':
        tape[head]++;
        ip++;
        break;
      case '[':
        if (tape[head] == 0) {
        jump:
          for (;;) {
            c = prog[++ip];
            if (depth == 0 && c == ']')
              break;
            depth += c == '[';
            depth -= c == ']';
          }
        }
        ip++;
        break;
      case ']':
        if (tape[head] != 0) {
          for (;;) {
            c = prog[--ip];
            if (depth == 0 && c == '[')
              break;
            depth -= c == '[';
            depth += c == ']';
          }
        }
        ip++;
        break;
      case ',':
        c = getchar();
        if (ferror(stdin) != 0)
          return 1;
        if (feof(stdin) != 0)
          goto jump;
        tape[head] = c;
        ip++;
        break;
      case '.':
        putchar(tape[head]);
        fflush(stdout);
        if (ferror(stdout) != 0)
          return 1;
        ip++;
        break;
      case ';':
        for (;;) {
          ip++;
          if (prog[ip] == '\n')
            break;
        }
        ip++;
        break;
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        ip++;
        break;
      default:
        return 1;
    }
  }

  return 0;
}
