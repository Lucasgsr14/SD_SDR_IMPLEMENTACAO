/*
* SDR.c
*
* Created: 26/03/2021 17:20:46
* Author : kaike
*/ 


#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t ultimaLeitura_PINC; // Esse tipo de variável volátil permite ser acessar nos vetor de interrupção e main
volatile uint8_t status[3];

ISR(PCINT1_vect) { // Vetor de Interrupçao do PORTC
	
	
static uint8_t mudanca_bits; // Cria uma variável estática para guardar a mudança de bits do PORTC
mudanca_bits = PINC ^ ultimaLeitura_PINC; // Realiza uma operaçao XOR bit a bit para identificar qual pino foi alterado o status
ultimaLeitura_PINC = PINC; // Recebe a última leitura do PORTC

// Verifica bit a bit se o pino PC2 é igual a 1, ex: (0000 0100 & 0000 0100) = 1
if(mudanca_bits & (1 << PINC2)) status[0] = 1;
else if(mudanca_bits & (1 << PINC3)) status[1] = 1;
else if(mudanca_bits & (1 << PINC4)) status[2] = 1;	


}

int main(void)
{
cli();
//PC4 = P, PC3 = R PC2 = M
DDRC &= ~(1<<PC4) |  ~(1<<PC3) |  ~(1<<PC2); // DEFINE COMO ENTRADA OS BOTÕES
	

// PB1 = led
DDRB |= (1 << PB1); // Configura o PB1 como SAÍDA
PORTB &= ~(1 << PB1);
	

	
PORTC |= (1 << PORTC2); // PC2 = INPUT_PULLUP
PORTC |= (1 << PORTC3); // PC3 = INPUT_PULLUP
PORTC |= (1 << PORTC4); // PC4 = INPUT_PULLUP
	
	
PCICR |= (1 << PCIE1); // Configura a chave PCINT do PORTC
	
//ativa a interrupcao no pinos PC2, PC3 e PC4.
PCMSK1 |= (1 << PCINT10); 

ultimaLeitura_PINC = PINC;	// guarda a leitura inicial do PORTC

sei();
	
	while(1){
		
		
}
}

