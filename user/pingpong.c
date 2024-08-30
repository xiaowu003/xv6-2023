#include "kernel/types.h"
#include "user/user.h"

#define BUFMAX 100

int main() {
    int p1[2], p2[2];
    int pid;
    char buffer[BUFMAX];
    
    // creat pipe
    if(pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "ERROR: pipe fail");
        exit(1);
    }

    // fork
    pid = fork();
    if (pid == 0) {
        // child process
        close(p1[1]);   // close pipe-1 write end
        close(p2[0]);   // close pipe-2 read end

        // child process receive buffer
        read(p1[0], buffer, sizeof(buffer));
        fprintf(1, "%d: received %s\n", pid, buffer);
        close(p1[0]);   // close pipe-1 read end

        // child process write buffer in pipe-2
        write(p2[1], "pong", 4);
        close(p2[1]);   // close pipe-2 write end
    } else if (pid > 0) {
        // parent process
        close(p1[0]);   // close pipe-1 read end
        close(p2[1]);   // close pipe-2 write end

        // parent process write in pipe-1
        write(p1[1], "ping", 4);
        close(p1[1]);   // close pipe-1 write end

        // parent process read in pipe-2
        read(p2[0], buffer, sizeof(buffer));
        fprintf(1, "%d: received %s\n", pid, buffer);
        close(p2[0]);   // close pipe-2 read end

        wait(0);    // wait child process exit
    }
    exit(0);
}