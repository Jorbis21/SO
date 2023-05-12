#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

int main(int argc, char *argv[]){
    fd_set fdtubs;
    int fdTubes;
    int fdTub1, fdTub2;

    fdTub1 = open("tube", O_RDONLY | O_NONBLOCK);
    fdTub2 = open("tube2", O_RDONLY | O_NONBLOCK);

    FD_ZERO(&fdtubs);
    FD_SET(fdTub1, &fdtubs);
    FD_SET(fdTub2, &fdtubs);

    fdTubes = select(2, &fdtubs, NULL,NULL, NULL);

    if(fdTubes){
        printf("Tuberia %i, lista para leer", 1);
    }
    return 1;
}