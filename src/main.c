/* =====[ Discription   ]=======
 *
 * PORTA = LED, CA->Q5~Q8/FND RIGHT
 * PORTC = FND Numaric Display->Q5~Q8/FND RIGHT
 *
 * PORTB = FND Numaric Display->Q1~Q4/FND LEFT
 * PORTD = Switch, CA->Q1~Q4/FND LEFT
 *
 */

#define F_CPU   16000000UL // 16 MHz


//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void initialize_POT(void); 
void initialize_INT(void); 
void initialize_TIM(void); 

char swread(void);
volatile unsigned char PushFlg[5]={0};
volatile unsigned char Switch=0,i=0;

//const unsigned char fndc[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x27,0x7f,0x67};//Type CK
const unsigned char fnd[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98,0x88,0x83,0xc6,0xa1,0x86,0x8e};  //Type CA
const unsigned dt = 1;
volatile unsigned char fnd_dgt=0,fnd_buf[9]={0};

 

int main(void)

{ 
  initialize_POT(); 
  //initialize_INT(); 
  initialize_TIM();    
  fnd_buf[1] = 1;
  fnd_buf[2] = 2;	
  fnd_buf[3] = 3;
  fnd_buf[4] = 4;
  fnd_buf[5] = 5;
  fnd_buf[6] = 6;
  fnd_buf[7] = 7;
  fnd_buf[8] = 8;

  do {
  	_delay_ms(500);
	swread();
	fnd_buf[1]++;
	if(fnd_buf[1]>15) {
	  fnd_buf[1]=0;
	}    
  }while (1);
}

/* ================================================================================= */

void initialize_POT(void) {

  DDRA = 0xff;
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0x0f;
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

  

/* timer0 ================================================================================= */

/*

SIGNAL(SIG_OVERFLOW0)//timer0 Overflow interrupt 

{ 

  interrupt_count--; ms++; 

    if(!interrupt_count){//10ms * 100 = 1000ms delay = 1s 

        interrupt_count = 100; ss++; ms=0; 

    } 

  TCNT0 = 0x70; 

} 

  */

ISR(TIMER0_COMP_vect){ // timer0 Overflow interrupt
    
	PORTA &= !(0x0f); //캐소드
	PORTD &= !(0x0f);
	
	PORTC = 0x00;
    PORTB = 0x00;
	//PORTA = (0xf0 ^ PORTA);
	//PORTA = (0x30 ^ PORTA);
    
	switch(fnd_dgt){
        //캐소드
        case 1:
		  PORTC=fnd[fnd_buf[1]];
		  PORTB=fnd[fnd_buf[5]];
		  PORTA |= (0x01<<0);
		  PORTD |= (0x01<<0);
		  break;
        case 2: 
		  PORTC=fnd[fnd_buf[2]];
		  PORTB=fnd[fnd_buf[6]];
		  PORTA |= (0x01<<1);
		  PORTD |= (0x01<<1);
		  break;
        case 3: 
		  PORTC=fnd[fnd_buf[3]];
		  PORTB=fnd[fnd_buf[7]];
		  //PORTA &= (0x01<<1);		
		  PORTA |= (0x01<<2);
		  PORTD |= (0x01<<2);
	      break;
        case 4: 
		  PORTC=fnd[fnd_buf[4]];
		  PORTB=fnd[fnd_buf[8]];
		  //PORTA &= (0x01<<2);
		  PORTA |= (0x01<<3);
		  PORTD |= (0x01<<3);
		  break;
		default : 
		  break;
    }

	if(++fnd_dgt>4) fnd_dgt=1;

}

char swread() {
	static unsigned char i=0;
	for( i=4 ; i<8 ; i++ )    
		if (PIND & (1<<i)) {
		   if(1==PushFlg[i]) {
		     PushFlg[i] = 0;
			 PORTA = 0x80^PORTA;
			 _delay_ms(10);
			 //PORTA &= ~(1<<i);
			 return i;
		   }
        }
        else {
		  if(0 == PushFlg[i]) {
		    PushFlg[i] = 1;
		    _delay_ms(10);
            //PORTA |= (1<<i);
		  }	
        }

return -1;
}

/*
if (PIND & (1<<PIND4)) {
	  PORTA &= ~(1<<4);
	}
	else {
	  PORTA |= (1<<4);
	}
*/
