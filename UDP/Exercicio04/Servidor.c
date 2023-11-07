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

		/*================ MOSTRAR MENSAGEM E LOCALIZA��O DE ORIGEM =============*/
		
		/*
		* O que pretendemos neste exerc�cio � mostrar (al�m das mensagens) a
		* localiza��o de quem enviou os datagramas e para isso podemos/devemos
		* usar a estrutura cli_addr usada na linha 54 dentro da fun��o recvfrom.
		* 
		* A fun��o recvfrom recebe uma estrutura vazia e preenche-a com a informa��o
		* de quem enviou o datagrama (IP e porto - entre outros).
		* NOTA: Lembrem-se do desenho que fiz no quadro.
		* 
		* Depois � s� usar as fun��es auxiliares para traduzir algo que n�o conhecemos
		* para algo que conhecemos e podemos usar no printf.
		* 
		* 
		* P: Porque n�o podemos usar o getsockname nesta situa��o?
		* R: Lembrem-se que o getsockname permite obter informa��es associadas
		*	 ao socket. O socket do servidor cont�m informa��es do servidor e n�o
		*    dos clientes. Logo, se usassemos a fun��o getsockname aqui iriamos
		*    estar a mostrar o IP e porto do servidor e n�o � isso que se pretende
		*    neste exerc�cio (at� porque n�s j� sabemos o IP e porto do servidor).
		*/

		printf("\n<SER1>Mensagem recebida {%s} de {IP: %s; porto: %d}\n",
			buffer, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
		
		/*
		* Para quem n�o percebeu a estrutura ou como se usa o printf vou tentar explicar
		* (se mesmo assim n�o entenderem... pe�am ajuda! Discord, aula, email, . . .)
		*
		* A fun��o printf permite-nos escrever uma mensagem de texto na consola (terminal).
		* Al�m disso permite-nos formatar a mensagem inserindo vari�veis em sitios espec�ficos.
		* Esses "s�tios" s�o especificados atrav�s de: {%s} ou {%d} (entre outros)
		* Isto quer dizer que nesses s�tios ser� introduzida uma vari�vel.
		* Essas vari�veis podem ser:
		*	- arrays de caracteres da� usarmos {%s}
		*	- inteiros da� usarmos {%d}
		*	- entre outros
		* 
		* As vari�veis em si s�o introduzidas por ordem como parametros da fun��o printf.
		* Ou seja, se tiverem 3 {%s} ou {%d} t�m que especificar 3 vari�veis.
		* 
		* NOTA: � mais f�cil explicar no quadro, lembrem-me na pr�xima aula :) 
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