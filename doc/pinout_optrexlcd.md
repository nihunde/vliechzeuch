Datasheet Controller: https://www.lcd-module.de/eng/pdf/zubehoer/hd61202.pdf

Pinout LCD-Board (Optrex mit 2x HD61202)
```
1 VCC,V1R,V1L 3,7,74
2 ADC,GND,CS1 1,78,92
!gebraten 3 Reset
!gebraten 4 EN
!gebraten 5 CS3
6 CS2 91
7 R/W 94
8 D/I 95
9 DB0 79
10 DB1 80
11 DB2 81
12 DB3 82
13 DB4 83
14 DB5 84
15 DB6 85
16 DB7 86
```

Verbindung Arduino - GLCD-Board für openGLCD
```
Vcc 1
GND 2
D12 16
D11 15
D10 14
D09 13
D08 12 
D07 11
D06 10
D05 09
D04 08
D03 07
D02 06
```

Constructor für u8g2
```
U8G2_KS0108_128X64_F u8g2(U8G2_R1, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ A4, /*dc=*/ A3, /*cs0=*/ A0, /*cs1=*/ A1, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);   // Set R/W to low!
```
