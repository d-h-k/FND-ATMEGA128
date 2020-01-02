

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
    if(!SW1){            // SW==0? ����ġ ����
        if(PushFlg1){     // ó�� low ���� üũ
             PushFlg1=0;  // �÷��� clear �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x10 ^ PORTA);
             _delay_ms(100);
        };
    }
    else {                // SW==1? ����ġ ������ ����
        if(!PushFlg1){    // ó�� high ���� üũ
             PushFlg1=1;  // �÷��� set �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x10 ^ PORTA);
             _delay_ms(100);
        };
    }



	if(!SW2){            // SW==0? ����ġ ����
        if(PushFlg2){     // ó�� low ���� üũ
             PushFlg2=0;  // �÷��� clear �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x20 ^ PORTA);
             _delay_ms(100);
        };
    }
    else{                // SW==1? ����ġ ������ ����
        if(!PushFlg2){    // ó�� high ���� üũ
             PushFlg2=1;  // �÷��� set �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x20 ^ PORTA);
             _delay_ms(100);
        };
    }




	if(!SW3){            // SW==0? ����ġ ����
        if(PushFlg3){     // ó�� low ���� üũ
             PushFlg3=0;  // �÷��� clear �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x40 ^ PORTA);
             _delay_ms(100);
        };
    }
    else{                // SW==1? ����ġ ������ ����
        if(!PushFlg3){    // ó�� high ���� üũ
             PushFlg3=1;  // �÷��� set �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x40 ^ PORTA);
             _delay_ms(100);
        };
    }



	if(!SW4){            // SW==0? ����ġ ����
        if(PushFlg4){     // ó�� low ���� üũ
             PushFlg4=0;  // �÷��� clear �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x80 ^ PORTA);
             _delay_ms(100);
        };
    }
    else{                // SW==1? ����ġ ������ ����
        if(!PushFlg4){    // ó�� high ���� üũ
             PushFlg4=1;  // �÷��� set �ؼ� �������� ���� ���ϰ� ��
             PORTA = (0x80 ^ PORTA);
             _delay_ms(100);
        };
    }
	
  }
}
