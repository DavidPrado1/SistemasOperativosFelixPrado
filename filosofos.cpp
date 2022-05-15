#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int n;

pthread_mutex_t tenedores[5];

int comida = 1001;
int cnt = 0;

void restaurar(){
	if(comida==0){
		comida = 1000;
		cnt++;
	}
}

int min(int num1, int num2){
	int result;
	if(num1<num2){
		result = num1;
	}
	else{
		result = num2;
	}
	return result;
}

int max(int num1, int num2){
	int result;
	if(num1>num2){
		result = num1;
	}
	else{
		result = num2;
	}
	return result;
}

void pensar(int i){
	printf("Filosofo %d pensando.\n", i);
	sleep(random() % 10);
}

void comer(int i){
	printf("Filosofo %d comio y solto sus tenedores.\n", i);
}

void *filosofo(void *i2){
	int i =*((int *)i2);
	for(;;){
		pensar(i);
		int m1 = min(i, (i+1)%n);
		int m2 = max(i, (i+1)%n);
		pthread_mutex_lock(&tenedores[m1]);
		printf("Filosofo %d consiguio el tenedor derecho\n",i);
		pthread_mutex_lock(&tenedores[m2]);
		printf("Filosofo %d consiguio el tenedor izquierdo\n",i);
		comer(i);
		pthread_mutex_unlock(&tenedores[i]);
		pthread_mutex_unlock(&tenedores[(i+1)%n]);
	}
}

int main(void){
	printf("Ingrese la cantidad de filosofos: ");
	scanf("%i",&n);
	&tenedores[n];

	pthread_t filosofos[n];
	int iden[n];
	int i;
	for(i=0;i<n;i++){
		iden[i]=i+1;
		pthread_create(&filosofos[i], NULL, &filosofo, &iden[i]);
	}
	for(i=0; i<n;i++){
		pthread_join(filosofos[i],NULL);
	}
	return 0;
}
