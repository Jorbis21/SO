#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>

typedef enum{
	READ_ALL_FILE,
	READ_N_FIRST_BYTES,
	READ_N_LAST_BYTES
}action_t;

struct options{
	char *inputFile;
	int Nbytes;
	action_t action;
};
void readFile(int fd){
	char *buff = malloc(sizeof(char));
	while (read(fd, buff, 1) != 0){
		write(1, buff, 1);
	}	
}
int main(int argc, char *argv[]){
	int opt, fd;
	struct options options;
	while ((opt = getopt(argc, argv, "i:n:e")) != -1){
		switch (opt){
		case 'i':
			options.inputFile = optarg;
			options.action = READ_ALL_FILE;
			options.Nbytes = 0;
			break;
		case 'n':
			options.action = READ_N_FIRST_BYTES;
			options.Nbytes = atoi(optarg);
			break;
		case 'e':
			options.action = READ_N_LAST_BYTES;
			break;
		default:
			break;
		}
	}
	if(options.inputFile == NULL){
		printf("The file can not be found\n");
		return EXIT_FAILURE;
	}
	fd = open(options.inputFile, O_RDONLY);
	switch (options.action)
	{
	case READ_ALL_FILE:
		readFile(fd);
		break;
	case READ_N_FIRST_BYTES:
		if(lseek(fd, options.Nbytes, SEEK_SET) == -1){
			printf("Can not move de pointer\n");
			return EXIT_FAILURE;
		}
		readFile(fd);
		break;
	case READ_N_LAST_BYTES:
		if(lseek(fd, options.Nbytes * (-1), SEEK_END) == -1){
			printf("Can not move de pointer\n");
			return EXIT_FAILURE;
		}
		readFile(fd);
		break;
	default:
		break;
	}
	close(fd);
	return EXIT_SUCCESS;
}
