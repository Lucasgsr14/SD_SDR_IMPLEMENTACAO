/*
 * LCD.h
 *
 * Created: 02/04/21  17:50:05
 * Author : Vinicius
 */ 


#ifndef LCD_H
#define LCD_H	   
#include <avr/io.h>

/* Funções para chamar na main:
   setLCD()
   valueP(int)
   showP()
   valueF(int)
   valueMsgAnalog(int)
   valueMsgDigital(int)
   selMode(int)
   initialPage()
   */

void dly(){
	for (int i = 0; i<255; i++){
		for (int j = 0; j<2; j++){
			int a = 0;
			PORTC = 0x00;
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
	// Função para fazer a configuração inicial do LCD.
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
	
void valueP(int num){
	//Função para mostrar o valor da portadora
	PORTB = 0b00100000; //Mover para posição 0B
	clk(PORTB);
	PORTB = 0b00101100;
	clk(PORTB);
	
	//setNumber(num)
	int cen = num/100;
	int dez = (num-(100*cen))/10;
	int uni = num - 100*cen - 10*dez;
	
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

void valueF(int num){
	//Função para mostrar o valor da frequência
	PORTB = 0b00100000; //Mover para posição 0B
	clk(PORTB);
	PORTB = 0b00101100;
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

void valueMsgAnalog(int num){
	//Função para mostrar o valor da mensagem na modulação analógica
	PORTB = 0b00110000; //Mover para posição 45
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
	//Função para mostrar o valor da mensagem na modulação digital
	PORTB = 0b00110000; //Mover para posição 45
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
	//Função para mostrar os caracteres da parte de frequência. Não chamar na main
	PORTB = 0b00100000; //Mover para posição 09
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
	//Função para mostrar os caracteres da parte da portadora. Deve ser chamada na main quando for pro estado de seleção da portadora
	PORTB = 0b00100000; //Mover para posição 09
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
	//Função paara mostrar "-" no lugar dos números. Não chamar na main
	for(int i=0;i<num; i++){
		PORTB = 0b01001000; //---
		clk(PORTB);
		PORTB = 0b01110100;
		clk(PORTB);
	}
}

void dot(){
	//Ponto. Não chamar na main
	PORTB = 0b01001000; //.
	clk(PORTB);
	PORTB = 0b01111000;
	clk(PORTB);
}

void blank(){
	//Limpar espaço. Não chamar na main
	for(int i=0; i<8; i++){
		PORTB = 0b01001000; //Blank
		clk(PORTB);
		PORTB = 0b01000000;
		clk(PORTB);
	}
}

void selMode(int modulacao){
	//Função que faz a seleção do tipo de modulação
	PORTB = 0b00100000; //Mover para posição 05
	clk(PORTB);
	PORTB = 0b00010100;
	clk(PORTB); 
	   
	if(modulacao == 0){
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
		
		PORTB = 0b00110000; //Mover para posição
		clk(PORTB);
		PORTB = 0b00010100;
		clk(PORTB);
		
		defPad(3);
		blank();
	}
	else if(modulacao == 1){
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

		PORTB = 0b00110000; //Mover para posição
		clk(PORTB);
		PORTB = 0b00010100;
		clk(PORTB);
		
		defPad(3);
		
		blank();
	}
	else if(modulacao == 2){
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
		
		PORTB = 0b00110000; //Mover para posição
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
	else if(modulacao == 3){
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
		
		PORTB = 0b00110000; //Mover para posição
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
	//Avnaçar uma casa para a direita. Não chamar na main
	PORTB = 0b00000100; //Shift cursor right
	clk(PORTB);
	PORTB = 0b00011000;
	clk(PORTB);
}

void initialPage(){
	//Função que seta a "página inicial" do LCD
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
