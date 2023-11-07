/*
* Programa para explicar como se criam threads
* 
* 6 passos que têm que ter em atenção:
* 
*	1 - Declarar as variáveis (ver formulário)
*	2 - Atribuir os valores à variável "sa" (ver formulário)
*	3 - Criar a thread com a função "CreateThread" (ver formulário)
*	    Ter atenção aos parâmetros relativos à função que a thread vai executar e a variável que queremos passar para a função
*	4 - Definir parâmetro como LPVOID (ver formulário)
*	5 - Fazer cast para o tipo de variável que precisamos
*	6 - Terminar thread principal (main) com ExitThread(...)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

void MostraMensagem(LPVOID param);

/*________________________________ main ________________________________________
*/
int main(int argc, char* argv[]) {
	int numeroDeThreads;
	int result;
	int i;
	
	// Passo 1	- Declarar as variáveis necessárias
	//			- Tal como está no formulário
	SECURITY_ATTRIBUTES sa;
	DWORD thread_id;

	printf("Este programa cria X threads que apresentam uma mensagem\n");
	printf("Quantas threads queres criar?\n");
	printf("> ");
	scanf_s("%d", &numeroDeThreads);
	printf("\nVou criar %d threads\n", numeroDeThreads);

	i = numeroDeThreads;

	while (i != 0) {

		// Passo 2	- Atribuir os valores à variável sa
		//			- Tal como está no formulário
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;


		// Passo 3	- usar a função CreateThread para criar a thread
		//			- Tal como está no formulário
		//			
		//			- Ter especial atenção para 2 parâmetros:
		//				- MostraMensagem: função que a thread vai executar
		//				- numeroDeThreads: variável que é passada para a função MostraMensagem
		result = CreateThread(&sa, 0, (LPTHREAD_START_ROUTINE)MostraMensagem, (LPVOID)numeroDeThreads, (DWORD)0, &thread_id);
		if (result == NULL) {
			printf("Nao e' possivel iniciar uma nova thread (error: %d)\n", GetLastError());
		}

		i--;
	}

	// Passo 6	- Terminar a thread principal (main) com a função ExitThread(...)
	//			- Sem esta instrução, é o processo que termina em vez da thread principal.
	//			- Se terminarmos o processo, então todas as suas threads serão terminadas.
	ExitThread(EXIT_SUCCESS);
}

// Passo 4	- Definir parâmetro como LPVOID
//			- Tal como está no formulário
void MostraMensagem(LPVOID param) {
	int numeroDeThreads;

	// Passo 5	- Fazer cast para o tipo de variável que precisamos
	numeroDeThreads = (int)param;

	printf("\nOla, existem %d threads e eu tenho o id = %d", numeroDeThreads, GetCurrentThreadId());
	ExitThread(EXIT_SUCCESS);
}