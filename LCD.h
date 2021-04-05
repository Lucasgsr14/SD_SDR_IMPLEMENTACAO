/*
 * LCD.h
 *
 * Created: 02/04/21  17:50:05
 * Author : Vinicius
 */ 


#ifndef LCD_H
#define LCD_H	   
#include <avr/io.h>

/* Fun??es para chamar na main:
   setLCD() // FORA DO WHILE
   initialPage() // FORA DO WHILE
   
   selMode(int) // ESTADO AJUSTE MODULACAO
   valueF(int) // MOSTRA O VALOR DA FREQUENCIA
   
   valueP(int) // AJUSTE PORTADORA
   showP() // AJUSTE PORTADORA
   
   
   showF(); // RUN
   
   valueMsgAnalog(int) // resultado da msg de 0-255 8 bits
   valueMsgDigital(int) // digital em binário
   
   
   
   */

void dly(){
	// unsigned long delaY
	 //volatile unsigned long i = 0;
	 //for (i = 0; i < delay; i++) {
		 //__asm__ __volatile__ ("nop");
	 //}
	//
	for (int i = 0; i<255; i++){
		for (int j = 0; j<2; j++){
			//int a = 0;
			DDRB = 0xFF;
		}
	}
}

void clk(int num){
	dly();
	PORTB = num + 128;
	dly();
	PORTB = num;
	dly();
	
}	

void setLCD(){
	// Fun??o para fazer a configura??o inicial do LCD.
	DDRB = 0xFF;
	PORTB = 0x00;
	dly();
	
	// Modo de 4 bits
	PORTB = 0b00001000;
	clk(PORTB);
	
	// Tela de 2 linhas
	PORTB = 0b00001000; //
	clk(PORTB);
	PORTB = 0b00100000;
	clk(PORTB);
	
	// Display On/Off
	PORTB = 0b00000000;
	clk(PORTB);
	PORTB = 0b00111000;
	clk(PORTB);
	
	// Mode Set
	PORTB = 0b00000000;
	clk(PORTB);
	PORTB = 0b00011000;
	clk(PORTB);
}

void setNumber(int num){
	int cen = num/100;
	int dez = (num-(100*cen))/10;
	int uni = num - 100*cen - 10*dez;
	
	uint8_t defValue = 0b01001100;
	uint8_t dezena = (dez<<2) + 64;
	uint8_t unidade = (uni<<2) + 64;
}
	
void valueP(uint16_t num){ // CRIAR A REGRA DE TRES PARA 5-50Khz
	//Fun??o para mostrar o valor da portadora
	PORTB = 0b00100000; //Mover para posi??o 0B
	clk(PORTB);
	PORTB = 0b00101100;
	clk(PORTB);
	
	//setNumber(num)
	uint16_t cen = num/100;
	uint16_t dez = (num-(100*cen))/10;
	uint16_t uni = num - 100*cen - 10*dez;
	
	uint8_t defValue = 0b01001100;
	uint8_t dezena = (dez<<2) + 64;
	uint8_t unidade = (uni<<2) + 64;
	
	PORTB = defValue; //Dezena
	clk(PORTB);
	PORTB = dezena;
	clk(PORTB);
	
	PORTB = defValue; //Unidade
	clk(PORTB);
	PORTB = unidade;
	clk(PORTB);
}

void valueF(int num){ // CRIAR A REGRA DE TRES 50-500Hz
	//Fun??o para mostrar o valor da frequ?ncia
	PORTB = 0b00100010; //Mover para posi??o 0B
	clk(PORTB);
	PORTB = 0b00101110;
	clk(PORTB);
	
	int cen = num/100;
	int dez = (num-(100*cen))/10;
	int uni = num - 100*cen - 10*dez;
	
	uint8_t defValue = 0b01001110;
	uint8_t centena = (cen<<2) + 66;
	uint8_t dezena = (dez<<2) + 66;
	uint8_t unidade = (uni<<2) + 66;
	
	PORTB = defValue; //Centena
	clk(PORTB);
	PORTB = centena;
	clk(PORTB);
	
	PORTB = defValue; //Dezena
	clk(PORTB);
	PORTB = dezena;
	clk(PORTB);
	
	PORTB = defValue; //Unidade
	clk(PORTB);
	PORTB = unidade;
	clk(PORTB);
}

void valueMsgAnalog(int num){
	//Fun??o para mostrar o valor da mensagem na modula??o anal?gica
	PORTB = 0b00110000; //Mover para posi??o 45
	clk(PORTB);
	PORTB = 0b00010100;
	clk(PORTB);
	
	int cen = num/100;
	int dez = (num-(100*cen))/10;
	int uni = num - 100*cen - 10*dez;
	
	uint8_t defValue = 0b01001100;
	uint8_t centena = (cen<<2) + 64;
	uint8_t dezena = (dez<<2) + 64;
	uint8_t unidade = (uni<<2) + 64;
	
	PORTB = defValue; //Centena
	clk(PORTB);
	PORTB = centena;
	clk(PORTB);
	
	PORTB = defValue; //Dezena
	clk(PORTB);
	PORTB = dezena;
	clk(PORTB);
	
	PORTB = defValue; //Unidade
	clk(PORTB);
	PORTB = unidade;
	clk(PORTB);
}

void valueMsgDigital(int num){
	//Fun??o para mostrar o valor da mensagem na modula??o digital
	PORTB = 0b00110000; //Mover para posi??o 45
	clk(PORTB);
	PORTB = 0b00010100;
	clk(PORTB);
	uint8_t aux = num;
	uint8_t defValue = 0b01001100;
	uint8_t b7 = (aux>>5) + 64;
	aux = (aux<<1);
	uint8_t b6 = (aux>>5) + 64;
	aux = (aux<<1);
	uint8_t b5 = (aux>>5) + 64;
	aux = (aux<<1);
	uint8_t b4 = (aux>>5) + 64;
	aux = (aux<<1);
	uint8_t b3 = (aux>>5) + 64;
	aux = (aux<<1);
	uint8_t b2 = (aux>>5) + 64;
	aux = (aux<<1);
	uint8_t b1 = (aux>>5) + 64;
	aux = (aux<<1);
	uint8_t b0 = (aux>>5) + 64;
	aux = (aux<<1);
	
	PORTB = defValue; //B7
	clk(PORTB);
	PORTB = b7;
	clk(PORTB);
	
	PORTB = defValue; //B6
	clk(PORTB);
	PORTB = b6;
	clk(PORTB);
	
	shiftCursorRight();
	
	PORTB = defValue; //B5
	clk(PORTB);
	PORTB = b5;
	clk(PORTB);
	
	PORTB = defValue; //B4
	clk(PORTB);
	PORTB = b4;
	clk(PORTB);
	
	shiftCursorRight();
	
	PORTB = defValue; //B3
	clk(PORTB);
	PORTB = b3;
	clk(PORTB);
	
	PORTB = defValue; //B2
	clk(PORTB);
	PORTB = b2;
	clk(PORTB);
	
	shiftCursorRight();
	
	PORTB = defValue; //B1
	clk(PORTB);
	PORTB = b1;
	clk(PORTB);
	
	PORTB = defValue; //B0
	clk(PORTB);
	PORTB = b0;
	clk(PORTB);
	
}

void showF(){ 
	//Fun??o para mostrar os caracteres da parte de frequ?ncia. N?o chamar na main
	PORTB = 0b00100000; //Mover para posi??o 09
	clk(PORTB);
	PORTB = 0b00100100;
	clk(PORTB);
	
	PORTB = 0b01010000; //F
	clk(PORTB);
	PORTB = 0b01011000;
	clk(PORTB);
	
	PORTB = 0b01001100; //:
	clk(PORTB);
	PORTB = 0b01101000;
	clk(PORTB);
	
	defPad(3);
	
	PORTB = 0b01010000; //H
	clk(PORTB);
	PORTB = 0b01100000;
	clk(PORTB);
	
	PORTB = 0b01011100; //z
	clk(PORTB);
	PORTB = 0b01101000;
	clk(PORTB);
}

void showTb(){
	PORTB = 0b01010100; //T
	clk(PORTB);
	PORTB = 0b01010000;
	clk(PORTB);
	
	PORTB = 0b01001100; //:
	clk(PORTB);
	PORTB = 0b01101000;
	clk(PORTB);
}

void showP(){
	//Fun??o para mostrar os caracteres da parte da portadora. Deve ser chamada na main quando for pro estado de sele??o da portadora
	PORTB = 0b00100000; //Mover para posi??o 09
	clk(PORTB);
	PORTB = 0b00100100;
	clk(PORTB);
	
	PORTB = 0b01010100; //P
	clk(PORTB);
	PORTB = 0b01000000;
	clk(PORTB);
	
	PORTB = 0b01001100; //:
	clk(PORTB);
	PORTB = 0b01101000;
	clk(PORTB);
	
	defPad(2);
	
	PORTB = 0b01011000; //k
	clk(PORTB);
	PORTB = 0b01101100;
	clk(PORTB);
	
	PORTB = 0b01010000; //H
	clk(PORTB);
	PORTB = 0b01100000;
	clk(PORTB);
	
	PORTB = 0b01011100; //z
	clk(PORTB);
	PORTB = 0b01101000;
	clk(PORTB);
	
	
}

void defPad(int num){
	//Fun??o paara mostrar "-" no lugar dos n?meros. N?o chamar na main
	for(int i=0;i<num; i++){
		PORTB = 0b01001000; //---
		clk(PORTB);
		PORTB = 0b01110100;
		clk(PORTB);
	}
}

void dot(){
	//Ponto. N?o chamar na main
	PORTB = 0b01001000; //.
	clk(PORTB);
	PORTB = 0b01111000;
	clk(PORTB);
}

void blank(){
	//Limpar espa?o. N?o chamar na main
	for(int i=0; i<8; i++){
		PORTB = 0b01001000; //Blank
		clk(PORTB);
		PORTB = 0b01000000;
		clk(PORTB);
	}
}

void selMode(uint16_t modulacao){
	//Fun??o que faz a sele??o do tipo de modula??o
	PORTB = 0b00100000; //Mover para posi??o 05
	clk(PORTB);
	PORTB = 0b00010100;
	clk(PORTB); 
	   
	if(modulacao >= 0 && modulacao <= 255){
		PORTB = 0b01010000; //A
		clk(PORTB);
		PORTB = 0b01000100;
		clk(PORTB);
		
		PORTB = 0b01010000; //M
		clk(PORTB);
		PORTB = 0b01110100;
		clk(PORTB);
		
		PORTB = 0b01001000; //Blank
		clk(PORTB);
		PORTB = 0b01000000;
		clk(PORTB);
		
		PORTB = 0b00110000; //Mover para posi??o
		clk(PORTB);
		PORTB = 0b00010100;
		clk(PORTB);
		
		defPad(3);
		blank();
	}
	else if(modulacao >= 256 && modulacao <= 511){
		PORTB = 0b01010000; //F
		clk(PORTB);
		PORTB = 0b01011000;
		clk(PORTB);
		
		PORTB = 0b01010000; //M
		clk(PORTB);
		PORTB = 0b01110100;
		clk(PORTB);
		
		PORTB = 0b01001000; //Blank
		clk(PORTB);
		PORTB = 0b01000000;
		clk(PORTB);

		PORTB = 0b00110000; //Mover para posi??o
		clk(PORTB);
		PORTB = 0b00010100;
		clk(PORTB);
		
		defPad(3);
		
		blank();
	}
	else if(modulacao >= 512 && modulacao <= 767){
		PORTB = 0b01010000; //A
		clk(PORTB);
		PORTB = 0b01000100;
		clk(PORTB);
		
		PORTB = 0b01010100; //S
		clk(PORTB);
		PORTB = 0b01001100;
		clk(PORTB);
		
		PORTB = 0b01010000; //K
		clk(PORTB);
		PORTB = 0b01101100;
		clk(PORTB);
		
		PORTB = 0b00110000; //Mover para posi??o
		clk(PORTB);
		PORTB = 0b00010100;
		clk(PORTB);
		
		defPad(2);
		dot();
		defPad(2);
		dot();
		defPad(2);
		dot();
		defPad(2);
		
	}
	else if(modulacao >= 768 && modulacao <= 1023){
		PORTB = 0b01010000; //F
		clk(PORTB);
		PORTB = 0b01011000;
		clk(PORTB);
		
		PORTB = 0b01010100; //S
		clk(PORTB);
		PORTB = 0b01001100;
		clk(PORTB);
		
		PORTB = 0b01010000; //K
		clk(PORTB);
		PORTB = 0b01101100;
		clk(PORTB);
		
		PORTB = 0b00110000; //Mover para posi??o
		clk(PORTB);
		PORTB = 0b00010100;
		clk(PORTB);
		
		defPad(2);
		dot();
		defPad(2);
		dot();
		defPad(2);
		dot();
		defPad(2);
	}
}

void shiftCursorRight(){
	//Avna?ar uma casa para a direita. N?o chamar na main
	PORTB = 0b00000100; //Shift cursor right
	clk(PORTB);
	PORTB = 0b00011000;
	clk(PORTB);
}

void initialPage(){
	//Fun??o que seta a "p?gina inicial" do LCD
	PORTB = 0b01010000; //M
	clk(PORTB);
	PORTB = 0b01110100;
	clk(PORTB);
	
	PORTB = 0b01011000; //o
	clk(PORTB);
	PORTB = 0b01111100;
	clk(PORTB);
	
	PORTB = 0b01011000; //d
	clk(PORTB);
	PORTB = 0b01010000;
	clk(PORTB);
	
	PORTB = 0b01001100; //:
	clk(PORTB);
	PORTB = 0b01101000;
	clk(PORTB);
	
	PORTB = 0b00110000; //Mover para segunda linha
	clk(PORTB);
	PORTB = 0b00000000;
	clk(PORTB);
	
	PORTB = 0b01010000; //M
	clk(PORTB);
	PORTB = 0b01110100;
	clk(PORTB);
	
	PORTB = 0b01011100; //s
	clk(PORTB);
	PORTB = 0b01001100;
	clk(PORTB);
	
	PORTB = 0b01011000; //g
	clk(PORTB);
	PORTB = 0b01011100;
	clk(PORTB);
	
	PORTB = 0b01001100; //:
	clk(PORTB);
	PORTB = 0b01101000;
	clk(PORTB);
	
	showF();
	
}

#endif // LCD_H
