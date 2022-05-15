#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int n;

pthread_mutex_t tenedores[5];

int comida = 1000;
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

void *filosofo(void *i2){
	int i =*((int *)i2);
	int estomagototal = 1 + rand() %10;
	int estomago = 0;
	printf("Filosofo %d pensando\n",i);
	int energiagastada = 1;
	sleep(random() % 10);
	for(;;){
		int m1 = min(i, (i+1)%n);
		int m2 = max(i, (i+1)%n);
		pthread_mutex_lock(&tenedores[m1]);
		printf("Filosofo %d consiguio el tenedor derecho\n",i);
		pthread_mutex_lock(&tenedores[m2]);
		printf("Filosofo %d consiguio el tenedor izquierdo\n",i);
		restaurar();
		comida--;
		estomago++;
		printf("Filosofo %d comio y cesa, Comida Total: %d, Estomago: %d/%d, Energia Gastada: %d\n",i, comida, estomago, estomagototal, energiagastada);
		if(estomago==estomagototal){
			energiagastada++;
			estomago--;
			printf("Filosofo %d lleno, se puso a pensar, Estomago: %d/%d, Energia Gastada: %d\n",i, estomago, estomagototal, energiagastada);
		}
		pthread_mutex_unlock(&tenedores[i]);
		pthread_mutex_unlock(&tenedores[(i+1)%n]);
		sleep(random() % 10);
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
