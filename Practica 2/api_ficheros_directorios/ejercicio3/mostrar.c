#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>


void readfile(int fdo, int numbytes, int read);

int main(int argc, char *argv[]){
	int numbytes = 0, opt, fdo;
	int read = 0;//se trata de un booleano, inicializado en false
	while((opt = getopt(argc, argv, "a:n:e")) != -1) {
		switch(opt) {
		case 'a':
			fdo = open(optarg,O_RDONLY);
			if(fdo == -1){
				fprintf(stderr,"El archivo (%s) no se ha podido abrir\n", optarg);
				exit(1);
			}
			break;
		case 'n':
			numbytes = *optarg - '0';
			break;
		case 'e':
			read = 1;//se pone a true
			numbytes *= -1;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
	readfile(fdo,numbytes,read);
	if(close(fdo)==-1){
		fprintf(stderr,"El archivo no se ha podido cerrar\n");
		exit(2);
	}
	return 0;
}

void readfile(int fdo, int numbytes, int r){
	int l;
	if(r == 0){
		l = lseek(fdo, numbytes, SEEK_SET);
	}
	else{
		l = lseek(fdo,numbytes,SEEK_END);
	}
	if(l == -1){
		err(errno,"El descriptor del fichero no se ha podido modificar\n");
		exit(errno);
	}
	char buffer[1];
	int cr, cw;
	do{
		cr = read(fdo,buffer,1);
		if(cr == -1){
			err(errno,"El archivo no se puede leer\n");
			exit(errno);
		}
		cw = write(STDOUT_FILENO, buffer, cr);
		if(cw == -1){
			err(errno, "El archivo no se puede escribir\n");
			exit(errno);
		}
	} while(cr != 0);
	cw = write(STDOUT_FILENO, "\n", 1);
}