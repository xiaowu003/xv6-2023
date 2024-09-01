#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void assamble_argv(char** argument, int *n) {
    char ch;
    int another = 0, m = 0, offset = 0;
    char argu[512];
    memset(argu, 0, sizeof(argu));
    //char* argu;
    //printf("before read\n");

    argument[(*n)++] = argu + 0;

    while (read(0, &ch, 1) == 1) {
       // printf("ch = %c\n", ch);
        switch (ch) {
            case ' ':
                //printf("case 1\n");
                if (!another) {
                    another = 1;
                }
                break;
            case '\n':
                //printf("case 2\n");
                argu[m++] = '\0';
                offset++;
                argument[(*n)++] = argu + offset;
                //return;
                break;
            default:
                //printf("case 3\n");
                if (another) {
                    argu[m++] = '\0';
                    offset++;

                    argument[(*n)++] = argu + offset;

                    another = 0;
                    //printf("argument[%d] : %s\n", n-1, argument[n-1]);
                }

                argu[m++] = ch;
                offset++;

                break;
        }
    }
}

int main(int argc, char* argv[]) {
    //char buf[512];
    char* argument[MAXARG];
    int pid, n = 0;

    if (argc < 2) {
        //printf("argc = %d\n", argc);
        //printf("argv[0] = %s\n", argv[0]);
        fprintf(2, "FORMAT ERROR: xargs <command>\n");
        exit(1);
    } else if (argc > MAXARG) {
        fprintf(2, "ARG ERROR: too many arguments");
        exit(1);
    }

    for (; n < (argc - 1); n++) {
        argument[n] = argv[n + 1];
    }

    assamble_argv(argument, &n);

    //printf("after assamble\n");

    for (int i = 0; i < n; i++) {
        //printf("argument[%d] : %s\n", i, argument[i]);
    }

    pid = fork();
    if (pid == 0) {
        //printf("argv[1] = %s\n", argv[1]);

        //printf("%s\n", (*argument));
        exec(argv[1], argument);
        fprintf(2, "exec %s failed\n", argv[1]);
    } else if (pid > 0) {
        wait(0);
    }

    exit(0);
}