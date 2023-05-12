#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>
#include <string.h>


const int tBuffer=512;

void symbolic(struct stat *buf, char *argv[]); //copia un enlace simbolico
void copy_regular(int fdo, int fdd); //copia un archivo regular
void regular(char *argv[]); //control de errores de una copia regular


int main(int argc, char *argv[]){
	argc = 3;
	argv[1] = "mylink";
	argv[2] = "mylinkcopy";
	struct stat *buf = malloc(sizeof(struct stat));
	if(argc != 3){
		fprintf(stderr,"Argumentos insuficientes: <Archivo origen> <Archivo destino>\n");
		exit(0);
	}
	
	int st = lstat(argv[1], buf);
	if(st == -1){
		err(errno,"El archivo de origen no se puede leer\n");
		exit(errno);
	}
	if(S_ISLNK(buf->st_mode)){
		symbolic(buf,argv);
 	}
	else if(S_ISREG(buf->st_mode)){
		regular(argv);
	}
	else{
		fprintf(stderr,"El archivo de origen no es un archivo regular o un link simbolico\n");
		exit(1);
	}
	return 0;
}

void symbolic(struct stat *buf, char *argv[]){
	char *buffer;
	buffer = malloc(buf->st_size+1);
	int  rl = readlink(argv[1], buffer, buf->st_size+1);
	if(rl == -1){
		fprintf(stderr,"No se ha podido leer la ruta de destino\n");
		exit(2);
	}	
	buffer[rl+1] = '\0';
	symlink(buffer, argv[2]);
}

void copy_regular(int fdo, int fdd){
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

void regular(char *argv[]){
	int fdo = open(argv[1], O_RDONLY);
	int fdd = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0777);
	if(fdo == -1){
		fprintf(stderr,"El archivo de origen (%s) no se ha podido abrir\n", argv[1]);
		exit(3);
	}
	else if (fdd == -1){
		fprintf(stderr,"El archivo de destino (%s) no se ha podido abrir\n", argv[2]);
		exit(4);
	}
	else{
		copy_regular(fdo,fdd);
	}
	if(close(fdo)==-1){
		fprintf(stderr,"El archivo de origen (%s) no se ha podido cerrar\n", argv[1]);
		exit(5);
	}
	if(close(fdd)==-1){
		fprintf(stderr,"El archivo de destino (%s) no se ha podido cerrar\n", argv[2]);
		exit(6);
	}
}
