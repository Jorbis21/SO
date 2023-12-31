#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BYTE 512

void copy(int fdo, int fdd){
	int sizeRead;
	char *buffer = malloc(sizeof(char) * MAX_BYTE);
	while((sizeRead = read(fdo, buffer, MAX_BYTE)) > 0){
		if(write(fdd, buffer, sizeRead) == -1)
			printf("Can not write on the file");
	}
}

int main(int argc, char *argv[])
{
	int fdRead, fdWrite;
	char *readFile, *writeFile;
	if(argc != 3){
		printf("Invalid number of arguments\n");
		return 1;
	}
	readFile = argv[1];
	writeFile = argv[2];
	if((fdRead = open(readFile, O_RDONLY)) == -1){
		printf("Read archive can not be open\n");
		return 1;
	}
	if((fdWrite = open(writeFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) == -1){
		printf("Write archive can not be open\n");
		return 1;
	}
	copy(fdRead, fdWrite);
	return 0;
}
