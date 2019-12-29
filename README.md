# FND-ATMEGA128

- FND ATMEGA128

1. 개요 : 보드프리()의 BL-700, BL-703 보드 가지고 시계만들기
   - ![.](/img/bl703.png)
   - ![.](/img/lb700.png)
   
2. 환경 :
   - IDE : AVRstudio 4.18 , Winavr2010
   - Windows10 only
   - BL-700, BL-703 Board used
   - Atmega128, 16Mhz

3. 조립
   - 계산해보니 BJT의 BASE 단자 저항과 LED 직렬저항은 470옴,510옴 정도까지  사용해도 괜찮습니다.
   - Common Anode(이하 CA)Type FND를 사용하면 안됩니다(저는 집에 놀고있는 CA타입을 사용하느라 PCB를 칼로 뜯어내고 점퍼선날리는 대공사를 진행했습니다 ㅠㅠ.. 못할짓이네요)
   - Common Cathode(이하 CC)Type FND를 사용합니다. 핀맵은 아래와 같습니다. (아래 사진 참고)
   - 2SC1815 라는 BJT가 사용됬는데 일반적으로 많이쓰는 2n3904와 특성차이가 조금 있는데 FND Common gate로 사용할때는 뭐를 사용해도 문제가 되지 않습니다.
   - 단 위 2SC1815 와 2N3904 사이의 C,B,E 핀 배열이 서로 다릅니다. (아래 사진 참고)
   - ![.](/img/2SC1815-NPN-Transistor-Pinout.jpg)
   - 필요부품은 아래와 같습니다
     - FND * 2EA : CC(comman Cathod)타입 - 
     - 1/8W 탄소피막저항 330omh * 32EA
     - Tact Switch * 4EA
     - 3pi LED * 4EA
     - 2SC1815 BJT *8EA
   - 보드프리 사이트 메뉴얼은 codevision 용 소스코드이고, 제가 사용하는 컴파일러는 Winavr소스코드입니다. (예약어 차이 존재)

4. 참고문서
    - /resources 폴더에 위치

5. 예제코드
   - /snipet 폴더에 위치
