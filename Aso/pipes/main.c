#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){

    if(argc < 3){
        printf("Argumentos invalidos");
        exit(1);
    }

    char *nombreTub = argv[1];

    //necesario si la tuberia no existe
    mkfifo(nombreTub, 0777);

    int fdTub = open(nombreTub, O_WRONLY | O_APPEND | O_NONBLOCK);

    if(fdTub == -1){
        printf("Error al abrir la tuberia");
        exit(1);
    }

    write(fdTub, argv[2], strlen(argv[2]));

    close(fdTub);
    return 1;
}