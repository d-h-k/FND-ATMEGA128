/* =====[ Discription   ]=======
 *
 * PORTA = LED, CA->Q5~Q8/FND RIGHT
 * PORTC = FND Numaric Display->Q5~Q8/FND RIGHT
 *
 * PORTB = FND Numaric Display->Q1~Q4/FND LEFT
 * PORTD = Switch, CA->Q1~Q4/FND LEFT
 *
 */

#define F_CPU 16000000UL // 16 MHz
#define  SW1     (PIND&0x10)
#define  SW2     (PIND&0x20)
#define  SW3     (PIND&0x40)
#define  SW4     (PIND&0x80)
//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <stdio.h> 
//
void initialize_POT(void); 
void initialize_INT(void); 
void initialize_TIM(void); 

void swread();

//const unsigned char fndc[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x27,0x7f,0x67};//Type CK
const unsigned char fnd[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98,0x88,0x83,0xc6,0xa1,0x86,0x8e};  //Type CA
const unsigned dt = 1;

volatile unsigned char fnd_dgt=0,fnd_buf[9]={0};
//volatile unsigned char hh=23, mi=59, ss=55, ms=00; 
//volatile unsigned char alh=2, alm=10, als=55;
//volatile unsigned char yy=15, mo=06, dd=01; 
//volatile unsigned char mode=0, alarm=1;
//volatile unsigned char interrupt_count=100,stw_count=100; 

volatile unsigned char PushFlg1=0,PushFlg2=0,PushFlg3=0,PushFlg4=0;
volatile unsigned char Switch=0,i=0;

int main(void)

{
	

  initialize_POT(); 
  //initialize_INT(); 
  //initialize_TIM();    

  fnd_buf[3] = 3;
  fnd_buf[4] = 4 ;
  fnd_buf[1] = 1;
  fnd_buf[2] = 2;

	

  do {
  	_delay_ms(10);
    //for(int i=0 ; i<4 ; i++) {
	
	if (PIND & (1<<PIND4)) {
	  PORTA &= ~(1<<4);
	}
	else {
	  PORTA |= (1<<4);
	}

     
/*
    if(SW1 == 0){
      if(PushFlg1 == 1){     // 처음 low 인지 체크
        PushFlg1=0;  // 플래그 clear 해서 연속으로 받지 못하게 함
        _delay_ms(10);
	    
	  }
	  else {}
    }
    else{
      PushFlg1=1;
    }*/

    
    
  }while (1);
}
/* ================================================================================= */
void initialize_POT(void) {
  DDRA = 0xff;
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0x0f;
  PORTA = 0xf0;
  //PORTD |= 0xf0;
  // DDRA=0xff; 
  // DDRB=0x0f; 
  // DDRC=0xff; 
  // DDRD=0xf0; 
  // DDRE=0xff; 
  // DDRF=0x01; 
} 
 
/* ================================================================================= */
void initialize_INT(void) { 
  //Needs to be modiy//EIMSK = 0x03;
  //EICRA = 0x03;//falling edge interrupt request 
  //sei();//SREG MSBit Set 1 - all interrupt ON 
} 
  
  
/* ================================================================================= */  

void initialize_TIM(void) { 
  
  cli();
  TCCR0 |= (1<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
  TCCR0 &= (0<<WGM00); 
  TCCR0 = 0x0c;//0000 1100
  
  //TCNT0 = 0x00;
  
  OCR0 = 249; 
 
  TIMSK = 2;
  //TIMSK |= (1<<OCIE0);
  
  SREG = 0x80;
  
  //TIFR = 0x40; 
  //TCNT0 = 0x70;
  //TCCR2 = 0x07; 
  //TCNT2 = 0x70;//{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
  //TIMSK = 2; 
  
  sei();//enable all interrupts
  
 
} 


ISR(TIMER0_COMP_vect){ // timer0 Overflow interrupt
    PORTA = 0x00; //캐소드
	//PORTC = 0x00;
    //PORTB=0; //애노드
	//PORTA = (0xf0 ^ PORTA);
	PORTA = (0x30 ^ PORTA);
    switch(fnd_dgt){
        //캐소드
        case 1:
		  PORTC=fnd[fnd_buf[0]];
		  PORTA=1; 
		  break;
        case 2: 
		  PORTC=fnd[fnd_buf[1]];     
		  PORTA=2; 
		  break;
        case 3: 
		  PORTC=fnd[fnd_buf[2]];		
		  PORTA=4; 
		  break;
        case 4: 
		  PORTC=fnd[fnd_buf[3]];
		  PORTA=8;
		  break;
		default : 
		  break;
        //애노드
        //case 0: PORTA=fnd_table[fnd_buf[0]];      PORTB=1; break;
        //case 1: PORTA=fnd_table[fnd_buf[1]];      PORTB=2; break;
        //case 2: PORTA=fnd_table[fnd_buf[2]]&0x7F; PORTB=4; break;
        //case 3: PORTA=fnd_table[fnd_buf[3]];      PORTB=8; break;
    }
	if(++fnd_dgt>4) fnd_dgt=1;
}


void swread() {
    if (PIND & (1<<PIND4)) {
	  PORTA &= ~(1<<4);
	}
	else {
	  PORTA |= (1<<4);
	}

}

/*
if (PIND & (1<<PIND4)) {
	  PORTA &= ~(1<<4);
	}
	else {
	  PORTA |= (1<<4);
	}
*/
