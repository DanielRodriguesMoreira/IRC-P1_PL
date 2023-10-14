#include <stdio.h>
#include <winsock.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERV_UDP_PORT 6000
#define BUFFERSIZE     4096

void Abort(char* msg);

/*________________________________ main ________________________________________
*/

int main(int argc, char* argv[])
{
	SOCKET sockfd;
	int iResult, nbytes, msg_len, length_addr;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[BUFFERSIZE];
	WSADATA wsaData;

	/*=============== INICIA OS WINSOCKS ==============*/

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		getchar();
		exit(1);
	}

	/*============ CRIA O SOCKET PARA RECEPCAO/ENVIO DE DATAGRAMAS UDP ============*/

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		Abort("Impossibilidade de abrir socket");

	/*=============== ASSOCIA O SOCKET AO  ENDERECO DE ESCUTA ===============*/

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; /*Address Family: Internet*/
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /*Host TO Network Long*/
	serv_addr.sin_port = htons(SERV_UDP_PORT);  /*Host TO Network Short*/

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		Abort("Impossibilidade de registar-se para escuta");


	/*================ PASSA A ATENDER CLIENTES INTERACTIVAMENTE =============*/

	while (1) {

		fprintf(stderr, "<SER1>Esperando datagrama...\n");

		length_addr = sizeof(cli_addr);
		nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cli_addr, &length_addr);

		if (nbytes == SOCKET_ERROR)
			Abort("Erro na recepcao de datagrams");

		buffer[nbytes] = '\0'; /*Termina a cadeia de caracteres recebidos com '\0'*/

		/*================ MOSTRAR MENSAGEM E LOCALIZAÇÃO DE ORIGEM =============*/
		
		/*
		* O que pretendemos neste exercício é mostrar (além das mensagens) a
		* localização de quem enviou os datagramas e para isso podemos/devemos
		* usar a estrutura cli_addr usada na linha 54 dentro da função recvfrom.
		* 
		* A função recvfrom recebe uma estrutura vazia e preenche-a com a informação
		* de quem enviou o datagrama (IP e porto - entre outros).
		* NOTA: Lembrem-se do desenho que fiz no quadro.
		* 
		* Depois é só usar as funções auxiliares para traduzir algo que não conhecemos
		* para algo que conhecemos e podemos usar no printf.
		* 
		* 
		* P: Porque não podemos usar o getsockname nesta situação?
		* R: Lembrem-se que o getsockname permite obter informações associadas
		*	 ao socket. O socket do servidor contém informações do servidor e não
		*    dos clientes. Logo, se usassemos a função getsockname aqui iriamos
		*    estar a mostrar o IP e porto do servidor e não é isso que se pretende
		*    neste exercício (até porque nós já sabemos o IP e porto do servidor).
		*/

		printf("\n<SER1>Mensagem recebida {%s} de {IP: %s; porto: %d}\n",
			buffer, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
		
		/*
		* Para quem não percebeu a estrutura ou como se usa o printf vou tentar explicar
		* (se mesmo assim não entenderem... peçam ajuda! Discord, aula, email, . . .)
		*
		* A função printf permite-nos escrever uma mensagem de texto na consola (terminal).
		* Além disso permite-nos formatar a mensagem inserindo variáveis em sitios específicos.
		* Esses "sítios" são especificados através de: {%s} ou {%d} (entre outros)
		* Isto quer dizer que nesses sítios será introduzida uma variável.
		* Essas variáveis podem ser:
		*	- arrays de caracteres daí usarmos {%s}
		*	- inteiros daí usarmos {%d}
		*	- entre outros
		* 
		* As variáveis em si são introduzidas por ordem como parametros da função printf.
		* Ou seja, se tiverem 3 {%s} ou {%d} têm que especificar 3 variáveis.
		* 
		* NOTA: é mais fácil explicar no quadro, lembrem-me na próxima aula :) 
		*/

		/*====================== ENVIA MENSAGEM AO CLIENTE ==================*/

		if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cli_addr, sizeof(cli_addr)) == SOCKET_ERROR)
			Abort("O subsistema de comunicacao nao conseguiu aceitar o datagrama");

		printf("<SER1>Mensagem enviada ... a entrega nao e' confirmada.\n");
	}
}

void Abort(char* msg)
{
	fprintf(stderr, "<SERV>Erro fatal: <%s> (%d)\n", msg, WSAGetLastError());
	exit(EXIT_FAILURE);
}