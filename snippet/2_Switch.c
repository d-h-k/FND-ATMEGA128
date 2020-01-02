

#define F_CPU 16000000UL // 16 MHz
#include <avr/io.h>
#include <util/delay.h>
//
#define  SW1     (PIND&0x10)
#define  SW2     (PIND&0x20)
#define  SW3     (PIND&0x40)
#define  SW4     (PIND&0x80)
#define  LED1    PORTD=1
#define  LED2    PORTD=2
//
char  PushFlg1=0;
char  PushFlg2=0;
char  PushFlg3=0;
char  PushFlg4=0;

//
int main(){
  
  unsigned char led = 0;
  DDRA = 0xff;
  DDRD = 0x0f;
  PORTA = 0xf0;	

  while(1){
    if(!SW1){            // SW==0? 스위치 누름
        if(PushFlg1){     // 처음 low 인지 체크
             PushFlg1=0;  // 플래그 clear 해서 연속으로 받지 못하게 함
             PORTA = (0x10 ^ PORTA);
             _delay_ms(100);
        };
    }
    else {                // SW==1? 스위치 누르지 않음
        if(!PushFlg1){    // 처음 high 인지 체크
             PushFlg1=1;  // 플래그 set 해서 연속으로 받지 못하게 함
             PORTA = (0x10 ^ PORTA);
             _delay_ms(100);
        };
    }



	if(!SW2){            // SW==0? 스위치 누름
        if(PushFlg2){     // 처음 low 인지 체크
             PushFlg2=0;  // 플래그 clear 해서 연속으로 받지 못하게 함
             PORTA = (0x20 ^ PORTA);
             _delay_ms(100);
        };
    }
    else{                // SW==1? 스위치 누르지 않음
        if(!PushFlg2){    // 처음 high 인지 체크
             PushFlg2=1;  // 플래그 set 해서 연속으로 받지 못하게 함
             PORTA = (0x20 ^ PORTA);
             _delay_ms(100);
        };
    }




	if(!SW3){            // SW==0? 스위치 누름
        if(PushFlg3){     // 처음 low 인지 체크
             PushFlg3=0;  // 플래그 clear 해서 연속으로 받지 못하게 함
             PORTA = (0x40 ^ PORTA);
             _delay_ms(100);
        };
    }
    else{                // SW==1? 스위치 누르지 않음
        if(!PushFlg3){    // 처음 high 인지 체크
             PushFlg3=1;  // 플래그 set 해서 연속으로 받지 못하게 함
             PORTA = (0x40 ^ PORTA);
             _delay_ms(100);
        };
    }



	if(!SW4){            // SW==0? 스위치 누름
        if(PushFlg4){     // 처음 low 인지 체크
             PushFlg4=0;  // 플래그 clear 해서 연속으로 받지 못하게 함
             PORTA = (0x80 ^ PORTA);
             _delay_ms(100);
        };
    }
    else{                // SW==1? 스위치 누르지 않음
        if(!PushFlg4){    // 처음 high 인지 체크
             PushFlg4=1;  // 플래그 set 해서 연속으로 받지 못하게 함
             PORTA = (0x80 ^ PORTA);
             _delay_ms(100);
        };
    }
	
  }
}
