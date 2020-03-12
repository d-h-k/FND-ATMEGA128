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
void cal_demon(void);
void dgt_test(void);
void base_tim(void);
void fast_tim(void);

volatile unsigned char PushFlg[5]={0};
volatile unsigned char Switch=0,i=0;

//const unsigned char fndc[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x27,0x7f,0x67};//Type CK
const unsigned char fnd[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98,0x88,0x83,0xc6,0xa1,0x86,0x8e};  //Type CA
const unsigned char chatdt = 5;
const unsigned int tcnt1_cnt = 64911;//nomal=64911//test3x=65411//test10x=65505


volatile unsigned char fnd_dgt=0,fnd_buf[9]={0},mode;
volatile unsigned char hh=23,mi=59,ss=51,ms10x=00,interrupt_count=100; 
volatile unsigned char yy=19,mo=12,dd=31; 


int main(void)

{ 
  unsigned char swbuf=0;
  initialize_POT(); 
  //initialize_INT(); 
  initialize_TIM();    


  do {
    
	switch(mode) {
	  case 0:
	    base_tim();
        break;

	  case 1:
        fast_tim();
        break;
/*
	  case 2:
        //
        break;

  	  case 3:
	    //
        break;
 */
      default:
        dgt_test(); 
		break;
    }


  	_delay_ms(100);
	cal_demon();
	swbuf = swread();
	if(swbuf==4) {
	  mode++;
	  if(mode > 3) {
	    mode = 0;
	  }
	}

  }while (1);
}

/* ================================================================================= */

void initialize_POT(void) {

  DDRA = 0xff;
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0x0f;
  PORTD |= 0xf0;

} 

 

/* ================================================================================= */

void initialize_INT(void) { 

  //Needs to be modiy//EIMSK = 0x03;
  //EICRA = 0x03;//falling edge interrupt request 
  //sei();//SREG MSBit Set 1 - all interrupt ON 

} 

  

  

/* ================================================================================= */  

void initialize_TIM(void) { 
  //interrupt disable
  cli();
  
  //Timer init
  //TIMSK = 2;
  TIMSK |= (1<<OCIE0) | (1<<OCIE2) | (1<<TOIE1);
  //TIFR = 0x40; 

  //Timer0 Cofig
  TCCR0 |= (1<<WGM01) | (1<<WGM00) | (1<<CS02);
  //TCCR0 &= (1<<WGM00); 
  //TCCR0 = 0x0c;//0000 1100
  OCR0 = 249; 
  

  //Timer1 Config
  TCNT1 = tcnt1_cnt;
  TCCR1A = 0x00;
  TCCR1B = (1<<CS12) | (0<<CS11) | (0<<CS10);//prescaler 256
  //fout = 10ms interrupt, fs=256, TCNT1 = 64911


  //Timer2 Confuig  
  //TCCR2 |= (1<<CS22) | (1<<CS20);//Prescaler ??
  TCNT2 = 0x70;//{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
  

  
  //TCNT0 = 0x70;
  
  sei();//enable all interrupts
  
} 

  

/* Timer0 ================================================================================= */
ISR(TIMER0_COMP_vect){ // timer0 Overflow interrupt
    
	PORTA &= ~(0x0f);
	PORTD &= ~(0x0f);
    
	switch(fnd_dgt){

        case 1:
		  PORTC=fnd[fnd_buf[1]];
		  PORTB=fnd[fnd_buf[5]];
		  PORTA |= (0x01<<3);
		  PORTD |= (0x01<<3);
		  break;

        case 2: 
		  PORTC=fnd[fnd_buf[2]];
		  PORTB=fnd[fnd_buf[6]];
		  PORTA |= (0x01<<2);
		  PORTD |= (0x01<<2);
		  break;

        case 3: 
		  PORTC=fnd[fnd_buf[3]];
		  PORTB=fnd[fnd_buf[7]];
		  PORTA |= (0x01<<1);
		  PORTD |= (0x01<<1);
	      break;

        case 4: 
		  PORTC=fnd[fnd_buf[4]];
		  PORTB=fnd[fnd_buf[8]];
		  PORTA |= (0x01<<0);
		  PORTD |= (0x01<<0);
		  break;

		default : 
		  break;
    }

	if(++fnd_dgt>4) fnd_dgt=1;

}


/* Timer1 ================================================================================= */
ISR(TIMER1_OVF_vect) {
  TCNT1 = tcnt1_cnt;
  ms10x++;
  if(ms10x >= 100) {
    ss++;
	ms10x=0;
  }

}



/* Timer2 ================================================================================= */
ISR(TIMER2_OVF_vect) { //timer2 Overflow interrupt, every 10ms 


  
  //TCNT2 = 0x70; 
} 

/* Switch read func wich chattering-countermeasure ================================================================================= */
char swread() {
	static unsigned char i=0;
	for( i=4 ; i<8 ; i++ )    
		if (PIND & (1<<i)) {
		   if(1==PushFlg[i-4]) {
		     PushFlg[i-4] = 0;
			 PORTA = PORTA^(1<<i);
			 //PORTA &= ~(1<<i);
			 _delay_ms(chatdt);
			 return (i-3);
		   }
        }
        else {
		  if(0 == PushFlg[i-4]) {
		    PushFlg[i-4] = 1;
		    _delay_ms(chatdt);
			PORTA = PORTA^(1<<i);
		  }	
        }

return -1;
}


void cal_demon(void) { 
  if(ss>=60){ss-=60;mi++;} 
  if(mi>=60){mi-=60;hh++;} 
  if(hh>=24){hh-=24;dd++;} 
  if(dd>=32){dd-=32;mo++;} 
  if(mo>=12){mo-=12;yy++;} 
} 
  

void dgt_test(void) {

  fnd_buf[1] = 1;
  fnd_buf[2] = 2;	
  fnd_buf[3] = 3;
  fnd_buf[4] = 4;
  fnd_buf[5] = 5;
  fnd_buf[6] = 6;
  fnd_buf[7] = 7;
  fnd_buf[8] = 8;

}


void base_tim(void) {

  fnd_buf[1] = ss%10;
  fnd_buf[2] = ss/10;
  fnd_buf[3] = mi%10;
  fnd_buf[4] = mi/10;
  fnd_buf[5] = hh%10;
  fnd_buf[6] = hh/10;
  fnd_buf[7] = dd%10;
  fnd_buf[8] = dd/10;

}



void fast_tim(void) {

  fnd_buf[1] = ms10x%10;
  fnd_buf[2] = ms10x/10;
  fnd_buf[3] = ss%10;
  fnd_buf[4] = ss/10;
  fnd_buf[5] = mi%10;
  fnd_buf[6] = mi/10;
  fnd_buf[7] = hh%10;
  fnd_buf[8] = hh/10;
  

}

