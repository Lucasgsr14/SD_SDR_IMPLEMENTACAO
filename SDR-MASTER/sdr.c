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
uint16_t freqPortadora = 5;

//************** PROTOTIPOS DE FUNCOES *********************************
static inline void run();

static inline void AjusteModulacao();

static inline void AjustePortadora();

static inline void EnvioSucesso();

static inline void AjusteModulacao();

static inline void AtualizaModulcao();

static inline uint16_t AtualizaPortadora();

static inline uint16_t lerPot();


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


}

int main(void)
{	   
cli();	
	
DDRB = 0xFF; 
DDRD = 0xFF;
	
setLCD();
initialPage();
	
//PC4 = P, PC3 = R PC2 = M
DDRC &= (~(1<<PC4) |  ~(1<<PC3) |  ~(1<<PC2)); // DEFINE COMO ENTRADA OS BOTÕES
	

// PB1 = led 
PORTB &= ~(1 << PB1); // 0 - vermelho e 1 - azul
	
ADCSRA  = 0x90;                //liga conversão AD, fator de divisão de 2
ADMUX   = 0x41;                //tensão de ref. de 5V, canal A1 0100 0001
	
PORTC |= (1 << PORTC2); // PC2 = INPUT_PULLUP
PORTC |= (1 << PORTC3); // PC3 = INPUT_PULLUP
PORTC |= (1 << PORTC4); // PC4 = INPUT_PULLUP
	
	
PCICR |= (1 << PCIE1); // Configura a chave PCINT do PORTC
	
//ativa a interrupcao no pinos PC2, PC3 e PC4.
PCMSK1 |= (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12);

ultimaLeitura_PINC = PINC;	// guarda a leitura inicial do PORTC
	
sei();

PORTD &= ~(1<<PD0);	
while(1){
	
		run();
		AjusteModulacao();
	//	AjustePortadora();
		
}
}


static inline void run(){
statusBotoes[1] = 0;// limpa a flag do botao R	

while ((statusBotoes[0] || EN) != 1){}

while ((statusBotoes[0] || !EN) != 1){	// BOTAO M NAO PRESSIONADO E EN IGUAL 1 PODE ENVIAR!
EnvioSucesso();

}
}

			static inline void AjusteModulacao(){
	
			static uint16_t lastDetect = 0; // guarda a ultima leitura do Potenciometro
			statusBotoes[0] = 0; // zero a flag do botao M
	
			lastDetect = lerPot(); // FACO A LEITURA DO POT
	
			while(statusBotoes[2] != 1){ // ENQUANTO O BOTAO P N FOR PRESSIONADO FICA NESSE LOOP
			detectPot = lerPot();
			selMode(detectPot);
			//while(abs(detectPot - lastDetect) != 1){ // VERIFICA SE HOUVE MUDANCA NA LEITURA DO POT
		//
					//detectPot = lerPot();
					//lastDetect = detectPot;	
					//PORTD |= (1<<PD0);
						//
			//
			//} // END LEITURA POT
	  //
	      //PORTD &= ~(1<<PD0);
		
			//AtualizaModulcao();

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
static uint16_t lastDetectP;	
	
lastDetectP = lerPot();
	
	while(statusBotoes[1] != 1){ // Verifica se o botao R foi pressionado.
		while(detectPot - lastDetectP > 0){
			detectPot = lerPot();
			lastDetectP = detectPot;
		}
		
valueP(AtualizaPortadora()); // para mostrar no display

	}
}

static inline uint16_t AtualizaPortadora(){
	static uint16_t Pot;
	EN = 1;
	
	Pot = lerPot();
	freqPortadora = (((45/1023)*Pot) + 5);
	
	return freqPortadora;		
}

static inline void EnvioSucesso(){
	PORTB |= (1 << PB1); // led azul
}

static inline uint16_t lerPot()
{
static uint16_t analogL, analogH, analog;    //variáveis locais para valores ADC
	
ADCSRA |= (1<<ADSC);                    //inicia conversão ad
	
while(!(ADCSRA&=~(1<<ADIF)));           //aguarda conversão ad completar
	
ADCSRA |= (1<<ADIF);                    //limpa ADIF com transição LOW para HIGH
analogL = ADCL;                         //armazena o byte menos significativo
analogH = ADCH;                         //armazena o byte mais  significativo
		
	
analog  = (analogH<<8) | analogL;       //calcula para valor de 10 bits

return analog;                          //retorna resultado da conversão 0 - 1023
	
} //end ad_conv
