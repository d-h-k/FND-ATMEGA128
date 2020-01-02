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
void FND_Display(); //FND Display Func
void testseq1(void);
void testseq2(void);
void N_Clock(void);

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

typedef struct _fnd_param
{
  volatile unsigned char d1,d2,d3,d4;	
}fnd_param;
fnd_param FND1,FND2;

volatile unsigned char hh=23, mi=59, ss=55, ms=00; 
volatile unsigned char alh=2, alm=10, als=55;
volatile unsigned char yy=15, mo=06, dd=01; 
volatile unsigned char mode=0, alarm=1;


volatile unsigned char interrupt_count=100,stw_count=100; 

int main(void)
{
  initialize_POT(); 
  initialize_INT(); 
  //initialize_TIM();    
  FND1.d1=8;
  FND1.d2=7;
  FND1.d3=6;
  FND1.d4=8;
  FND2=FND1;
  do {
    //N_Clock();
    FND_Display();
	//testseq1();  
	//testseq2();  
  }while (1);
}

/*
void seg(int digit, int num) {
  int fnd_sel = 0x01;
  if(digit>=1 && digit<=4) {
    PORTD = fnd_sel<<(digit-1);
    PORTB = fnd[num];
  }
  else if( 5<=digit && digit<=8  ) {
    PORTA = fnd_sel<<(digit-5);
    PORTC = fnd[num];
  }
  
}*/
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


void FND_Display() {
  seg1(1,FND1.d1);_delay_ms(dt);
  seg1(2,FND1.d2);_delay_ms(dt);
  seg1(3,FND1.d3);_delay_ms(dt);
  seg1(4,FND1.d4);_delay_ms(dt);
  seg2(1,FND2.d1);_delay_ms(dt);
  seg2(2,FND2.d2);_delay_ms(dt);
  seg2(3,FND2.d3);_delay_ms(dt);
  seg2(4,FND2.d4);_delay_ms(dt);
}

void testseq1(void) {
  int i=0,j=0;
  for(j=0 ; j<725 ; j++) {
    for(i=1 ; i<=8 ; i++) {
      seg1(i,i);
	  seg2(i,i);
	  _delay_ms(dt);
	}
  }	
}

void testseq2(void) {
  int i=0;
  PORTA = 0x0f;
  PORTD = 0x0f;	
  for(i = 0 ; i<16 ; i++) {
    PORTC = fnd[i];
	PORTB = fnd[i];
    PORTA = (0xf0 ^ PORTA);
    _delay_ms(1000);
  }	
}

/* ================================================================================= */
void initialize_INT(void) { 
  //Needs to be modiy//EIMSK = 0x03;
  //EICRA = 0x03;//falling edge interrupt request 
  //sei();//SREG MSBit Set 1 - all interrupt ON 
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
  //TCCR2 = 0x07; 
  //TCNT2 = 0x70;//{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
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
  
void N_Clock(void)//
{ 
// 
//1.일반시계  2.정밀시계  5.슬립모드 
if(ms==100){ms=0;} 
if(ss==60){ss=0;mi++;} 
if(mi>60){mi=0;hh++;} 
if(hh==24){hh=0;dd++;} 
if(dd>31){dd=0;mo++;} 
if(mo==12){mo=0;yy++;} 
// 
//6.알람 
if(als==60){als=0;alm--;} 
if(alm==60){alm=0;alh--;} 
if(alh==24){alh=0;}//알람은 23시간 59분 59초가 최대이고 그이상으로 넘어가면 0이됨 
if(als<0){alm--;als=60;} 
if(alm<0){alh--;alm=60;} 
if(alh<0){if(alm==0){if(als==0){alarm=0;}}} 
//알람은 업카운트 다운카운트 모든 경우에 다 대응해야함. 
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

*/

