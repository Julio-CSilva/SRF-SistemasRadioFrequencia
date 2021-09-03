//******************************************************************************
// IMD - Instituto Metropole Digital 
// Disciplina: Projetos de Sistemas RF
// Aula 09: Transmissão de dados via protocolo SPI
// Autor: Ellon Paiva Mendes
// Professor Revisor: Heitor Florencio
//
// Este código faz parte da aula prática da disciplina de Projetos de 
// Sistemas RF do Curso Técnico do Instituto Metropole Digital. 
// O objetivo da aula mostrar como é feita a transmissão de dados via protocolo SPI. 
// Este código-fonte contém apenas as instruções a serem gravadas no dispositivo mestre. 
//
// ATENÇÃO: O código neste arquivo está INCOMPLETO. O objetivo é que os alunos 
// completem os trechos que estão faltando utilizando o conhecimento da aula. 
// Para instruções do que deve ser feito, consulte os comentários em cada seção.
// *******************************************************************
// Código-fonte baseado no código da aula 8 do PICkit 3. (PIC18F45K20 PICkit 3 
// Debug Express Lesson 8 - Interrupts)
//
// *******************************************************************
// *    Consulte a documentação da aula para maiores informações!    *
// *******************************************************************

/** B I T S   D E   C O N F I G U R A Ç Ã O ******************************/
// As linhas abaixo tratam de inicializar os bits de configuração do PIC. Esses 
// comandos são indicados pela diretiva do compilador "#pragma config". Para a 
// lista de possiveis valores, veja "Help > Topics... > PIC18 Config Settings > 
// PIC18F45K20". Para maiores informações, consulte o manual do C18.
#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                  
#pragma config PWRT = OFF, BOREN = OFF, BORV = 18                              
#pragma config WDTEN = OFF, WDTPS = 32768                                     
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTBE        
#pragma config STVREN = ON, LVP = OFF, XINST = OFF                          
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF                   
#pragma config CPB = OFF, CPD = OFF                                         
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF               
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF                           
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF           
#pragma config EBTRB = OFF                                            

/** I N C L U D E S **************************************************/
#include <xc.h>
#include "header_mestre.h"  //cabeçalho com protótipos das funções do código e algumas definições.

#define _XTAL_FREQ 1000000

/** V A R I A V E I S *************************************************/
// A directiva "#pragma udata" informa ao compilador para declarar variáveis 
// estáticas não inicializadas. No caso é declarada uma variavel unsigned char
// de nome LED_Display.
#pragma udata   // declara uma variavel não inicializada alocada estaticamente 
unsigned char LED_Display;  // unsigned char = variável de 8-bits.

// FUNÇÃO: void main (void): 
// A função "main" apenas chama a função "UserInit()", que é a responsável por 
// configurar o PIC da forma que o usuáro desejar, e depois chama a "UserProcess()" 
// que vai realizar o processamento principal do programa. Note que o 
// "UserProcess()" é chamado dentro de um laço infinito (o "while(1)"), enquanto o
// "UserInit()" é executado apenas uma vez, no início do programa. O laço infinito 
// é chamado de laço principal, e o PIC vai ficar para sempre executando a função 
// "UserProcess()". Para a experiencia, o aluno não deve alterar a função main, e 
// sim as funções "UserInit()" e "UserProcess()", que serão declaradas mais a 
// seguir. Lógico que substituir essas funções no main pelo respectivo código 
// também funcionaria, porém é uma péssima prática de programação: isso deixa o 
// código muito desorganizado e dificil de manter. Sempre é indicado dividir as 
// partes do seu programa em funções que realizem pequenas tarefas, e então 
// chamá-las na ordem correta. Lembre-se sempre do lema: dividir para conquistar.
void main (void)
{
	UserInit();	

	while(1){
		UserProcess();
	}
}

// FUNÇÃO: void UserInit(void)
// A função UserInit é chamada no começo do "main" e é onde o aluno deve colocar 
// a rotina de configuração do PIC. Essa rotina consiste em setar os valores corretos 
// nos registradores do PIC para fazer os modulos funcionarem da forma desejada. 
//
// Nesta experiência, você deve configurar o seguinte: 
// 1- Inicializar o valor inicial da variável LED_Display
// 2- Configurar a porta D para funcionar como saída digital
// 3- Configurar o pino RB0 como entrada digital
// 4- Configurar a porta SPI para funcionar como mestre
// 5- Configurar o PIC para receber a interrupção INT2
//
// Realize as adições necessárias no código seguindo as instruções dos comentários 
// abaixo.
 void UserInit(void)
{

	// 1- A variável LED_Display é onde vamos armazenar o valor a ser mostrado 
	// nos LEDs(DS1, DS2, DS3 e DS4) da placa (Demo Board do PICKIT3), e também 
	// é o valor a ser enviado pela SPI. Defina um valor inicial que de alguma 
	// forma te informe que o PIC esteja funcionando, ou seja, evite dar o valor
	// zero como valor inicial.
	LED_Display = 0b0001;   // Acende o primeiro LED como condição inicial.

	// 2- A porta D está sendo utilizada para acender os leds no circuito da placa, portanto 
	// seus pinos tem que ser configurados como saídas digitais. No microcontrolador estamos 
	// utilizando apenas os pinos RD0 a RD3, pois só estamos utilizando 4 LEDs da placa. 
	// Para definir um pino como saída digital, você deve limpar o valor do registrador 
	// TRISD relativo ao bit do registrador PORTD que você quer que seja a saída. 
	// É sempre importante verificar com quais saídas/entradas um pino do PIC está
	// multiplexado, e desabilitar aqueles que podem interferir no funcionamento desejado.
	TRISD = 0b00000000;     	// PORTD bits 7:0 are all outputs (0)

	// 3- O botão da placa está conectado a porta RB0 do PIC, logo essa porta tem 
	// que ser configurada como uma entrada digital. Isso pode ser feito alterando o 
	// valor do bit TRISB0 do registrador TRISB: 0 indica saída; 1 indica entrada. 
	TRISBbits.TRISB0 = 1;       // PORTB bit 0 (connected to switch) is input (1)

	// 4- Aqui você vai precisar programar o funcionamento do módulo MSSP para 
	// funcionar com uma comunicação SPI. Para isso, você vai ter que alterar os valores 
	// dos registradores SSPSTAT e SSPCON1. É importante setar esses valores corretamente 
	// e de acordo com o código do dispositivo escravo, senão a comunicação não vai 
	// ocorrer. Você deve definir o modo de operação da SPI nos bits SSPM<3:0> (bits SSPM 
	// de 3 a 0) do registrador SSPCON1. Veja no datasheet como colocar o PIC para trabalhar 
	// como MESTRE, utilizando um clock igual a frequencia de oscilação dividido por 4 (FOSC/4).
	// Temos que definir como será o clock gerado pelo mestre. Isso é feito nos bits CKE e SMP 
	// do registrador SSPSTAT, e no CKP do registrador SSPCON1. Veja no datasheet como 
	// configurar o SPI para transmitir quando o clock mudar para "ativo", como colocar 
	// a polaridade "idle" do clock para 0V, e como configurar para o dado ser amostrado no
	// meio do pulso de clock. Veja no datasheet onde estão os pinos SDI, SDO e SCK 
	// e com quais bits da porta C eles estão multiplexados. Você deve então definir o 
	// sentido que os dados vão transitar nessas portas. SDO é a saida do SPI, logo ela deve
	// ser configurada como saída. SDI é a entrada do SPI, logo ela tem que ser entrada. E 
	// como esse PIC é o mestre da comunicação SPI, estamos gerando o sinal de clock e 
	// portanto esse pino deve ser configurado como saída. Essas configurações são feitas nos 
	// bits do registrador TRISC relativos aos bits destas portas que estão 
	// multiplexados com os sinais do SPI. Por fim, inicie o funcionamento do SPI através do 
	// bit SSPEN do registrador SSPCON1. Lembre-se que o Escravo tem que estar configurado 
	// de acordo com o Meste para a comunicação acontecer! 

	//Seta o modo de operação da SPI: Master mode, com o clock sendo FOSC/4.
	SSPCON1bits.SSPM3 = 0; 
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 0;
	TRISCbits.TRISC3 = 0; //Configura o pino SCK (que é o RC3) como saída
	TRISCbits.TRISC4 = 1; //Configura o pino SDI (que é o RC4) como entrada
	TRISCbits.TRISC5 = 0; //Configura o pino SDO (que é o RC5) saída
	SSPSTATbits.CKE = 0; //Realiza a transmissão quando o clock mudar do estado "idle" para o "active".
	SSPCON1bits.CKP = 0; //Define a polaridade "idle" do clock como sendo zero.
	SSPSTATbits.SMP = 0; //Define para que durante a transmissão, seja feita a amostragem do dado no meio do ciclo do clock.
	SSPCON1bits.SSPEN = 1; //Habilita o uso do SPI usando as configurações acima.
	
	// 5- Para configurar corretamente as interrupções (INT0, INT1 ou INT2), você deve alterar os registradores 
	// INTCON, INTCON2 e INTCON3. A entrada RB0 gera a interrupção INT0, portanto você 
	// deve habilitá-la no bit INT0IE no registrador INTCON. É interessante limpar a flag
	// do INT0 para garantir que ela vai começar em zero (bit INT0IF do registrador INTCON).
	// Observe o circuito da placa (demo board) seguindo as orientações na aula. Quando o botão é 
	// pressionado, aplicamos 0V no RB0, voltando a 5V ao liberar o botão. Queremos que a 
	// interrupção seja gerada no momento que pressionamos o botão, ou seja, quando o sinal 
	// muda de "1" para "0". Isso é informado no bit INTEDG0 do registrador INTCON2. É 
	// importante habilitar também os resistores de "pull ups" internos para a porta B. 
	// Isso é feito no bit RBPU do registrador INTCON2. Nesse experimento, não estamos 
	// utilizando prioridades de interrupção: todas as interrupções geradas serão de alta 
	// prioridade. Por isso, desabilite as prioridades no bit IPEN no registrador RCON.
	// Mesmo que quiséssemos não seria possível definir uma alta prioridade para a interrupção INT0
	// porque não existe prioridade para o INT0 no PIC18F45K20 (Observe essa informação no datasheet).
	// Por fim, habilite o PIC para capturar as interrupções setando o bit GIEH do registrador 
	// INTCON.
	INTCONbits.INT0IE = 1;      // enable INT0 interrupt
    INTCONbits.INT0IF = 0;      // ensure INT0 flag is cleared
	INTCON2bits.INTEDG0 = 0;    // interrupt on falling edge of INT0 (switch pressed)
	INTCON2bits.RBPU = 0;		// enable PORTB internal pullups
	RCONbits.IPEN = 0; //desliga as prioridades na interrupção
    INTCONbits.GIEH = 1;        // Interrupting enabled.

}

// FUNÇÃO: UserProcess
// Nesta função você deve colocar a rotina que o PIC vai ficar realizando continuamente, 
// após a inicialização. Basicamente, o que deseja é que ele exiba o valor contido na 
// variável LED_Display nos LEDs conectados a porta D. Você não deve tratar o acionamento 
// do botão ou o envio pela SPI aqui, já que desejamos fazer isso através de uma rotina 
// de interrupção. 
void UserProcess(void){
	PORTD = LED_Display; //Coloca a saída na Demo Board
}

// -------------------- Rotinas de Serviço das Interrupções --------------------------
// FUNÇÃO: InterruptServiceHigh
//
// É nessa função que a mágica acontece. Após configurar corretamente as interrupções 
// na função UserInit, sempre que uma interrupção for gerada o PIC vai parar a 
// execução do código atual e iniciar a execução desta função. 
// Na interrupção, você deve fazer o seguinte:
//
// 1- Verificar qual interrupção causou a execução desta função;
// 2- Limpar o flag da interrupção
// 3- Atualizar o valor da variavel LED_Display;
// 4- Transmitir o valor de LED_Display pela SPI;
void __interrupt() InterruptServiceHigh(void)
{
    // 1- Para verificar qual foi a interrupção que fez o PIC entrar nesta função, 
	// basta checar o flag das interrupções que você está trabalhando. Neste exemplo, 
	// estamos trabalhando somente com uma interrupção, logo não é necessário verificar 
	// o que interrompeu o PIC. Mas vamos imaginar que temos mais interrupções ativadas 
	// e vamos realizar sim essa verificação. Para isso, basta verificar se o flag da 
	// interrupção está igual a 1. No nosso experimento, estamos utilizando a interrupção 
	// INT0. Verifique no datasheet onde fica a bit de flag do INT0 e faça um "if" 
	// testando se ela é igual a "1". Todo o código de tratamento desta interrupção deve 
	// ficar dentro deste "if".
    if (INTCONbits.INT0IF == 1) //Verifica se o INT0 interrompeu o PIC.
    {
		// 3- Aqui você deve atualizar o valor de LED_Display. Perceba que a interrupção 
		// será gerada quando pressionarmos o botão na placa (demo board). Portanto essa atualização
		// só será feita quando o botão for pressionado. Lembre-se que ao sair da rotina 
		// de interrupção a rotina principal vai atualizar o valor dos LEDs baseado no 
		// valor da variável LED_Display. Note também que temos apenas 4 LEDs (DS1-RD0,
		// DS2-RD1, DS3-RD2 e DS4-RD3) e a variavel LED_Display tem 8 bits. Logo, somente 
		// 4 bits realmente serão transformados em acionamento dos LEDs. Tome cuidado 
		// para não gerar uma atualização que mude somente bits que não serão visíveis nos LEDs.
		LED_Display = LED_Display << 1; //Desloca o valor do LED_Display uma vez para a esquerda.

		if(LED_Display == 0b10000){ //Verifica se o bit "1" saiu do escopo dos 4 leds da placa.
			LED_Display = 1; //Se saiu, volta a variável LED_Display para o valor inicial.
		}

		// 4- Para transmitir o dado pela SPI basta escrever o dado no buffer 
		// SSPBUF. Desde que a SPI esteja corretamente configurada, o módulo MSSP vai 
		// iniciar a transmissão do dado. Note que o dado vai tomar um certo tempo para
		// ser transmitido, porém o programa vai continuar executando as outras linhas 
		// do código. Logo, pode acontecer de um dado ser enviado pela SPI e antes de 
		// ele terminar de ser transmitido, o PIC tentar enviar outro dado, escrevendo 
		// novamente no SSPBUF. Isso também pode acontecer caso você deseje enviar mais 
		// de um byte pela SPI. Uma forma simples de contornar esse problema é monitorar 
		// o bit SSPIF do registrador PIR1. Durante a transmissão, esse bit permanecerá 
		// setado, indicando que existe uma transmissão em andamento. Assim que a 
		// transmissão terminar, o PIC vai limpar o valor desse bit. Um código simples 
		// de espera ocupada faz com que o código fique parado esperando a transmissão 
		// terminar para executar as demais linhas do codigo. Outros testes podem ser 
		// feitos verificando os respectivos bits (como por exemplo, verificar se houve 
		// colisão durante a transmissão), mas em uma experiência simples como esta isso 
		// não deve ocorrer. Portanto escrever o valor de LED_Display no registrador 
		// SSPBUF deve ser suficiente.
		SSPBUF = LED_Display; //Escreve o dado no buffer de transmissão.
        __delay_ms(200);
        
        // 2- Aqui você deve limpar o flag da interrupção INT0. Isso indica que a 
		// interrupção está sendo tratada. Isso é importante pois se ela não for limpa, 
		// e outra interrupção for gerada, o teste acima indicaria que INT0 gerou a 
		// interrupção (pois seu flag ainda estaria "1"), quando na verdade foi outra 
		// coisa que interrompeu o PIC. Sempre lembre de limpar as flags de interrupção 
		// ao tratá-las na sua rotina de interrupção.
        INTCONbits.INT0IF = 0; //Limpa o flag de interrupção do INT0.
    }

    
} 