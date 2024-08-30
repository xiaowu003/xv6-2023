#include "kernel/types.h"
#include "user/user.h"

void dfs(int *p1, int n) {
    int p2[2];
    int num, pid;
    
    if (pipe(p2) < 0) {
        fprintf(2, "ERROR: pipe fail\n");
    }
    
    // Filter out the prime numbers, and then write the rest of the undetermined into the pipeline
    while (read(p1[0], &num, 1) > 0) {
        //fprintf(1, "%d\n", num);
        if (num < 2 * n) {
            fprintf(1, "prime %d\n", num);
        }

        if ((num > 2 * n) && (num % n != 0)) {
            write(p2[1], &num, 1);
        } 
    }
    close(p1[0]);

    pid = fork();
    if (pid == 0) {
        close(p2[1]);
        dfs(p2, n + 1);
    } else if (pid > 0) {
        close(p2[0]);
        close(p2[1]);
        wait(0);
    }
}


int main() {
    int p1[2];
    int pid;
    int n = 2;
    if (pipe(p1) < 0) {
        fprintf(2, "ERROR: pipe fail\n");
        exit(1);
    }
    // Filter out the first prime numbers, and then write the rest of the undetermined into the pipeline
    while (n <= 35) {
        if (n < 4) {
            fprintf(1, "prime %d\n", n);
        }
        if (n > 4 && n % 2 != 0) {
            write(p1[1], &n, 1);
            // fprintf(1, "write %d\n", n);
        }
        n++;
    }

    pid = fork();
    if (pid == 0) {
        close(p1[1]);
        dfs(p1, 3);
    } else if (pid > 0) {
        close(p1[0]);
        close(p1[1]);
        wait(0);
    }
    exit(0);
}