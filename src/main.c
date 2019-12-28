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

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <stdio.h> 

void initialize_POT(void); 
void initialize_INT(void); 
void initialize_TIM(void); 

void seg1(int digit, int num);
void seg2(int digit, int num);
void Fnd_Display(int digit, int num); //FND Display Func
void testseq(void);

const unsigned char fndc[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x27,0x7f,0x67};//Type CK
const unsigned char fnd[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98,0x88,0x83,0xc6,0xa1,0x86,0x8e};  //Type CA
const unsigned dt = 1;

/*
typedef struct _about_time
{ 
  volatile unsigned char hh, mi, ss, ms; 
  volatile unsigned char alh, alm, als;
  volatile unsigned char yy, mo, dd;
  volatile unsigned char mode, alarm;  
}Tdata;
Tdata TIME; */

volatile unsigned char hh=23, mi=59, ss=55, ms=00; 
volatile unsigned char alh=2, alm=10, als=55;
volatile unsigned char yy=15, mo=06, dd=01; 
volatile unsigned char mode=0, alarm=1;


volatile unsigned char interrupt_count=100,stw_count=100; 

int main(void)
{
  initialize_POT(); 
  //initialize_INT(); 
  //initialize_TIM();    
  
  do {
    //

	//
    testseq();  
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

void testseq(void) {
  int i=0,j=0,num=0;
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
	
}

/* ================================================================================= */
void initialize_INT(void) { 
  //Needs to be modiy//EIMSK = 0x03;
  EICRA = 0x03;//falling edge interrupt request 
  sei();//SREG MSBit Set 1 - all interrupt ON 
} 
  
  
/* ================================================================================= */
void initialize_POT(void) {
  DDRA = 0xff;
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0x0f;
  // DDRA=0xff; 
  // DDRB=0x0f; 
  // DDRC=0xff; 
  // DDRD=0xf0; 
  // DDRE=0xff; 
  // DDRF=0x01; 
} 

/* ================================================================================= */  
void initialize_TIM(void) { 
  TCCR0 = 0x07; 
  TCNT0 = 0x70;//{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
  TCCR2 = 0x07; 
  TCNT2 = 0x70;//{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
  TIMSK = 0x41; 
  TIFR = 0x40; 
  sei();//enable all interrupts 
} 
  
/* timer0 ================================================================================= */
SIGNAL(SIG_OVERFLOW0)//timer0 Overflow interrupt 
{ 
  interrupt_count--; ms++; 
    if(!interrupt_count){//10ms * 100 = 1000ms delay = 1s 
        interrupt_count = 100; ss++; ms=0; 
    } 
  TCNT0 = 0x70; 
} 
  
  
/* timer1 ================================================================================ */
/*
SIGNAL(SIG_OVERFLOW2)//timer1 Overflow interrupt 
{ 
  stw_count--; 
    if(!stw_count){//10ms * 100 = 1000ms delay = 1s 
        stw_count = 100; sws++; 
    } 
  TCNT2 = 0x70; 
} */
  


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

