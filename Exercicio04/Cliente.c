#include <winsock.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERV_HOST_ADDR "127.0.0.1"
#define SERV_UDP_PORT  6000
#define BUFFERSIZE     4096

void Abort(char* msg);

int main(int argc, char* argv[])
{
	SOCKET sockfd;
	int msg_len, iResult, length_addr;
	struct sockaddr_in serv_addr, cli_addr;
	WSADATA wsaData;
	int nbytes;
	char buffer[BUFFERSIZE];

	/*========================= TESTA A SINTAXE =========================*/

	if (argc != 2) {
		fprintf(stderr, "Sintaxe: %s frase_a_enviar\n", argv[0]);
		getchar();
		exit(EXIT_FAILURE);
	}

	/*=============== INICIA OS WINSOCKS ==============*/

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		getchar();
		exit(1);
	}

	/*=============== CRIA SOCKET PARA ENVIO/RECEPCAO DE DATAGRAMAS ==============*/

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (sockfd == INVALID_SOCKET)
		Abort("Impossibilidade de criar socket");

	/*================= PREENCHE ENDERECO DO SERVIDOR ====================*/

	memset((char*)&serv_addr, 0, sizeof(serv_addr)); /*Coloca a zero todos os bytes*/
	serv_addr.sin_family = AF_INET; /*Address Family: Internet*/
	serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); /*IP no formato "dotted decimal" => 32 bits*/
	serv_addr.sin_port = htons(SERV_UDP_PORT); /*Host TO Netowork Short*/

	/*====================== ENVIA MENSAGEM AO SERVIDOR ==================*/

	msg_len = strlen(argv[1]);

	if (sendto(sockfd, argv[1], msg_len, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		Abort("O subsistema de comunicacao nao conseguiu aceitar o datagrama");

	printf("<CLI1>Mensagem enviada ... a entrega nao e' confirmada.\n");

	/*================ MOSTRA PORTO AUTOMATICAMENTE ATRIBUIDO ==============*/

	length_addr = sizeof(cli_addr);
	if (getsockname(sockfd, (struct sockaddr*)&cli_addr, &length_addr) == SOCKET_ERROR) {
		Abort("Erro ao obter informações sobre o socket");
	}
	else {
		printf("<CLI1>Porto local automatico: %d\n", ntohs(cli_addr.sin_port));
	}

	/*========================= RECEBER MENSAGEM ===========================*/

	nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

	if (nbytes == SOCKET_ERROR)
		Abort("Erro na recepcao de datagrams");

	buffer[nbytes] = '\0'; /*Termina a cadeia de caracteres recebidos com '\0'*/

	printf("\n<CLI>Mensagem recebida {%s}\n", buffer);

	/*========================= FECHA O SOCKET ===========================*/

	closesocket(sockfd);

	printf("\n");
	exit(EXIT_SUCCESS);
}

void Abort(char* msg)
{

	fprintf(stderr, "<CLI1>Erro fatal: <%s> (%d)\n", msg, WSAGetLastError());
	exit(EXIT_FAILURE);

}
