#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

char buffer0[128];
char buffer1[128];

int flag = 0;
int next = 0;

pthread_t p;


void * f0 (void * dummy){
	snprintf(buffer0,128,"blablabla*********************zsd");
	
	flag = 1;
	while(next == 0);

	printf("buffer1 est : %s\n",buffer1);
	return NULL;
}

void * f1 (void * dummy){
	snprintf(buffer1,128,"hello world");
	
	next = 1;
	while(flag == 0);

	printf("buffer0 est : %s\n",buffer0);
	return NULL;
}

int main(void){
	pthread_create(&p, NULL, f0, NULL);
	f1(NULL);
	pthread_join(p, NULL);
	return 0;
}