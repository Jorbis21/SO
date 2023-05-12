#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>

int lecturayejecucion(struct stat *buf) {
    if(((S_IRGRP | S_IROTH |  S_IRUSR) & buf->st_mode) && ((S_IXGRP | S_IXOTH | S_IXUSR) & buf->st_mode)) {
        return 1;
    }
    return 0;
}
int ejecucion(struct stat *buf) {
    if(((S_IXGRP | S_IXOTH | S_IXUSR) & buf->st_mode)) {
        return 1;
    }
    return 0;
}

void d(struct stat *buf, DIR *dir, struct dirent *ent){
    lstat(optarg, buf);
    if(S_ISDIR(buf->st_mode)) {
        if(lecturayejecucion(buf)) {
            if((dir = opendir(optarg))!= NULL){
                while((ent = readdir(dir)) != NULL) {
                    if(ent->d_name[0] != '.') {
                        lstat(ent->d_name, buf);
                        if(S_ISREG(buf->st_mode)) {
                            char ast = '*';
                            if(!ejecucion(buf)) 
                                ast = ' ';
                            printf("%s (%d kb, %d link)%c \n", ent->d_name, (buf->st_size>>10), buf->st_nlink);
                        } else if(S_ISDIR(buf->st_mode)) {
                            printf("[%s] (%d link) \n", ent->d_name, buf->st_nlink);
                        } else if(S_ISLNK(buf->st_mode)) {
                            printf("%s(%s)\n", ent->d_name, realpath(ent->d_name, NULL));
                        }
                                    
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    
    struct stat *buf = malloc(sizeof(struct stat));
    int opt;
    DIR *dir;
    struct dirent *ent = malloc(sizeof(struct dirent));
    if (argc != 3) {
        printf("Numero de argumentos invalido");
        exit(0);
    }

    while ((opt = getopt(argc, argv, "d:f:")) != -1) {
        switch (opt) {
            case 'd':
                d(buf, dir, ent);
            break;
            case 'f':
                lstat(optarg, buf);
                printf("%s(inodo: %d, %0.2f kb) \n", optarg, buf->st_ino, (float)(buf->st_size>>10));
            break;
        }
    }
    return 0;
}