/* =====[ Discription   ]=======
 *
 * PORTA = LED, CA->Q5~Q8/FND RIGHT
 * PORTC = FND Numaric Display->Q5~Q8/FND RIGHT
 *
 * PORTB = LED, CA->Q1~Q4/FND LEFT
 * PORTD = FND Numaric Display->Q1~Q4/FND LEFT
 *
 * 
 *
 */


#include <avr/io.h>
#include <util/delay.h>
const unsigned char fndc[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x27,0x7f,0x67};//Type CK
const unsigned char fnd[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98,0x88,0x83,0xc6,0xa1,0x86,0x8e};  //표시할 숫자들을 배열로 정리, Type CA

const unsigned dt = 1;

void seg1(int digit, int num);
void seg2(int digit, int num);
void Fnd_Display(int digit, int num); //FND Display 함수

int main(void)
{
  int i=0,j=0,num;
  DDRA = 0xff;
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0x0f;
  
  
  do {

    PORTA = 0x0f;
    PORTD = 0x0f;	
  	for(i = 0 ; i<16 ; i++) {
      PORTC = fnd[i];
	  PORTB = fnd[i];
      PORTA = (0xf0 ^ PORTA);
      _delay_ms(1000);
	}

    for(j=0 ; j<725 ; j++) {
      for(i=1 ; i<=4 ; i++) {
        seg1(i,num+i+4);
		seg2(i,num+i);
		_delay_ms(dt);
	  }
	}
	
	
	
	 
	
  }while (1);
}

void seg2(int digit, int num) {
  int fnd_sel = 0x08;
  PORTA = fnd_sel>>(digit-1);
  PORTC = fnd[num];
}

void seg1(int digit, int num) {
  int fnd_sel = 0x08;
  PORTD = fnd_sel>>(digit-1);
  PORTB = fnd[num];
}

void Fnd_Display(int digit, int num) {
  //int fnd_sel = 0x00;
  if(digit == 1) {
    PORTD = 0x08;
  }
  else if(digit == 2) {
    PORTD = 0x04;
  }
  else if(digit == 3) {
    PORTD = 0x02;
  }
  else if(digit == 4) {
    PORTD = 0x01;
  }
  PORTD = 0xff;
  PORTB = fnd[num];
}


//  	
	//PORTC = fnd[3];
	//_delay_ms(1000);
  	/*
	for(i = 0 ; i<16 ; i++) {
      PORTC = fnd[i];
	  PORTB = fnd[i];
      _delay_ms(500);
	}



      PORTA = 0x01;
	  PORTC = fnd[1];
	  _delay_ms(dt);
	  PORTA = 0x02;
	  PORTC = fnd[2];
	  _delay_ms(dt);
	  PORTA = 0x04;
	  PORTC = fnd[3];
	  _delay_ms(dt);
	  PORTA = 0x08;
	  PORTC = fnd[4];
	  _delay_ms(dt);


	
	Fnd_Display(1,4);
	_delay_ms(20);
	Fnd_Display(2,3);
	_delay_ms(20);
	Fnd_Display(3,2);
	_delay_ms(20);
	Fnd_Display(4,1);
    _delay_ms(20);	

*/
