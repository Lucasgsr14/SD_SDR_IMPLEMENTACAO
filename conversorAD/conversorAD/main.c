#include<avr/io.h>

void configuracaoADC(){
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

int conversaoADC(){
	int valor_convertido;
    /* start a new conversion on channel 8 */
    ADCSRA |= (1<<ADSC);

    /* wait for conversion to complete */
    while ((ADCSRA & (1<<ADSC)) != 0)
    ;

    ADCSRA |= (1<<ADIF);	
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
		analog = conversaoADC();
	}
	
    return -1;
}