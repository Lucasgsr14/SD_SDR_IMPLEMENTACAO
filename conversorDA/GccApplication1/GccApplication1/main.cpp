/*
 * GccApplication1.cpp
 *
 * Created: 01/04/2021 14:09:55
 * Author : lucas
 */ 
#include <avr/io.h>
#include <math.h>

static inline int AM_r2rEntrada(uint8_t s_t, int fc, float t);
static inline int FM_r2rEntrada(uint8_t s_t, int fm, int fc, float t);
static inline int ASK_r2rEntrada(uint8_t s_t, int fm, int fc, float t);
static inline int FSK_r2rEntrada(uint8_t s_t, int fm, int fc, float t);
static inline uint8_t mapeamento(uint16_t amostraAD);

int main(void){
	DDRD = 0xFF;
	float t=0;
	while(1){
		PORTD = FSK_r2rEntrada(mapeamento(512+511.5*cos(2*M_PI*t)), 1, 100, t);
		t+=0.001;
	}
}

static inline uint8_t mapeamento(uint16_t amostraAD){
	return (255.0*amostraAD/1023.0);
}
static inline int AM_r2rEntrada(uint8_t s_t, int fc, float t){
	float s=0;
	s = (s_t/127.5-1) * cos(2*M_PI*fc*t);
	//cout<<"cos(sinalSenoidal): "<<s_t/255.0<<" tempo: "<<t<<'\n';
	//cout<<"cos: "<<cos(2*M_PI*fc*t)<<" interior: "<<2*M_PI*fc*t<<" tempo: "<<t<<'\n';
	return  128 + int(127.5 * s);
}
static inline int FM_r2rEntrada(uint8_t s_t, int fm, int fc, float t){
	float s=0;
	s = cos(2*M_PI*fc*t + (1/fm)*(s_t/127.5-1));
	return  128 + (int)(127.5 * s);
}
static inline int ASK_r2rEntrada(uint8_t s_t, int fm, int fc, float t){
	float s=0;
	if(s_t > 128){
		s = 128 + (127.5 * cos(2*M_PI*fc*t));
	}
	else{
		s = 127;
	}
	return s;
}
static inline int FSK_r2rEntrada(uint8_t s_t, int fm, int fc, float t){
	float s=0;
	if(s_t > 128){
		s = 128 + (127.5 * cos(2*M_PI*fc*t));
		}
	else{
		s = 128 + (127.5 * sin(2*M_PI*(fc/10)*t));
	}
	return s;
}