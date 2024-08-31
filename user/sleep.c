#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {  
    if(argc <= 1) {
        fprintf(2, "Usage: sleep [tick number]\n");
        exit(1);
    }

    if (atoi(argv[1]) < 0) {
        fprintf(2, "ERROR : the sleep time is minus\n");
        exit(1);
    }
    sleep(atoi(argv[1]));

    exit(0);
}