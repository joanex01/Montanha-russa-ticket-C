#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

int t[20];  //Array que armazena os numeros aleatorios para o passeio das pessoas
int volta=1; //Contador de voltas da montanha russa
int number=1;  //Variavel que representa o "ticket" da pessoa da fila
int next=5;  //Variavel que determina os proximos "tickets" a serem chamados
int turn[20];  //Array que armazena o turno de cada pessoa no carro
int passageiros=0;  //Contador do número de passageiros no carro
int corrida=1;  //Inteiro que representa um booleano (Se torna 0 quando o carro acaba a corrida e se torna 1 apos as pessoas sairem do carro)
int caminho=0;  //Inteiro que representa um booleano (É 1 quando o carro esta em movimento e 0 quando esta em espera)

void* pessoa(void* p){  //Funcao que representa as pessoas do parque
    int c= (int) p;
    while(1){
      	turn[c]=__sync_fetch_and_add(&number, 1);  //Armazena o turno da pessoa c em turn[c] e adiciona 1 ao contador de ticket
      	printf("Pessoa %d tem turno = %d\n", c, turn[c]);  
      	while(turn[c]>next);  //So sao permitidos passar do while os primeiros 5 da fila
      	printf("Pessoa %d esta na vez, mas nao entrou no carro\n", c);  
      	while(caminho==1);  //Enquanto caminho e 1, não é possivel entrar no carro
 	  	__sync_fetch_and_add(&passageiros, 1);  //Adiciona 1 ao numero de passageiros
 	  	printf("Pessoa %d entrou no carro\n", c);  
 	  	//if(volta>10) printf("Pessoa %d ficou presa no parque e morreu de fome\n", c);
 	  	while(corrida==1);  //Enquanto o carro esta em movimento, nao e possivel avançar
 	  	
 	  	printf("Pessoa %d saiu do carro\n", c);
 		__sync_fetch_and_add(&passageiros, -1);  //As pessoas saem do carro
 		
 		t[c]=rand()%11+5;  //Numero aleatorio que representa tempo de passeio pelo parque
 		
 		printf("Pessoa %d vai passear\n", c);
 		
 		sleep(t[c]);
 		
    }
}

void* carro(){  //Função que representa o carro
	int n=0;  //Variável que representa o número de passageiros no carro efetivamente
	while (1) {
 		corrida=1;  
 		printf("Carro aguardando proxima volta\n");
		sleep(2);  //O carro espera a entrada dos passageiros por 2 segundos
 		caminho=1;  //Impede a entrada de mais pessoas no carro
 		int r=rand()%3+2;  //Numero aleatorio que representa o tempo da volta do carro
		printf("Carro entra em movimento por %d segundos (volta %d)\n", r ,volta);
 		sleep(r);  
 		n=passageiros;  
 		printf("Corrida finalizada\n");
		corrida=0;  //Permite a saida dos passageiros
		printf("Esperando passageiros sairem\n");
 		while(passageiros!=0);  //Espera o carro esvaziar
 		next+=n;  //Incrementa next com o numero efetivo de pessoas no carro
		caminho=0;  //Permite a entrada de pessoas no carro
 		volta++; //Contador de voltas é incrementado
 	}
}

int main(){
	srand(time(NULL));
    pthread_t thread_pessoa[20], thread_carro;  //Threads serão armazenadas no array "thread_pessoa[20]" e na variável "thread_carro"
    int i=0;
    pthread_create(&thread_carro, NULL, carro, NULL);  //Cria a thread da função carro
    for(i=0;i<20;i++){
        pthread_create(&thread_pessoa[i], NULL, pessoa, (void*) i);  //Cria as threads da função pessoa
    }
    
    while(volta<=10);  //Quando a variável global atinge 10, a thread main saí do while e finaliza o programa
    printf("O carro deu 10 voltas, o parque esta sendo fechado\n");
    return 0;
}
