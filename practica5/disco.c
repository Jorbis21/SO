#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

/** Variable de condicion el aforo
 * hay que protegerla con un mutex.
*/
pthread_mutex_t m;
pthread_cond_t v;
pthread_cond_t n;
int actualPeople = 0;
int vipsWaiting = 0;

int nextTurnVip = 0;
int nextTurnNormal = 0;
int actualTurnVip = 0;
int actualTurnNormal = 0;

void enter_normal_client(int id){
	/** Aqui va un mutex para el acceso
	 * a la variable condicion.
	*/
	int turn;
	pthread_mutex_lock(&m);
	turn = nextTurnNormal++;
	while(turn != actualTurnNormal || vipsWaiting > 0 || actualPeople == CAPACITY){
		pthread_cond_wait(&n, &m);
	}
	actualTurnNormal++;
	actualPeople++;
	pthread_cond_broadcast(&n);
	pthread_cond_broadcast(&v);
	pthread_mutex_unlock(&m);
}

/**Este tiene prioridad*/
void enter_vip_client(int id){
	/** Aqui va un mutex para el acceso
	 * a la variable condicion.
	*/
	int turn;
	pthread_mutex_lock(&m);
	turn = nextTurnVip++;
	vipsWaiting++;
	while(turn != actualTurnVip || actualPeople >= CAPACITY){
		pthread_cond_wait(&v, &m);
	}

	printf("Client %d (%s) enters disco\n", id, VIPSTR(1));
	
	actualTurnVip++;
	actualPeople++;
	vipsWaiting--;
	pthread_cond_broadcast(&n);
	pthread_cond_broadcast(&v);
	pthread_mutex_unlock(&m);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip){
	/** Al salir de la disco hay que hacer un signal al siguiente*/
	pthread_mutex_lock(&m);
	actualPeople--;
	printf("Client %d (%s) leaving disco\n", id, VIPSTR(isvip));
	if(vipsWaiting > 0){
		pthread_cond_broadcast(&v);
	}
	else{
		pthread_cond_broadcast(&n);
	}
	pthread_mutex_unlock(&m);
	pthread_exit(0);
}
/** Funcion que se pasa a los hilos para ver
 * si entran a la disco.
*/
void *client(void *arg){
	int isvip = atoi(arg);
	if(isvip){
		enter_vip_client(pthread_self());
	}
	else{
		enter_normal_client(pthread_self());
	}
	dance(pthread_self(), isvip);
	disco_exit(pthread_self(), isvip);
}

int main(int argc, char *argv[]){
	FILE *discoFile;
	pthread_attr_t attr;
	pthread_t thid[50];
	int numThreads, i;
	char *buffer = malloc(sizeof(char));

	if(argc != 2){
		printf("Usage: ./disco <nameFile>\n");
		exit(EXIT_FAILURE);
	}
	if((discoFile = fopen(argv[1], "r")) == NULL){
		printf("Can not open the file\n");
		exit(EXIT_FAILURE);
	}
	pthread_attr_init(&attr);
	pthread_mutex_init(&m,NULL);
	pthread_cond_init(&v,NULL);
	pthread_cond_init(&n,NULL);
	fscanf(discoFile, "%u",&numThreads);
	for(i = 0; i < numThreads; i++){
		fscanf(discoFile, "%s",buffer);
		pthread_create(&thid[i], &attr, client, buffer);
	}
	fclose(discoFile);
	for(i = 0; i < numThreads; i++){
		pthread_join(thid[i], NULL);
	}
	return 0;
}
