#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */

int main(int argc, char **argv){
	pid_t pid = fork();
	if(pid == 0){ //proceso hijo
		printf("Soy el proceso hijo, este es mi pid: %d, este es el de mi padre: %d \n", getpid(), getppid());
		pid_t pid_hijo_hijo = fork();
		if(pid_hijo_hijo == 0){
			printf("Soy el proceso hijo, este es mi pid: %d, este es el de mi padre: %d \n", getpid(), getppid()); 
			execvp(argv[1], argv);
		}
		else if(pid_hijo_hijo > 0){
			waitpid(pid_hijo_hijo);
			printf("Soy el proceso padre, este es mi pid: %d, este es el de mi hijo: %d \n", getpid(), pid_hijo_hijo); 
			execvp(argv[1], argv);
		}
		else{
			fprintf(stderr,"AAAAAAAAAAAAAAAAAAAAAAAAAAAA NO VA AYUDAAAAA\n");
			exit(1);
		}
	}
	else if(pid > 0){
		waitpid(pid);
		printf("Soy el proceso padre, este es el pid: %d, este es el de mi hijo: %d \n", getpid(), pid);
		pid_t pid2 = fork();
		if(pid2==0){
			//hijo
			printf("Soy el proceso hijo, este es mi pid: %d, este es el de mi padre: %d \n", getpid(), getppid()); 
			execvp(argv[1], argv);
		} else if(pid2>0){
			//padre
			waitpid(pid2);
			printf("Soy el proceso padre, este es mi pid: %d, este es el de mi hijo: %d \n", getpid(), pid2); 
		}
		else{
			//error
			fprintf(stderr,"AAAAAAAAAAAAAAAAAAAAAAAAAAAA NO VA AYUDAAAAA\n");
			exit(1);
		}
	}
	else{
		fprintf(stderr,"AAAAAAAAAAAAAAAAAAAAAAAAAAAA NO VA AYUDAAAAA\n");
		exit(1);
	}
	return 0;
}