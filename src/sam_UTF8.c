/*======================================================================== 
1.스위치 케이스 구문에 defualt 값이 설정되어이있지 않음 
-> defualt 값을 꼭 써줌 

2.기능 구현 부족 -> 코드 짜면서 이 기능 구현해야 겠다! 생각했는데 구현하지 않은 것들  
   2.1. 시계의 10가지 동작 모드 구현(스톱워치, 알람) 
   2.2. 4년, 100년 간격 윤년 보정 기능 구현 
   2.3. 오실레이터 고정 오차 보정기능 구현 

3. 서미스터 온도 오차 -> ntc 서미스터로 온도 측정기능을 넣었는데 서미스터 온도특성 커브를 그대로 추종하지 않고 실험환경인 10도~25도 사이만 대략 비슷한 온도값이 나오도록 일차함수로 때려박았습니다->배열에서 값 비교하도록 수정
->>사실 키트님이 올려주신 NTC서미스터 글을 읽었는데 아무리 봐도 이해도 안되고 기능구현도 실패해서... 프로젝트 막바지에 땜빵식으로 대충 값만 나오게 만들고 넘겼죠...

4.다이나믹 디스플레이 -> 다이나믹 디스플레이를 그냥 빠르게 돌려라 수준이고, 돌아는 가지만
타이머 카운트 인터럽트를 이용해서 한자리를 정확하게 1초에 60회 표시하도록 변경

5.시계가 시계가 아니다-> RTC + EEPROM 이용해서 시간을 저장하거나 혹은 시리얼 통신으로 시간데이터를 받아올 방법을 고민 중 입니다.

6.코딩스타일 아쉬움 -> 
           1. 중복되는 코드 많음(반복문으로 간결하게 짜면 좋을듯)
           2. 들여쓰기, 주석 부족
           3. upgrading...
============================================================================
코드리뷰 하면서 제가 짠 코드에 아쉬운점들 입니다. 사실 회사에서 코드리뷰 하면 뭔가 눈치가 보이죠.. 눈치를 주지는 않는데 대놓고 까기엔 아직... .. 무서워요.. ;ㅁ;


아래는 코드입니다. 언제나 좋은 말씀 감사합니다!!


    File Name : AVR Timer //designed by KDH ----university Embededsysytem 
========================================================================*/ 

//=================================================================================미리정의해둔 해더파일을 선언해주는 부분입니다. 
#define F_CPU 14745600UL //메인클럭 설정 
#include <avr/io.h>    
#include <avr/interrupt.h> 
#include <util/delay.h> 
#include <stdio.h> 

//=================================================================================초기 설정(레지스터 설정)을 사용할수 있도록 정의된 함수를 선언하는 부분입니다. 
void initialize_timerct(void); 
void initialize_INT(void); 
void initialize_PORT(void); 
void initialize_TIME(void); 
void initialize_adc(void); 

//=================================================================================표시함수 입니다 총8가지의 모드가 제공됩니다. 
void MODE_TIMER(void);//기본 시계모드 24시간 모드 
void MODE_MTIMER(void);//정밀 시계모드  
void MODE_DATE(void);//날짜 표시모드 입니다 
void MODE_STOPWATCH(void);//스탑워치 입니다 
void MODE_SLEEP(void);//슬립모드 입니다. 
void MODE_ALARM(void);//알람모드 입니다. 
void MODE_TEMP(void);//온도를 나타내는 부분입니다 
void MODE_OFFF(void);//OFF기능입니다. 
void buzzer(void);//부저 함수 

//=================================================================================변수들 선언을 해주는 부분입니다 
volatile unsigned char n=0; 
volatile unsigned char hh=23,mi=59,ss=55,ms=00; 
volatile unsigned char yy=15,mo=06,dd=01; 
volatile unsigned char interrupt_count=100,stw_count=100; 
volatile unsigned char fnd[16] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//CA타입 SR-2156A FND사용을 위한 선언 
volatile unsigned char swh=0,swm=0,sws=0,sleep=0x80; 
volatile unsigned char mode=0; 
volatile unsigned char alh=2,alm=10,als=55,alarm=1;  
volatile unsigned int adc_result=0, temp=0; 
//volatile unsigned char  

//=================================================================================디파인 정의문을 선언해놓은 부분입니다 
#define FND_TIME _delay_ms(1);//1ms=167Hz, 2ms=83Hz, 3ms=55Hz, 4ms=41Hz, 5ms=33Hz, 6ms=27Hz //최소 30Hz 이상이면 표시에 지장이 없습니다. 
#define SLEEP_TIME _delay_ms(100);  
#define MIST_TIME _delay_us(50);  
#define buzzer_on PORTF=0x01;_delay_ms(10);PORTF=0x00; 
#define LED_on PORTD=0x00;_delay_ms(10);PORTD=0xff; 
// 
// 
// 
//=================================================================================여기는 메인함수 입니다 반복이 필요없는것은 do{}while 문 밖에 위치합니다. 
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
  
//================================================================================= 여기까지 메인함수 입니다 
// 
// 
// 
  
  
  
//=================================================================================외부 인터럽트제어가 가능한 부분입니다. 
//외부 인터럽트를 두개 사용하기 때문에(EXINT0, EXINT1) 이를 설정해줍니다. 
void initialize_INT(void) 
{ 
EIMSK = 0x03;//외부 인터럽트 0(INT0) 과 외부인터럽트1을 사용하기 위함입니다. 
EICRA = 0x03;//falling edge interrupt request 
sei();//SREG의 최상위 비트를1로 설정 - all interrupt ON 
} 
  
  
//================================================================================= 포트 방향을 설정하는 부분입니다. 
void initialize_PORT(void)//포트의 입출력 방향을 설정합니다 
{//0은 입력 1은 출력 
DDRA=0xff; 
DDRB=0x0f; 
DDRC=0xff; 
DDRD=0xf0; 
DDRE=0xff; 
DDRF=0x01; 
} 
  
  
//================================================================================= 
void initialize_TIME(void)//시간이 오버플로우 되면 다음자리로 넘겨줍니다 이건 반복해서 계속 체크해야합니다. 
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
//  
// 
//4.스탑워치, 업카운트 하다가 오버플로값에 도달하면 체크후 환원됩니다. 
if(sws>59){sws=0;swm++;} 
if(swm>60){swm=0;swh++;} 
if(swh>24){swh=0;} 
// 
// 
  
// 
//6.알람 
if(als==60){als=0;alm--;} 
if(alm==60){alm=0;alh--;} 
if(alh==24){alh=0;}//알람은 23시간 59분 59초가 최대이고 그이상으로 넘어가면 0이됨 
if(als<0){alm--;als=60;} 
if(alm<0){alh--;alm=60;} 
if(alh<0){if(alm==0){if(als==0){alarm=0;buzzer_on;buzzer_on;buzzer_on;buzzer_on;}}} 
//알람은 업카운트 다운카운트 모든 경우에 다 대응해야함. 
} 
  
  
void initialize_ALARM(void)//알람의 시간이 오버플로우 되면 다음자리로 넘겨줍니다 이건 반복해서 계속 체크해야합니다. 
{ 
if(ms==100){ms=0;} 
if(ss==60){ss=0;mi++;} 
if(mi==60){mi=0;hh++;} 
if(hh==24){hh=0;dd++;} 
if(dd==31){dd=0;mo++;} 
if(mo==12){mo=0;yy++;} 
} 
  
  
//================================================================================= 
void initialize_timerct(void)//1초를 만들기 위해서 타이머카운트 인터럽트를 사용합니다. 
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
void initialize_adc(void)//아날로그 디지털 컨버트  
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
 //아날로그 디지털 컨버트 부분입니다. 
SIGNAL(SIG_ADC) 
{ 
    adc_result = ADCH;      //ADC 변환값 save 
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
void MODE_MTIMER(void)//정밀시계모드를 표시하는 부분입니다. 
{ 
PORTE=1 ;PORTA=fnd[mi/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[mi%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[ss/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[ss%10];FND_TIME; 
PORTE=16;PORTA=fnd[ms/10];FND_TIME; 
PORTE=32;PORTA=fnd[ms%10];FND_TIME; 
} 
  
  
//================================================================================= 
void MODE_DATE(void)//날짜를 표시하는 모드입니다. 
{ 
PORTE=1 ;PORTA=fnd[yy/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[yy%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[mo/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[mo%10];FND_TIME; 
PORTE=16;PORTA=fnd[dd/10];FND_TIME; 
PORTE=32;PORTA=fnd[dd%10];FND_TIME; 
ss=0;} 
  
//================================================================================= 
void MODE_STOPWATCH(void)//스탑위치 모드를 선언해놓은 함수입니다. 
{sws=ss; 
PORTE=1 ;PORTA=fnd[swh/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[swh%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[swm/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[swm%10];FND_TIME; 
PORTE=16;PORTA=fnd[sws/10];FND_TIME; 
PORTE=32;PORTA=fnd[sws%10];FND_TIME; 
} 
  
  
//================================================================================= 
//희미함을 구현하는 방법은 잠깐켜고 꺼짐 : MIST_TIME = PORTE=0x00;PORTA=0x00;FND_TIME; 
// us단위로 켜고 ms단위로 꺼놓아서 초당 켜지는 횟수와 시간을 를 의도적으로 줄입니다.  
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
void MODE_ALARM(void)//알람모드입니다. 
{als--; 
PORTE=1 ;PORTA=fnd[alh/10];FND_TIME; 
PORTE=2 ;PORTA=fnd[alh%10];FND_TIME; 
PORTE=4 ;PORTA=fnd[alm/10];FND_TIME; 
PORTE=8 ;PORTA=fnd[alm%10];FND_TIME; 
PORTE=16;PORTA=fnd[als/10];FND_TIME; 
PORTE=32;PORTA=fnd[als%10];FND_TIME; 
} 
  
//================================================================================= 
  
void MODE_TEMP(void)//온도표시 설정 모드입니다. 
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
{//스위치를 누를때마다 부저가 켜집니다. 
 mode++; buzzer_on;_delay_ms(2); if(mode>7){mode=0;}  
} 
  
//================================================================================= 
SIGNAL(SIG_INTERRUPT1)//Exint1 외부인터럽트1 스위치는 모드별로 동작이 바뀌므로 케이스문을 사용합니다. 
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