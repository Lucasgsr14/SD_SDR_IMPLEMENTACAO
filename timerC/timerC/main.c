/*
 * timerC.c
 *
 * Created: 03/04/2021 19:09:31
 * Author : mateu
 */ 

#include <avr/io.h>
#include <stdio.h>

/*
Quantos pulsos serão necessários para se medir a passagem do período 
da onda mais longa de entrada?
(1/50)/(1/(2*10^6/1024)) ~ 39.06

- note que se usa o prescaler 1024
- 2MHz de clock
- entrada mais longa (50Hz)

Entretanto, iremos só ler a metade do período. Isso exige 20 pulsos, 
aproximadamente.

*/

static inline char controleTimer0(char ligaDesliga){
/*
	A função controleTimer0 serve pra controle do Timer0.
	O argumento da função é um char pra não ocupar tanta 
	memória. Caso seja '0', o timer é desligado. 
	Caso seja '1', ele liga.
*/	
	char pulsosTimer0;
	TCCR0A = 0; // normal mode
	if (ligaDesliga == '0'){ // desliga o timer
		TCCR0B = 0;
		pulsosTimer0 = TCNT0;
		return pulsosTimer0;
	}
	else{ // liga o timer
		TCCR0B = 5; // prescaler = 1024	
		return 'a';
	}
}

int main(void){
    /* Replace with your application code */
	char resultado;
	resultado = controleTimer0('1'); // liga o timer
    while(resultado<20){
		asm("nop");
    }
	resultado = controleTimer0('0');
	return 0;
}

