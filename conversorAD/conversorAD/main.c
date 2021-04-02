#include<avr/io.h>

void configuracaoADC(){
/*
	O objetivo dessa fun��o � configurar a refer�ncia de voltagem pro ADC, o prescaler do clock (sup�e-se 2MHz),
e a forma com que o valor convertido ser� registrado. Al�m disso, realiza-se uma convers�o inicial para "gastar-
se" os 25 pulsos iniciais. Essa fun��o deve ser chamada no setup. Uma vez chamada, basta-se chamar a fun��o
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

int conversaoADC(char canal){
/*
Essa fun��o realiza a amostragem de um valor no ADC. Seu argumento � o canal a ser usado
(deve ser usado um char pois esse dado s� consome um byte de mem�ria).

Primeiro realiza-se a multiplexa��o do canal utilizado, depois inicia-se a convers�o.
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
	valor_convertido =  (ADCH<<8) | ADCL;
	return valor_convertido;
}

int main(void)
{
	int analog;
	configuracaoADC();	

    /* Scan for changes on A/D input pin in an infinite loop */
    while(1)
    {
		analog = conversaoADC('1');
		analog = conversaoADC('0');
	}
	
    return -1;
}