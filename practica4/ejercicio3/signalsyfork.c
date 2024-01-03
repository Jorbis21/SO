#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

pid_t childPid;
/*programa que temporiza la ejecución de un proceso hijo */
void killChild(int sig){
	kill(childPid, SIGKILL);
}

int main(int argc, char **argv){
	int status;
	struct sigaction sa;
	/**Signal se pone aqui para que el padre ignore la señal
	 * Si se pone dentro del if no funciona, me imagino que 
	 * para el hijo sera dentro del if
	*/
	signal(SIGINT, SIG_IGN);
	
	childPid = fork();
	if(childPid == 0){
		/*Funcionamiento del hijo*/
		execvp(argv[1], argv + 1);
	}
	else{
		/*Funcionamiento del padre*/
		sa.sa_handler = killChild;
		sa.sa_flags = SA_RESTART;
		sigaction(SIGALRM, &sa, NULL);
		alarm(5);
		wait(&status);
		if(WIFEXITED(status)){
			printf("Program exited\n");
		}
		else if(WIFSIGNALED(status)){
			printf("Program signaled\n");
		}
	}

	return 0;
}
