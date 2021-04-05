/*
* SDR.c
*
* Created: 26/03/2021 17:20:46
* Authores : kaike, vinicius, samuel, mateus, lucas
*/


//******************* INCLUSÃO DE BIBLIOTECAS ***************************
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD.h"

//****************** DEFINICAO DE VARIAVEIS GLOBAIS ***********************
volatile uint8_t ultimaLeitura_PINC; // Esse tipo de variável volátil permite ser acessar nos vetor de interrupção e main
volatile uint8_t statusBotoes[3] = {0,0,0};
uint8_t EN = 0;
uint8_t mode;
uint16_t detectPot = 0;
uint16_t freqPortadora;
double t=0;

int sampleADC;
int amostraPassadaCentradaEmZero = 0;
uint8_t flagPassagemPorZero=0;
int analog;
volatile float frequenciaCalculada = 666;
int amostraCentradaEmZero = 0;
volatile uint8_t valorTimer;
//************** PROTOTIPOS DE FUNCOES *********************************
static inline void run();

static inline void AjusteModulacao();

static inline void AjustePortadora();

static inline void EnvioSucesso();

static inline void AjusteModulacao();

static inline void AtualizaModulcao();

static inline uint16_t AtualizaPortadora();

static inline uint16_t lerPot();

static inline int AM_r2rEntrada(uint8_t s_t, int fc, double t);
static inline int FM_r2rEntrada(uint8_t s_t, int fm, int fc, double t);
static inline int ASK_r2rEntrada(uint8_t s_t, int fm, int fc, double t);
static inline int FSK_r2rEntrada(uint8_t s_t, int fm, int fc, double t);
static inline uint8_t mapeamento(uint16_t amostraAD);

volatile inline uint8_t controleTimer0( uint8_t ligaDesliga);
static inline void configuracaoADC();
static inline int conversaoADC(char canal);
static inline float leituraFrequencia(int amostra);
//************* VETOR DE INTERRUPCAO PCINT PARA OS BOTOES **************************
ISR(PCINT1_vect) { // Vetor de Interrupçao do PORTC
	
	
static uint8_t mudanca_bits; // Cria uma variável estática para guardar a mudança de bits do PORTC
mudanca_bits = PINC ^ ultimaLeitura_PINC; // Realiza uma operaçao XOR bit a bit para identificar qual pino foi alterado o status
ultimaLeitura_PINC = PINC; // Recebe a última leitura do PORTC

// Verifica bit a bit se o pino PC2 é igual a 1, ex: (0000 0100 & 0000 0100) = 1
    if(mudanca_bits & (1 << PINC2)){
	if (PINC & (1 << PINC2))statusBotoes[0] = 1; // BOTAO M
		
	
}
else if(mudanca_bits & (1 << PINC3)){
	if (PINC & (1 << PINC3)) statusBotoes[1] = 1; // BOTAO R
		
	
}
else if(mudanca_bits & (1 << PINC4)){
	if (PINC & (1 << PINC4))statusBotoes[2] = 1; // BOTAO P
}
else if(mudanca_bits & (1 << PINC5)){
		if (PINC & (1 << PINC5)){
			valorTimer = controleTimer0(1);
			
			} 
			else {
				
				valorTimer =controleTimer0(0);
				frequenciaCalculada = (4000000.0/(valorTimer*1024.0));
				TCNT0 = 0;
			}
}


}

int main(void)
{	   
cli();	
CLKPR |= (1<<CLKPCE)|(1<<CLKPS1);	
DDRB = 0xFF; 
DDRD = 0xFF;


setLCD();
initialPage();
	
//PC4 = P, PC3 = R PC2 = M
DDRC &= (~(1<<PC4) |  ~(1<<PC3) |  ~(1<<PC2) | ~(1<<PC5)); // DEFINE COMO ENTRADA OS BOTÕES
//DDRC |= (1<<PC5);	

// PB1 = led 
PORTB &= ~(1 << PB1); // 0 - vermelho e 1 - azul

configuracaoADC();	
//ADCSRA  = 0x90;                //liga conversão AD, fator de divisão de 2
//ADMUX   = 0x41;                //tensão de ref. de 5V, canal A1 0100 0001
	
PORTC |= (1 << PORTC2); // PC2 = INPUT_PULLUP
PORTC |= (1 << PORTC3); // PC3 = INPUT_PULLUP
PORTC |= (1 << PORTC4); // PC4 = INPUT_PULLUP
	
	
PCICR |= (1 << PCIE1); // Configura a chave PCINT do PORTC
	
//ativa a interrupcao no pinos PC2, PC3 e PC4.
PCMSK1 |= (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13);

ultimaLeitura_PINC = PINC;	// guarda a leitura inicial do PORTC
	
sei();

PORTD &= ~(1<<PD0);	



while(1){
	
		run();
		AjusteModulacao();
		AjustePortadora();
		
}
}


static inline void run(){
statusBotoes[1] = 0;// limpa a flag do botao R	

while ((statusBotoes[0] || EN) != 1){}

while ((statusBotoes[0] || !EN) != 1){	// BOTAO M NAO PRESSIONADO E EN IGUAL 1 PODE ENVIAR!

EnvioSucesso();

}
PORTD = 0x00;
}

static inline void AjusteModulacao(){
				
			statusBotoes[0] = 0; // zero a flag do botao M
			
	
			while(statusBotoes[2] != 1){ // ENQUANTO O BOTAO P N FOR PRESSIONADO FICA NESSE LOOP
				     
		
			AtualizaModulcao();

  	} // end P
		 	
			
		}

static inline void AtualizaModulcao(){
	
	
	detectPot = lerPot();
	selMode(detectPot);
	if (detectPot >= 0 && detectPot <= 255) mode = 0; 
	else if (detectPot >= 256 && detectPot <= 511) mode = 1; 	
	else if (detectPot >= 512 && detectPot <= 767) mode = 2;
	else if (detectPot >= 768 && detectPot <= 1023) mode = 3;
	
}

static inline void AjustePortadora(){
	
statusBotoes[2] = 0; // LIMPA A FLAG DO BOTAO P	

	showP();
while(statusBotoes[1] != 1){ // Verifica se o botao R foi pressionado.
	
valueP(AtualizaPortadora()); // para mostrar no display

	}
	showF();
}

static inline uint16_t AtualizaPortadora(){
	
	EN = 1;
	
	detectPot = lerPot();
	freqPortadora = (((45.0/1023.0)*detectPot) + 5);
	
	return freqPortadora;		
}

static inline void EnvioSucesso(){
	PORTB |= (1 << PB1); // led azul
	PORTC |= (1<<PC5);
	//sampleADC = conversaoADC('0');
	//frequenciaCalculada = leituraFrequencia(sampleADC);
	valueF(frequenciaCalculada);
	switch(mode){
		
		case 0: 
		PORTD = AM_r2rEntrada(mapeamento(sampleADC), 10,  t);
		
		break;
		case 1:
		PORTD = FM_r2rEntrada(mapeamento(sampleADC), 1, 10, t);
		
		break;
		case 2:
		PORTD = ASK_r2rEntrada(mapeamento(sampleADC), 50, 1000, t);
		
		break;
		case 3:
		PORTD = FSK_r2rEntrada(mapeamento(sampleADC), 1, 200, t);
		
		break;
				
	}
	t+=0.000125;// AM
	// t+=0.000525; // FM
	//t+=0.000475; // ASK
	
}

static inline uint16_t lerPot()
{
static uint16_t analogL, analogH, analog;    //variáveis locais para valores ADC

ADMUX  |= (1<<MUX0);	
ADCSRA |= (1<<ADSC);                    //inicia conversão ad
	
while(!(ADCSRA&=~(1<<ADIF)));           //aguarda conversão ad completar
	
ADCSRA |= (1<<ADIF);                    //limpa ADIF com transição LOW para HIGH
analogL = ADCL;                         //armazena o byte menos significativo
analogH = ADCH;                         //armazena o byte mais  significativo
		
	
analog  = (analogH<<8) | analogL;       //calcula para valor de 10 bits

return analog;                          //retorna resultado da conversão 0 - 1023
	
} //end ad_conv
static inline uint8_t mapeamento(uint16_t amostraAD){
	return (255.0*amostraAD/1023.0);
}
static inline int AM_r2rEntrada(uint8_t s_t, int fc, double t){
	float s=0;
	s = (s_t/127.5-1) * sin(2*M_PI*fc*t);
	//cout<<"cos(sinalSenoidal): "<<s_t/255.0<<" tempo: "<<t<<'\n';
	//cout<<"cos: "<<cos(2*M_PI*fc*t)<<" interior: "<<2*M_PI*fc*t<<" tempo: "<<t<<'\n';
	return  (128 + (127.5 * s));
}
static inline int FM_r2rEntrada(uint8_t s_t, int fm, int fc, double t){
	float s=0;
	s = sin(2*M_PI*fc*t + (1/fm)*(s_t/127.5-1));
	
	return  128 + (int)(127.5 * s);
}
static inline int ASK_r2rEntrada(uint8_t s_t, int fm, int fc, double t){
	float s=0;
	if(s_t > 128){
		s = 128 + (127.5 * sin(2*M_PI*fc*t));
		//s =  (s_t/127.5) - 1;
	}
	else{
		s = 127;
	}
	PORTC &= ~(1<<PC5);
	return s;
}
static inline int FSK_r2rEntrada(uint8_t s_t, int fm, int fc, double t){
	float s=0;
	if(s_t > 128){
		s = 128 + (127.5 * sin(2*M_PI*fc*t));
	}
	else{
		s = 128 + (127.5 * sin(2*M_PI*(fc/10)*t));
	}
	return s;
}


static inline void configuracaoADC(){
/*
	O objetivo dessa função é configurar a referência de voltagem pro ADC, o prescaler do clock (supõe-se 2MHz),
e a forma com que o valor convertido será registrado. Além disso, realiza-se uma conversão inicial para "gastar-
se" os 25 pulsos iniciais. Essa função deve ser chamada no setup. Uma vez chamada, basta-se chamar a função
conversaoADC().	
*/
	/* Setup ADC to use AVcc
    and select channel ADC0*/
    ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);

    /* Set conversion time to 
	104usec	= [(1/2MHz/16)*(13 ADC clocks per conversion)]
     and enable the ADC*/
    ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADEN);

    /* Perform Dummy Conversion to complete ADC init */
    ADCSRA |= (1<<ADSC);

    /* wait for conversion to complete */
    while ((ADCSRA & (1<<ADSC)) != 0);
}

static inline int conversaoADC(char canal){
/*
Essa função realiza a amostragem de um valor no ADC. Seu argumento é o canal a ser usado
(deve ser usado um char pois esse dado só consome um byte de memória).
Primeiro realiza-se a multiplexação do canal utilizado, depois inicia-se a conversão.
Espera-se concluir e retorna-se o valor inteiro.
*/	

	if(canal == '0')
		ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);
	else
		ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (1<<MUX0);
	
	int valor_convertido;
    /* start a new conversion on chosen channel */
    ADCSRA |= (1<<ADSC);

    /* wait for conversion to complete */
    while ((ADCSRA & (1<<ADSC)) != 0)
    ;

    ADCSRA |= (1<<ADIF); // pause conversion (?)	
	//valor_convertido =  (ADCH<<8) | ADCL;
	valor_convertido = ADC;
	return valor_convertido;
}

volatile inline uint8_t controleTimer0( uint8_t ligaDesliga){
/*
	A função controleTimer0 serve pra controle do Timer0.
	O argumento da função é um char pra não ocupar tanta 
	memória. Caso seja '0', o timer é desligado. 
	Caso seja '1', ele liga.
*/	
	uint8_t pulsosTimer0;
	TCCR0A = 0; // normal mode
	if (ligaDesliga == 0){ // desliga o timer
		TCCR0B = 0;
	}
	else{ // liga o timer
		TCCR0B = 5; // prescaler = 1024
	}
	
	pulsosTimer0 = TCNT0;
	return pulsosTimer0;
}

static inline float leituraFrequencia(int amostra){
	
	
	
	
	
////	float frequencia = -1;
	////amostraCentradaEmZero = amostra - (1024/2);
	//uint8_t valorTimer = 0;
////	(amostraCentradaEmZero*amostraPassadaCentradaEmZero < 0)
	//if((amostra<=512) && (flagPassagemPorZero == 0)){
		//flagPassagemPorZero = 1;
		//valorTimer = controleTimer0(1);
		////PORTB = 0;
	//}
	//else{ 
		//if((amostra>=512)&&(flagPassagemPorZero == 1)){
			//valorTimer = controleTimer0(0);
			////frequenciaCalculada = 1.0/(2.0*valorTimer*2000000.0);
			//frequenciaCalculada = (1000000.0/(2*valorTimer*1024.0));
			//TCNT0 = 0;
			//flagPassagemPorZero = 0;
			////PORTB = 0;
		//}
	//}
//
	//amostraPassadaCentradaEmZero = amostraCentradaEmZero;
	//return frequenciaCalculada;	
}

