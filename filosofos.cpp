#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int n; 

pthread_mutex_t tenedores[5];

int comida = 1000; 
int cnt = 0; //contador de veces que se reinicia la comida

void restaurar(){ //Funcion para restaurar la comida cuando llegue a 0
	if(comida==0){
		comida = 1000;
		cnt++;
		printf("Se restauro la comida, Numero de Restauraciones: %d",cnt);
	}
}

int min(int num1, int num2){ //Funcion para determinar minimo, esta ayuda a identificar los tenedores del ultimo filosofo.
	int result;
	if(num1<num2){
		result = num1;
	}
	else{
		result = num2;
	}
	return result;
}

int max(int num1, int num2){ //Funcion para determinar maximo, esta ayuda a identificar los tenedores del ultimo filosofo.
	int result;
	if(num1>num2){
		result = num1;
	}
	else{
		result = num2;
	}
	return result;
}

void *filosofo(void *i2){ //Funcion que ejecuta el hilo
	int i =*((int *)i2); //variable que contiene el numero del filosofo
	int estomagototal = 50 + rand() %50; //Estomago total que puede iniciar de 50 a 100 unidades.
	int estomago = 0; //recurso propio del hilo
	printf("Filosofo %d pensando\n",i);
	int energiagastada = 1; //la energia empieza en 1 ya que el filosofo empieza pensando, aun no afecta al estomago.
	sleep(random() % 10);
	for(;;){
		int m1 = min(i, (i+1)%n); //Determinando tenedores del filosofo
		int m2 = max(i, (i+1)%n); //Determinando tenedores del filosofo
		pthread_mutex_lock(&tenedores[m1]); //Bloqueando el recurso compartido para que el hilo que lo quiera usar espere hasta que este hilo lo desbloquee.
		printf("Filosofo %d consiguio el tenedor derecho\n",i);
		pthread_mutex_lock(&tenedores[m2]); //Bloqueando el recurso compartido para que el hilo que lo quiera usar espera hasta que este hilo lo desbloquee.
		printf("Filosofo %d consiguio el tenedor izquierdo\n",i);
		restaurar(); //Antes de consumir se verifica para restaurar la comida.
		comida--; //Se resta el recurso compartido, como ya bloquee los tendores no hay necesidad de bloquear la comida, ya que el filosofo no comera hasta que tenga los 2 tenedores.
		estomago++; //Aumenta el contador del estomago en 1
		printf("Filosofo %d comio y cesa, Comida Total: %d, Estomago: %d/%d, Energia Gastada: %d\n",i, comida, estomago, estomagototal, energiagastada);
		if(estomago==estomagototal){ //Cuando el filosofo se llena  se pone a pensar, por ende gasta energia.
			energiagastada++;
			estomago--;//Como gasto energia se resta el contador del estomago.
			printf("Filosofo %d lleno, se puso a pensar, Estomago: %d/%d, Energia Gastada: %d\n",i, estomago, estomagototal, energiagastada);
		}
		pthread_mutex_unlock(&tenedores[i]); //Terminando el filosofo de comer, recien desbloquea los tenedores (uno por uno)
		pthread_mutex_unlock(&tenedores[(i+1)%n]);
		sleep(random() % 10); //Se manda a dormir el hilo para que todos los filosofos puedan comer y ninguno se quede sin comer.
	}
}

int main(void){
	printf("Ingrese la cantidad de filosofos: ");
	scanf("%i",&n); //Almacena la cantidad de filosofos a iniciar
	&tenedores[n];

	pthread_t filosofos[n];
	int iden[n];
	int i;
	for(i=0;i<n;i++){ //crea los hilos.
		iden[i]=i+1;
		pthread_create(&filosofos[i], NULL, &filosofo, &iden[i]);
	}
	for(i=0; i<n;i++){ //inicializa los hilos.
		pthread_join(filosofos[i],NULL);
	}
	return 0;
}
