#include<Arduino.h>
#include<U8g2lib.h>
 
#ifdef U8X8_HAVE_HW_SPI
#include<SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include<Wire.h>
#endif
 
U8G2_KS0108_128X64_F u8g2(U8G2_R1, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ A4, /*dc=*/ A3, /*cs0=*/ A0, /*cs1=*/ A1, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);   // Set R/W to low!
 
#define chaos_width 64
#define chaos_height 128
static const unsigned char chaos_bits[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x0f, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x20, 0x28,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0xa0, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0x09, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x10, 0xf0, 0x0f,
   0x00, 0x00, 0x00, 0xc0, 0x1f, 0x7c, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0xe0,
   0x8f, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0xf0, 0xe3, 0xff, 0xff, 0x07,
   0x00, 0x00, 0xf8, 0xfd, 0xfd, 0xff, 0xff, 0x03, 0x00, 0x80, 0x3f, 0xc0,
   0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0x03, 0x00, 0xff, 0xff, 0xff, 0x01,
   0x00, 0x70, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0x01, 0x00, 0x90, 0xff, 0x7f,
   0xff, 0xff, 0xff, 0x00, 0x00, 0xe8, 0xff, 0xbf, 0xff, 0xff, 0x7f, 0x00,
   0x00, 0xf8, 0xff, 0xdf, 0xff, 0xff, 0x3f, 0x00, 0x00, 0xff, 0xff, 0xef,
   0xff, 0xff, 0x1f, 0x00, 0x80, 0xff, 0xff, 0xfb, 0xff, 0xff, 0x0f, 0x00,
   0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0x3e, 0xc0, 0xff,
   0xff, 0xff, 0x03, 0x00, 0x00, 0x0c, 0xde, 0xff, 0xff, 0xff, 0x00, 0x00,
   0x00, 0x00, 0x87, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x83, 0xff,
   0xff, 0x0f, 0x00, 0x00, 0x00, 0x80, 0x03, 0xff, 0xff, 0x01, 0x00, 0x00,
   0x00, 0x80, 0x40, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x20, 0x66, 0x1e,
   0x0e, 0x00, 0x00, 0x00, 0x00, 0x98, 0x3f, 0xfe, 0x0f, 0x00, 0x00, 0x00,
   0x00, 0x74, 0x1e, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x08, 0xff,
   0x03, 0x00, 0x00, 0x00, 0x80, 0x03, 0x80, 0xff, 0x01, 0x00, 0x00, 0x00,
   0x60, 0x00, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xe0, 0x7f,
   0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x7c, 0xf6, 0xe1, 0xc3, 0x87, 0x1f, 0x00, 0x00, 0xfe, 0xfe, 0xf3,
   0xe7, 0xcf, 0x3f, 0x00, 0x00, 0x87, 0x0e, 0x03, 0x76, 0xdc, 0x20, 0x00,
   0x00, 0x03, 0x06, 0xf3, 0x37, 0xd8, 0x07, 0x00, 0x00, 0x03, 0x06, 0xfb,
   0x37, 0x98, 0x1f, 0x00, 0x00, 0x03, 0x06, 0x1b, 0x36, 0x18, 0x38, 0x00,
   0x00, 0x87, 0x06, 0x1b, 0x77, 0x5c, 0x30, 0x00, 0x00, 0xfe, 0x06, 0xfb,
   0xe7, 0xcf, 0x3f, 0x00, 0x00, 0x7c, 0x06, 0xf3, 0xc6, 0x87, 0x1f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x80, 0xcf, 0xe6, 0xdb, 0x3e, 0x7c, 0xf8, 0x01,
   0xc0, 0xdf, 0xfe, 0xdb, 0x7f, 0xfe, 0xfc, 0x03, 0x00, 0xd8, 0x0e, 0xdb,
   0x61, 0x83, 0x0d, 0x02, 0xc0, 0xdf, 0x06, 0xdb, 0x60, 0xff, 0x7d, 0x00,
   0xe0, 0xdf, 0x06, 0xdb, 0x60, 0xff, 0xf9, 0x01, 0x60, 0xd8, 0x06, 0xdb,
   0x60, 0x03, 0x80, 0x03, 0x60, 0xdc, 0x06, 0xdb, 0x60, 0x07, 0x05, 0x03,
   0xe0, 0xdf, 0x06, 0xdb, 0x60, 0xfe, 0xfd, 0x03, 0xc0, 0xdb, 0x06, 0xdb,
   0x60, 0xfc, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00 };
 
void draw(void) {
// graphic commands to redraw the complete screen should be placed here
u8g2.drawXBMP(0,0,chaos_width,chaos_height,chaos_bits);
}
 
void setup(void) {
u8g2.begin();
}
 
void loop(void) {
// picture loop
u8g2.firstPage();
do {
draw();
} while( u8g2.nextPage() );
 
// rebuild the picture after some delay
delay(1000);
}
