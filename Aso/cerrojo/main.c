#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){
    int fcrr;
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    struct flock *cerrojo = malloc(sizeof(struct flock));
    char *tipo, *formato = "%d/%m/%Y %H:%M:%S", fechayHora [20];
    if(argc != 2){
        printf("Numero de argumentos invalidos!\n");
        exit(0);
    }
    if((fcrr=open(argv[1], O_RDWR)) == -1){
        printf("El archivo no existe!\n");
        exit(1);
    }
    //Establecemos el cerrojo
    cerrojo->l_type = F_WRLCK;
    cerrojo->l_whence = SEEK_SET;
    cerrojo->l_start = 0;
    cerrojo->l_len = 0;
    //Control de errores para el cerrojo
    if(fcntl(fcrr,F_GETLK, cerrojo) == -1){
        printf("Error al obtener el cerrojo\n");
        exit(1);
    }
    //Comprobamos si el archivo esta bloqueado
    if(cerrojo->l_type == F_UNLCK){
        printf("El archivo no esta bloqueado\n");
        //Bloqueamos el archivo para escritura
        cerrojo->l_type = F_WRLCK;
        fcntl(fcrr, F_SETLK, cerrojo);
        //Escribimos la fecha y hora en el archivo
        strftime(fechayHora, sizeof(fechayHora), formato, &time);
        strcat(fechayHora, "\n");
        write(fcrr, fechayHora, 20);
        sleep(40);
        cerrojo->l_type = F_UNLCK;
        fcntl(fcrr, F_SETLK, cerrojo);
    }
    else{
        if(cerrojo->l_type == F_RDLCK)
            tipo = "Lectura";
        else
            tipo = "Escritura";
        printf("El archivo esta bloqueado por un cerrojo de  tipo: %s\n", tipo);
        exit(0);
    }
    
}
