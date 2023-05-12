#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	argv[1] = "ejemplo.txt";
	argc = 2;
	FILE* file=NULL;
	int c, tam;
	char* buffer;
	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name> \n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */

	do {
		
		/* Print byte to stdout */
		fwrite(buffer, tam, 1, stdout);
		printf("\nInserte el tamaño que desea leer: \n");
		scanf("%d",&tam);
		buffer = malloc(tam * sizeof(char));
		c = fread(buffer, tam, 1, file);

		
	}while (c != 0);

	fclose(file);
	return 0;
}