#include <stdio.h>
#define SIZE 65536

int main(int argc, char *argv[]) {
    unsigned char prog[SIZE];
    unsigned char tape[SIZE] = {0};
    size_t ip = 0;
    size_t head = 0;
    FILE *f;
    size_t len;
    size_t depth;
    int buf;

    if(argc < 2)
        return 1;

    if((f = fopen(argv[1], "rb")) == NULL)
        return 1;

    len = fread(prog, 1, SIZE, f);
    if(ferror(f) != 0 || feof(f) == 0)
        return 1;

    for(; ip < len; ++ip) {
        switch(prog[ip]) {
        case '>': ++head; break;
        case '<': --head; break;
        case '+': ++tape[head]; break;
        case '-': --tape[head]; break;
        case '[':
            if(tape[head] == 0)
            jump:
                for(depth = 0, ++ip; depth > 0 || prog[ip] != ']'; ++ip)
                    switch(prog[ip]) {
                    case '[': ++depth; break;
                    case ']': --depth; break;
                    }
            break;
        case ']':
            if(tape[head] != 0)
                for(depth = 0, --ip; depth > 0 || prog[ip] != '['; --ip)
                    switch(prog[ip]) {
                    case '[': --depth; break;
                    case ']': ++depth; break;
                    }
            break;
        case ',':
            buf = getchar();
            if(ferror(stdin) != 0)
                return 1;
            if(feof(stdin) != 0)
                goto jump;
            tape[head] = buf;
            break;
        case '.':
            putchar(tape[head]);
            fflush(stdout);
            break;
        case ';': for(; prog[ip] != '\n'; ++ip); break;
        default: break;
        }
    }

    if(fclose(f) != 0)
        return 1;

    if(ferror(stdout) != 0)
        return 1;

    return 0;
}
