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


void swread();

volatile unsigned char Switch=0,i=0;

int main(void)

{
  DDRA = 0xff;
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0x0f;
  PORTA = 0xf0;	

  do {
  	_delay_ms(10);
    
	
	
	if (PIND & (1<<PIND4)) {
	  PORTA &= ~(1<<4);
	}
	else {
	  PORTA |= (1<<4);
	}

     
   
    
  }while (1);
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
