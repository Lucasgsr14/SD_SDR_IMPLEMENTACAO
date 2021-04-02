/*
 * GccApplication1.cpp
 *
 * Created: 01/04/2021 14:09:55
 * Author : lucas
 */ 
#include <avr/io.h>
#include <math.h>

static inline int AM_r2rEntrada(float s_t, int fc, float t);
static inline int FM_r2rEntrada(float s_t, int fm, int fc, float t);
static inline uint8_t mapeamento(uint16_t amostraAD);

int main(void){
	DDRD = 0xFF;
	float t=0;
	while(1){
		PORTD = FM_r2rEntrada(mapeamento((uint16_t)(511.5*cos(2*M_PI*t)+512)), 1, 10, t);
		t+=0.001;
	}
}

static inline uint8_t mapeamento(uint16_t amostraAD){
	return (uint8_t)(255*amostraAD/1023.0);
}
static inline int AM_r2rEntrada(float s_t, int fc, float t){
	float s=0;
	s = (s_t/127.5-1) * cos(2*M_PI*fc*t);
	//cout<<"cos(sinalSenoidal): "<<s_t/255.0<<" tempo: "<<t<<'\n';
	//cout<<"cos: "<<cos(2*M_PI*fc*t)<<" interior: "<<2*M_PI*fc*t<<" tempo: "<<t<<'\n';
	return  128 + int(127.5 * s);
}
static inline int FM_r2rEntrada(float s_t, int fm, int fc, float t){
	float s=0;
	s = cos((s_t/127.5-1) + (1/fm)*sin(M_PI*fm*t));
	return  127 + (int)(127 * s);
}
