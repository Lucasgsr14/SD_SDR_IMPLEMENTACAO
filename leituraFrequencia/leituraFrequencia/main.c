/*
 * leituraFrequencia.c
 *
 * Created: 04/04/2021 11:21:11
 * Author : mateu
 */ 

#include <avr/io.h>



int amostraPassadaCentradaEmZero = 0;
uint8_t flagPassagemPorZero=0;
int analog;
float frequenciaCalculada = -1;
int amostraCentradaEmZero = 0;

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


static inline uint8_t controleTimer0( uint8_t ligaDesliga){
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
//	float frequencia = -1;
	//amostraCentradaEmZero = amostra - (1024/2);
	uint8_t valorTimer = 0;
//	(amostraCentradaEmZero*amostraPassadaCentradaEmZero < 0)
	if((amostra<=512) && (flagPassagemPorZero == 0)){
		flagPassagemPorZero = 1;
		valorTimer = controleTimer0(1);
		PORTB = 0;
	}
	else{ 
		if((amostra>=512)&&(flagPassagemPorZero == 1)){
			valorTimer = controleTimer0(0);
			//frequenciaCalculada = 1.0/(2.0*valorTimer*2000000.0);
			frequenciaCalculada = (1000000.0/(2*valorTimer*1024.0));
			TCNT0 = 0;
			flagPassagemPorZero = 0;
			PORTB = 0;
		}
	}

	amostraPassadaCentradaEmZero = amostraCentradaEmZero;
	return frequenciaCalculada;	
}

int main(void){
	CLKPR |= (1<<CLKPCE)|(1<<CLKPS1);
    /* Replace with your application code */
	DDRB |= (1<<DDB0);
	configuracaoADC();
    while (1){
		PORTB = 1;
		analog = conversaoADC('0');
		analog = analog + 0;
		frequenciaCalculada = leituraFrequencia(analog);
    }
}

