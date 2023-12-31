#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BYTE 512

int copy(int fdo, int fdd)
{
	int sizeRead;
	char *buffer = malloc(sizeof(char) * MAX_BYTE);
	while((sizeRead = read(fdo, buffer, MAX_BYTE)) > 0){
		if(write(fdd, buffer, sizeRead) == -1){
			printf("Can not write on the file");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int copy_regular(char *orig, char *dest){
	int fdo, fdd;
	if((fdo = open(orig, O_RDONLY)) == -1){
		printf("Read archive can not be open\n");
		return EXIT_FAILURE;
	}
	if((fdd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) == -1){
		printf("Write archive can not be open\n");
		return EXIT_FAILURE;
	}
	return copy(fdo, fdd);
}

void copy_link(char *orig, char *dest, struct stat *fileInfo){
	int size;
	char *buffer = malloc(fileInfo->st_size+1);
	size = readlink(orig, buffer, fileInfo->st_size+1);
	buffer[size] = '\0';
	symlink(buffer, dest);	
}

int main(int argc, char *argv[]){
	struct stat *fileInfo = malloc(sizeof(struct stat));
	if(argc != 3){
		printf("Invalid number of arguments\n"); 
		return EXIT_FAILURE;
	}
	if(lstat(argv[1], fileInfo) == -1){
		printf("Can not access to file information\n"); 
		return EXIT_FAILURE;
	}
	if(S_ISLNK(fileInfo->st_mode)){
		copy_link(argv[1], argv[2], fileInfo);
		return EXIT_SUCCESS;
	}
	else if(S_ISREG(fileInfo->st_mode)){
		return copy_regular(argv[1], argv[2]);
	}
	else{
		printf("File type not supported\n");
		return EXIT_FAILURE;
	}
}
