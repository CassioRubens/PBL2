#include <stdio.h>
#include <string.h>
#include <errno.h> // Erro e função strerror()
#include <stdlib.h> 
#include <termios.h> // Contém as definições de controle de terminal POSIX
#include <unistd.h> // write(), read(), close()
#include <wiringPi.h>
#include <wiringSerial.h> // Biblioteca simplificada de manuseio de porta serial. 

 int read_serial_int(); 
 int escolherFuncao();

int main () {
	int serial_port;
	
	 if ((serial_port = serialOpen("/dev/ttyAMA0", 9600)) < 0) { // Abre e inicializa o dispositivo serial e define a taxa de transmissão.
	 	fprintf(stderr, "%s\n", strerror(errno));
	 	return 1;
	 }
	 if (wiringPiSetup () == -1) { // retorna um código de erro se essas funções falharam por qualquer motivo. 
	 	fprintf(stdout, "%s\n", strerror(errno));
	 	return 1;
	 }
	int p = protocolo(serial_port); // Inicialização do protocolo
	while (p != 1) {
		p = protocolo(serial_port);
	} 
	close() // Fecha o dispositivo.
	return 0;
}

int protocolo (int  serial_port) {
	unsigned char msg[] = { 1 }; // Aloca espaço para a memoria

    write(serial_port, msg, sizeof(msg)); // Envia o byte único para o dispositivo serial.
	fflush(stdin); // Limpa o buff

	int s = read_serial_int(serial_port); // Retorna próximo valor inteiro disponível no dispositivo. 
	
	int opcao = 0;
	if (s == 1) { // Verifica se o valor retornado do sensor é 1
		printf("start\n");
		int op = escolherFuncao();
		while (op != 1 && op != 2 && op != 3) { // Fica no loop enquanto a  opção digitada estiver incorreta.
			printf("Escolha apenas uma das duas opções disponiveis!\n\n");
			fflush(stdin);
			op = escolherFuncao();
		}	
		int codigo; // Armazana o código de identificação do tipo.
		int inteiro; // Armazena o valor inteiro.
		int fracao; // Armazena o valor fracionado
		char send[] = { op };
		if (op == 4) {
			write(serial_port, send, sizeof(send));
			codigo = read_serial_int(serial_port);
			inteiro = read_serial_int(serial_port);
			fracao = read_serial_int(serial_port);
			printf("Codigo: %d, temperatura: %d,%d\n\n", codigo, inteiro, fracao);		
		} else if (op == 5) {
			write(serial_port, send, sizeof(send));
			codigo = read_serial_int(serial_port);
			inteiro = read_serial_int(serial_port);
			fracao = read_serial_int(serial_port);
			printf("Codigo: %d, humidade: %d,%d\n\n", codigo, inteiro, fracao);	
		} else {
				write(serial_port, send, sizeof(send));
			codigo = read_serial_int(serial_port);
			if (codigo == 0) {
				printf("Codigo: %d, sensor funcionando normalmente\n\n", codigo );	
			} else {
				printf("Codigo: %d, sensor com problema\n\n", codigo );
			}	
		}
	} else {
		printf("Erro ao tenta iniciar o programa\n");
	}
		fflush(stdin);
		printf("Digite (1) para voltar do inicio\n");
		printf("Digite (2) para finalizar\n");
		scanf("scan: %i", &opcao);
		return opcao;
}

int escolherFuncao () {
		int op;
		printf("Digite (4) para temperatura\n");
		printf("Digite (5) para humidade\n");
		printf("Digite (6) para solicitar a situacao atual do sensor\n");
		scanf("%i", &op);
		return op;
}

int read_serial_int (int fd) { // Retorna o byte convertido em inteiro
  char c;
  read(fd, &c, sizeof(c)); // Ler os bytes
  return c;
}

