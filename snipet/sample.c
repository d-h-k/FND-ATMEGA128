/*======================================================================== 
1.����ġ ���̽� ������ defualt ���� �����Ǿ������� ���� 
-> defualt ���� �� ���� 

2.��� ���� ���� -> �ڵ� ¥�鼭 �� ��� �����ؾ� �ڴ�! �����ߴµ� �������� ���� �͵�  
   2.1. �ð��� 10���� ���� ��� ����(�����ġ, �˶�) 
   2.2. 4��, 100�� ���� ���� ���� ��� ���� 
   2.3. ���Ƿ����� ���� ���� ������� ���� 

3. ���̽��� �µ� ���� -> ntc ���̽��ͷ� �µ� ��������� �־��µ� ���̽��� �µ�Ư�� Ŀ�긦 �״�� �������� �ʰ� ����ȯ���� 10��~25�� ���̸� �뷫 ����� �µ����� �������� �����Լ��� �����ھҽ��ϴ�->�迭���� �� ���ϵ��� ����
->>��� ŰƮ���� �÷��ֽ� NTC���̽��� ���� �о��µ� �ƹ��� ���� ���ص� �ȵǰ� ��ɱ����� �����ؼ�... ������Ʈ �������� ���������� ���� ���� ������ ����� �Ѱ���...

4.���̳��� ���÷��� -> ���̳��� ���÷��̸� �׳� ������ ������ �����̰�, ���ƴ� ������
Ÿ�̸� ī��Ʈ ���ͷ�Ʈ�� �̿��ؼ� ���ڸ��� ��Ȯ�ϰ� 1�ʿ� 60ȸ ǥ���ϵ��� ����

5.�ð谡 �ð谡 �ƴϴ�-> RTC + EEPROM �̿��ؼ� �ð��� �����ϰų� Ȥ�� �ø��� ������� �ð������͸� �޾ƿ� ����� ��� �� �Դϴ�.

6.�ڵ���Ÿ�� �ƽ��� -> 
           1. �ߺ��Ǵ� �ڵ� ����(�ݺ������� �����ϰ� ¥�� ������)
           2. �鿩����, �ּ� ����
           3. upgrading...
============================================================================
�ڵ帮�� �ϸ鼭 ���� § �ڵ忡 �ƽ������� �Դϴ�. ��� ȸ�翡�� �ڵ帮�� �ϸ� ���� ��ġ�� ������.. ��ġ�� ������ �ʴµ� ����� ��⿣ ����... .. ��������.. ;��;


�Ʒ��� �ڵ��Դϴ�. ������ ���� ���� �����մϴ�!!


    File Name : AVR Timer //designed by KDH ----university Embededsysytem 
========================================================================*/ 

//=================================================================================�̸������ص� �ش������� �������ִ� �κ��Դϴ�. 
#define F_CPU 14745600UL //����Ŭ�� ���� 
#include <avr/io.h>    
#include <avr/interrupt.h> 
#include <util/delay.h> 
#include <stdio.h> 

//=================================================================================�ʱ� ����(�������� ����)�� ����Ҽ� �ֵ��� ���ǵ� �Լ��� �����ϴ� �κ��Դϴ�. 
void initialize_timerct(void); 
void initialize_INT(void); 
void initialize_PORT(void); 
void initialize_TIME(void); 
void initialize_adc(void); 

//=================================================================================ǥ���Լ� �Դϴ� ��8������ ��尡 �����˴ϴ�. 
void MODE_TIMER(void);//�⺻ �ð��� 24�ð� ��� 
void MODE_MTIMER(void);//���� �ð���  
void MODE_DATE(void);//��¥ ǥ�ø�� �Դϴ� 
void MODE_STOPWATCH(void);//��ž��ġ �Դϴ� 
void MODE_SLEEP(void);//������� �Դϴ�. 
void MODE_ALARM(void);//�˶���� �Դϴ�. 
void MODE_TEMP(void);//�µ��� ��Ÿ���� �κ��Դϴ� 
void MODE_OFFF(void);//OFF����Դϴ�. 
void buzzer(void);//���� �Լ� 

//=================================================================================������ ������ ���ִ� �κ��Դϴ� 
volatile unsigned char n=0; 
volatile unsigned char hh=23,mi=59,ss=55,ms=00; 
volatile unsigned char yy=15,mo=06,dd=01; 
volatile unsigned char interrupt_count=100,stw_count=100; 
volatile unsigned char fnd[16] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//CAŸ�� SR-2156A FND����� ���� ���� 
volatile unsigned char swh=0,swm=0,sws=0,sleep=0x80; 
volatile unsigned char mode=0; 
volatile unsigned char alh=2,alm=10,als=55,alarm=1;  
volatile unsigned int adc_result=0, temp=0; 
//volatile unsigned char  

//=================================================================================������ ���ǹ��� �����س��� �κ��Դϴ� 
#define FND_TIME _delay_ms(1);//1ms=167Hz, 2ms=83Hz, 3ms=55Hz, 4ms=41Hz, 5ms=33Hz, 6ms=27Hz //�ּ� 30Hz �̻��̸� ǥ�ÿ� ������ �����ϴ�. 
#define SLEEP_TIME _delay_ms(100);  
#define MIST_TIME _delay_us(50);  
#define buzzer_on PORTF=0x01;_delay_ms(10);PORTF=0x00; 
#define LED_on PORTD=0x00;_delay_ms(10);PORTD=0xff; 
// 
// 
// 
//=================================================================================����� �����Լ� �Դϴ� �ݺ��� �ʿ���°��� do{}while �� �ۿ� ��ġ�մϴ�. 
int main(void) 
{ 
initialize_PORT(); 
initialize_INT(); 
initialize_timerct();    
initialize_adc(); 
PORTF=0x00;PORTD=0xff;  
do{ 

initialize_TIME(); 
PORTC = (0xff<< mode); 
  
switch(mode){ 
        case 0: MODE_TIMER(); break; 
        case 1: MODE_MTIMER(); break; 
        case 2: MODE_DATE(); break; 
        case 3: MODE_STOPWATCH(); break; 
      case 4: MODE_SLEEP(); break; 
      case 5: MODE_ALARM(); break; 
      case 6: MODE_TEMP(); break; 
      case 7: MODE_OFFF(); break; 
        
} 
   }while(1); 
} 
  
//================================================================================= ������� �����Լ� �Դϴ� 
// 
// 
// 
  
  
  
//=================================================================================�ܺ� ���ͷ�Ʈ��� ������ �κ��Դϴ�. 
//�ܺ� ���ͷ�Ʈ�� �ΰ� ����ϱ� ������(EXINT0, EXINT1) �̸� �������ݴϴ�. 
void initialize_INT(void) 
{ 
EIMSK = 0x03;//�ܺ� ���ͷ�Ʈ 0(INT0) �� �ܺ����ͷ�Ʈ1�� ����ϱ� �����Դϴ�. 
EICRA = 0x03;//falling edge interrupt request 
sei();//SREG�� �ֻ��� ��Ʈ��1�� ���� - all interrupt ON 
} 
  
  
//================================================================================= ��Ʈ ������ �����ϴ� �κ��Դϴ�. 
void initialize_PORT(void)//��Ʈ�� ����� ������ �����մϴ� 
{//0�� �Է� 1�� ��� 
DDRA=0xff; 
DDRB=0x0f; 
DDRC=0xff; 
DDRD=0xf0; 
DDRE=0xff; 
DDRF=0x01; 
} 
  
  
//================================================================================= 
void initialize_TIME(void)//�ð��� �����÷ο� �Ǹ� �����ڸ��� �Ѱ��ݴϴ� �̰� �ݺ��ؼ� ��� üũ�ؾ��մϴ�. 
{ 
// 
//1.�Ϲݽð�  2.���нð�  5.������� 
if(ms==100){ms=0;} 
if(ss==60){ss=0;mi++;} 
if(mi>60){mi=0;hh++;} 
if(hh==24){hh=0;dd++;} 
if(dd>31){dd=0;mo++;} 
if(mo==12){mo=0;yy++;} 
// 
//  
// 
//4.��ž��ġ, ��ī��Ʈ �ϴٰ� �����÷ΰ��� �����ϸ� üũ�� ȯ���˴ϴ�. 
if(sws>59){sws=0;swm++;} 
if(swm>60){swm=0;swh++;} 
if(swh>24){swh=0;} 
// 
// 
  
// 
//6.�˶� 
if(als==60){als=0;alm--;} 
if(alm==60){alm=0;alh--;} 
if(alh==24){alh=0;}//�˶��� 23�ð� 59�� 59�ʰ� �ִ��̰� ���̻����� �Ѿ�� 0�̵� 
if(als<0){alm--;als=60;} 
if(alm<0){alh--;alm=60;} 
if(alh<0){if(alm==0){if(als==0){alarm=0;buzzer_on;buzzer_on;buzzer_on;buzzer_on;}}} 
//�˶��� ��ī��Ʈ �ٿ�ī��Ʈ ��� ��쿡 �� �����ؾ���. 
} 
  
  
void initialize_ALARM(void)//�˶��� �ð��� �����÷ο� �Ǹ� �����ڸ��� �Ѱ��ݴϴ� �̰� �ݺ��ؼ� ��� üũ�ؾ��մϴ�. 
{ 
if(ms==100){ms=0;} 
if(ss==60){ss=0;mi++;} 
if(mi==60){mi=0;hh++;} 
if(hh==24){hh=0;dd++;} 
if(dd==31){dd=0;mo++;} 
if(mo==12){mo=0;yy++;} 
} 
  
  
//================================================================================= 
void initialize_timerct(void)//1�ʸ� ����� ���ؼ� Ÿ�̸�ī��Ʈ ���ͷ�Ʈ�� ����մϴ�. 
{ 
   TCCR0 = 0x07; 
    TCNT0 = 0x70;//{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
   TCCR2 = 0x07; 
    TCNT2 = 0x70;//{(0xff-0x70)+1} * 126 * (1/16Mhz) = 10ms 
   TIMSK = 0x41; 
   TIFR = 0x40; 
    sei();        //enable all interrupts 
} 
  
  
//================================================================================= 
void initialize_adc(void)//�Ƴ��α� ������ ����Ʈ  
{ 
    ADMUX = 0x66;//0x66 = 01100110 
                 //bit 7,6  REFS1~REFS0 :   01(AVCC w/External cap.at AREF pin) 
                 //bit 5    ADLAR       :   1 
                 //bit4~0   MUX4~MUX0   :   00110(ADC ch# 6.) 
    ADCSR = 0xEF;//0xEF = 11101111 
                 //bit7     ADEN    :   1(ADC enable) 
                 //bit6     ADSC    :   1(start conversion) 
                 //bit5     ADFR    :   1(free running mode) 
                 //bit4     ADIF    :   0(interrupt flag) 
                 //bit3     ADIE    :   1(interrupt enable) 
                 //bit2,1,0 ADPS2~0 :   111(128prescaler) 
                 //ADC CLOCK = system clock / 128 = 115.2KHz 
    SFIOR = 0x00; 
} 
  
  
  
  
//================================================================================= 
void initialize_adc(void); 
 //�Ƴ��α� ������ ����Ʈ �κ��Դϴ�. 
SIGNAL(SIG_ADC) 
{ 
    adc_result = ADCH;      //ADC ��ȯ�� save 
} 
  
  
  
//================================================================================= 
void MODE_TIMER(void) 
{DDRC=0xff; 
PORTE=1 ;PORTA=fnd[hh/10];if(ss%2)PORTC&=~0xff;FND_TIME; 
PORTE=2 ;PORTA=fnd[hh%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[mi/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[mi%10];FND_TIME; 
PORTE=16;PORTA=fnd[ss/10];FND_TIME; 
PORTE=32;PORTA=fnd[ss%10];FND_TIME; 
} 
  
//================================================================================= 
void MODE_MTIMER(void)//���нð��带 ǥ���ϴ� �κ��Դϴ�. 
{ 
PORTE=1 ;PORTA=fnd[mi/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[mi%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[ss/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[ss%10];FND_TIME; 
PORTE=16;PORTA=fnd[ms/10];FND_TIME; 
PORTE=32;PORTA=fnd[ms%10];FND_TIME; 
} 
  
  
//================================================================================= 
void MODE_DATE(void)//��¥�� ǥ���ϴ� ����Դϴ�. 
{ 
PORTE=1 ;PORTA=fnd[yy/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[yy%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[mo/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[mo%10];FND_TIME; 
PORTE=16;PORTA=fnd[dd/10];FND_TIME; 
PORTE=32;PORTA=fnd[dd%10];FND_TIME; 
ss=0;} 
  
//================================================================================= 
void MODE_STOPWATCH(void)//��ž��ġ ��带 �����س��� �Լ��Դϴ�. 
{sws=ss; 
PORTE=1 ;PORTA=fnd[swh/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[swh%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[swm/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[swm%10];FND_TIME; 
PORTE=16;PORTA=fnd[sws/10];FND_TIME; 
PORTE=32;PORTA=fnd[sws%10];FND_TIME; 
} 
  
  
//================================================================================= 
//������� �����ϴ� ����� ����Ѱ� ���� : MIST_TIME = PORTE=0x00;PORTA=0x00;FND_TIME; 
// us������ �Ѱ� ms������ �����Ƽ� �ʴ� ������ Ƚ���� �ð��� �� �ǵ������� ���Դϴ�.  
void MODE_SLEEP(void) 
{ 
PORTE=1 ;PORTA=fnd[hh/10];MIST_TIME;PORTE=0x00;PORTA=0x00;FND_TIME; 
PORTE=2 ;PORTA=fnd[hh%10];MIST_TIME;PORTE=0x00;PORTA=0x00;FND_TIME; 
PORTE=4 ;PORTA=fnd[mi/10];MIST_TIME;PORTE=0x00;PORTA=0x00;FND_TIME; 
PORTE=8 ;PORTA=fnd[mi%10];MIST_TIME;PORTE=0x00;PORTA=0x00;FND_TIME; 
PORTE=16;PORTA=fnd[ss/10];MIST_TIME;PORTE=0x00;PORTA=0x00;FND_TIME; 
PORTE=32;PORTA=fnd[ss%10];MIST_TIME;PORTE=0x00;PORTA=0x00;FND_TIME; 
} 
  
  
//================================================================================= 
void MODE_ALARM(void)//�˶�����Դϴ�. 
{als--; 
PORTE=1 ;PORTA=fnd[alh/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[alh%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[alm/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[alm%10];FND_TIME; 
PORTE=16;PORTA=fnd[als/10];FND_TIME; 
PORTE=32;PORTA=fnd[als%10];FND_TIME; 
} 
  
//================================================================================= 
  
void MODE_TEMP(void)//�µ�ǥ�� ���� ����Դϴ�. 
{temp=adc_result; 
PORTE=1 ;PORTA=fnd[7];FND_TIME; 
PORTE=2 ;PORTA=0x86;  FND_TIME; 
PORTE=4 ;PORTA=fnd[temp/90];FND_TIME; 
PORTE=8 ;PORTA=fnd[temp%50];FND_TIME; 
PORTE=16;PORTA=fnd[temp%10];FND_TIME; 
//PORTE=32;PORTA=fnd[adc_result%10];FND_TIME; 
} 
  
  
//================================================================================= 
void MODE_OFFF(void) 
{DDRC=0x00; PORTE=0x00; _delay_ms(200);PORTC=0xff; 
} 
  
  
//================================================================================= 
  
  
  
  
//=================================================================================timer0 
SIGNAL(SIG_OVERFLOW0)//timer0 Overflow interrupt 
{ 
  interrupt_count--; ms++; 
    if(!interrupt_count){//10ms * 100 = 1000ms delay = 1s 
        interrupt_count = 100; ss++; ms=0; 
    } 
  TCNT0 = 0x70; 
} 
  
  
//=================================================================================timer1 
SIGNAL(SIG_OVERFLOW2)//timer1 Overflow interrupt 
{ 
  stw_count--; 
    if(!stw_count){//10ms * 100 = 1000ms delay = 1s 
        stw_count = 100; sws++; 
    } 
  TCNT2 = 0x70; 
} 
  
  
//================================================================================= 
SIGNAL(SIG_INTERRUPT0)//Exint0 Overflow interrupt 
{//����ġ�� ���������� ������ �����ϴ�. 
 mode++; buzzer_on;_delay_ms(2); if(mode>7){mode=0;}  
} 
  
//================================================================================= 
SIGNAL(SIG_INTERRUPT1)//Exint1 �ܺ����ͷ�Ʈ1 ����ġ�� ��庰�� ������ �ٲ�Ƿ� ���̽����� ����մϴ�. 
{ LED_on; 
 switch(mode){ 
  
        case 0: mi++;buzzer_on; break; 
        case 1: mi++;buzzer_on; break; 
        case 2: dd++;buzzer_on; break; 
        case 3: mo++;buzzer_on; break; 
   case 4: _delay_ms(2); break; 
   case 5: _delay_ms(2); break; 
   case 6: _delay_ms(2); break; 
   case 7: _delay_ms(2); break; 
}}
