#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void dir_path(char* path_record, char *name, char* target_name) {
    char* p1;
    p1 = path_record + strlen(path_record);
    *p1++ = '/';    // add '/'
    strcpy(p1, name);
    if (strcmp(name, target_name) == 0) {
        printf("%s\n", path_record);
    }
}

void find(char* path_record, char* name) {
    int fd;
    char *p;
    struct stat st;
    struct dirent de;

    if((fd = open(path_record, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path_record);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path_record);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_DIR:
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
                
                dir_path(path_record, de.name, name);
                find(path_record, name);

                // backtracing
                p = path_record + strlen(path_record);
                p = p - (strlen(de.name) + 1);
                *p = '\0';
            }

            break;
        /*
        // 这里好像永远都用不到，如果它是目标文件，那么在他的父文件夹中已经被搜索过了
        // 且文件打开之后是没有像目录打开后的那些结构的，所以这里永远不会用到
        case T_FILE:
            if (strcmp(de.name, name) == 0) {
                printf("%s\n", path_record);
            }
            break;
        */
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    char** path;
    char** file_name;   // target file name

    // determine whether the order is legitmate
    if (argc < 2 || argc > 3) {
        fprintf(2, "FORMAT ERROR: find <path> <file name>");
        exit(1);
    }

    // get command line arguments
    if (argc == 2) {
        char path_next[2];
        path_next[0] = '.';
        path_next[1] = '\0';
        file_name = &argv[1];
        find(path_next, *file_name);
    } else if (argc == 3) {
        path = &argv[1];
        file_name = &argv[2];
        find(*path, *file_name);
    }

    exit(0);
}