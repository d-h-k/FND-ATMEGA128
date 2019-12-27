#include <avr/io.h>
#include <util/delay.h>
unsigned char fnd_num[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,
 0x7c,0x27,0x7f,0x67}; //FND�� ǥ���� ���ڵ��� �迭�� ������ ��
int fnd_sel_data; //FND �ڸ��� ����
int fnd_data = 0; //FND ǥ���� ���� ���ϱ�

void Fnd_Display(int fnd_sel, int num); //FND Display �Լ�

int main(void)
{
  int i=0;
  DDRB = 0xff; //��Ʈ ����
  DDRD = 0x0f;
  DDRA = 0xff;
  PORTA = 0xff;

  while (1)
  {
  	for(i = 1 ; i<9 ; i++) {
	  Fnd_Display(i,fnd_data); //�Լ� ȣ�� ��1��° �ڸ��� fnd_data �� ��¡�
	}
    	
  }
}

void Fnd_Display(int fnd_sel, int num) {

  if(fnd_sel == 1) fnd_sel_data = 0x08;
  else if(fnd_sel == 2) fnd_sel_data = 0x04;
  else if(fnd_sel == 3) fnd_sel_data = 0x02;
  else if(fnd_sel == 4) fnd_sel_data = 0x01;
PORTD = fnd_sel_data;
PORTB = fnd_num[num];
}
