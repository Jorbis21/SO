#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>


const int tBuffer=512;

void copy(int fdo, int fdd){
	char buffer[tBuffer];
	int cr, cw;
	do{
		cr = read(fdo,buffer,tBuffer);
		if(cr == -1){
			err(errno,"El archivo de origen no se puede leer\n");
			exit(errno);
		}
		cw = write(fdd, buffer, cr);
		if(cw == -1){
			err(errno, "El archivo de destino no se puede escribir\n");
			exit(errno);
		}
	} while(cr != 0);
}

int main(int argc, char *argv[])
{
	if(argc != 3){
		fprintf(stderr,"Argumentos insuficientes: <Archivo origen> <Archivo destino>\n");
		exit(1);
	}
	int fdo = open(argv[1], O_RDONLY);
	int fdd = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0777);
	if(fdo == -1){
		fprintf(stderr,"El archivo de origen (%s) no se ha podido abrir\n", argv[1]);
		exit(2);
	}
	else if (fdd == -1){
		fprintf(stderr,"El archivo de destino (%s) no se ha podido abrir\n", argv[2]);
		exit(2);
	}
	else{
		copy(fdo,fdd);
	}
	if(close(fdo)==-1){
		fprintf(stderr,"El archivo de origen (%s) no se ha podido cerrar\n", argv[1]);
		exit(3);
	}
	if(close(fdd)==-1){
		fprintf(stderr,"El archivo de destino (%s) no se ha podido cerrar\n", argv[2]);
		exit(3);
	}
	return 0;
}
