/*
arduino ติดต่อ JOY PS2 , arduino with JOY PS2  

ตัวอย่างนี้ ง่ายกว่าที่คุณคิด มาดูหัวต่อ PS2 กันก่อนนะครับ

สามารถนำไปประยุกต์ไปควบคุมหุ่นยนต์หรืองานด้านอื่นๆ  ได้ครับ


จากภาพนะครับมีสายทั้งหมด 8 เส้นนะครับแต่ใช้จริงๆ 6 เล้นนะครับ

เส้นที่ใช้นะครับ  

ขา 1 ขา data 

ขา2 command 

ขา 4 ขา GND

ขา 5 VCC 3.3 V ห้ามจ่าย 5 V นะครับ

ขา 6 ขา acttion 

ขา 7 clock

เกิดคำถามว่าแล้วจะต่อกับไมโครยังไง ต้องตัดหัวของ joy ps2 ออกหรือปล่าว

ตอบ นะครับไม่ต้องตัดหัวออกครับ  มีadapter แปลงขายเพื่อต่อครับ แต่มีวิธีที่ง่ายกว่านั้นคือต่อสายตัวเมียๆเข้ากับ joyได้เลยมีภาพให้ดูครับ

แล้วอีกฝั่งนึงก็ไปต่อกับ arduino ครับ 

ตรงนี้ขาที่จะไปต่อกับ arduino ครับ

ขา 1 ขา data   ต่อกับ arduino ขา//12

ขา2 command   ต่อกับ arduino ขา //11

ขา 4 ขา GND ต่อกับ arduino ขา GND

ขา 5 VCC 3.3 V ห้ามจ่าย 5 V นะครับ ต่อกับ arduino 3.3V

ขา 6 ขา acttion  ต่อกับ arduino ขา //10

ขา 7 clock ต่อกับ arduino ขา//13

*/





#include <math.h>
#include <stdio.h>
#include <avr/io.h>

//#define LED_PIN 13
//#define DELAY(wait) digitalWrite(LED_PIN,LOW); delay(wait); digitalWrite(LED_PIN,HIGH);

/* These are AVR PORTB pins, +8 to convert to Arduino pins */
#define PS2clk 5 //13
#define PS2cmd 3 //11
#define PS2att 2//10
#define PS2dat 4//12
#define PS2PORT PORTB
#define PS2IN PINB
#define CTRL_CLK 20
#define CTRL_BYTE_DELAY 20

//These are our button constants
#define PSB_SELECT 0x01
#define PSB_L3 0x02
#define PSB_R3 0x04
#define PSB_START 0x08
#define PSB_PAD_UP 0x10
#define PSB_PAD_RIGHT 0x20
#define PSB_PAD_DOWN 0x40
#define PSB_PAD_LEFT 0x80

#define PSB_L2 0x100
#define PSB_R2 0x200
#define PSB_L1 0x400
#define PSB_R1 0x800
#define PSB_GREEN 0x1000
#define PSB_RED 0x2000
#define PSB_BLUE 0x4000
#define PSB_PINK 0x8000

#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))

boolean PSButton();
unsigned char PS2data[9];
void read_gamepad();
void config_gampad();
unsigned char get_gamepad_mode();
unsigned char i;



void setup() {
// randomSeed(analogRead(0));
Serial.begin(9600);
// pinMode(LED_PIN,OUTPUT);
// digitalWrite(LED_PIN,HIGH);

pinMode(PS2clk+8,OUTPUT);
pinMode(PS2att+8,OUTPUT);
pinMode(PS2cmd+8,OUTPUT);
pinMode(PS2dat+8,INPUT);
digitalWrite(PS2dat+8,HIGH);

config_gampad();
}
void loop () {
while(1)
{

delay(80) ;
read_gamepad(); 
readkey () ;

} 
}
boolean PSButton(unsigned int button) {
int byte = 3;
if (button >= 0x100) {
byte = 4;
button = button >> 8;
}
if (~PS2data[byte] & button) 
return true;
else
return false;
}
unsigned char _gamepad_shiftinout (char byte) {
unsigned char tmp = 0;
for(i=0;i<8;i++) {
if(CHK(byte,i)) SET(PS2PORT,PS2cmd);
else CLR(PS2PORT,PS2cmd);
CLR(PS2PORT,PS2clk); 
delayMicroseconds(CTRL_CLK); 
if(CHK(PS2IN,PS2dat)) SET(tmp,i); 
SET(PS2PORT,PS2clk); 
}
SET(PS2PORT,PS2cmd); 
delayMicroseconds(CTRL_BYTE_DELAY);
return tmp;
}
void _gamepad_shiftout (char byte) {
for(i=0;i<8;i++) {
if(CHK(byte,i)) SET(PS2PORT,PS2cmd);
else CLR(PS2PORT,PS2cmd);
CLR(PS2PORT,PS2clk); 
delayMicroseconds(CTRL_CLK); 
SET(PS2PORT,PS2clk); 
//delayMicroseconds(CTRL_CLK); 
}
SET(PS2PORT,PS2cmd); 
delayMicroseconds(CTRL_BYTE_DELAY);
}
unsigned char _gamepad_shiftin() {
unsigned char tmp = 0;
for(i=0;i<8;i++) {
CLR(PS2PORT,PS2cmd);
CLR(PS2PORT,PS2clk); 
delayMicroseconds(CTRL_CLK);
if(CHK(PS2IN,PS2dat)) SET(tmp,i); 
SET(PS2PORT,PS2clk); 
delayMicroseconds(CTRL_CLK);
}
SET(PS2PORT,PS2cmd); 
delayMicroseconds(CTRL_BYTE_DELAY);
return tmp;
}
void read_gamepad() {
SET(PS2PORT,PS2cmd); 
SET(PS2PORT,PS2clk); 
CLR(PS2PORT,PS2att); // low enable joystick
delayMicroseconds(CTRL_BYTE_DELAY);
char dword[9] = {0x01,0x42,0,0,0,0,0,0,0};
for (int i = 0; i<9; i++) {
PS2data[i] = _gamepad_shiftinout(dword[i]);
}
SET(PS2PORT,PS2att); // HI disable joystick
}
unsigned char get_gamepad_mode() {
SET(PS2PORT,PS2cmd); 
SET(PS2PORT,PS2clk); 
CLR(PS2PORT,PS2att); // low enable joystick
_gamepad_shiftout(0x01);
unsigned char x = _gamepad_shiftin();
SET(PS2PORT,PS2att); // HI disable joystick
return x;
}
void config_gampad() {
SET(PS2PORT,PS2cmd); 
SET(PS2PORT,PS2clk); 
CLR(PS2PORT,PS2att); // low enable joystick
_gamepad_shiftout(0x01);
_gamepad_shiftout(0x43);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x01);
_gamepad_shiftout(0x00);
// Lock to Analog Mode on Stick
_gamepad_shiftout(0x01);
_gamepad_shiftout(0x44);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x01);
_gamepad_shiftout(0x03);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x00);
// Vibration
/*
_gamepad_shiftout(0x01);
_gamepad_shiftout(0x4D);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x01);
*/
_gamepad_shiftout(0x01);
_gamepad_shiftout(0x4F);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0xFF);
_gamepad_shiftout(0xFF);
_gamepad_shiftout(0x03);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x01);
_gamepad_shiftout(0x43);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x00);
_gamepad_shiftout(0x5A);
_gamepad_shiftout(0x5A);
_gamepad_shiftout(0x5A);
_gamepad_shiftout(0x5A);
_gamepad_shiftout(0x5A);
SET(PS2PORT,PS2att);
}

void readkey ()
{


if(PSButton(PSB_PAD_UP))
{
Serial.println("1");
}
else if(PSButton(PSB_SELECT))
{
Serial.println("x");
}
else if(PSButton(PSB_PAD_DOWN))
{
Serial.println("2");
} 
else if(PSButton(PSB_PAD_RIGHT))
{
Serial.println("4");
}
else if(PSButton(PSB_PAD_LEFT))
{
Serial.println("3");
}


else if(PSButton(PSB_L3))
{
Serial.println("L3\n");
}
else if(PSButton(PSB_R3))
{
Serial.println("R3\n");
}


else if(PSButton(PSB_L1))
{
Serial.println("a");
}
else if(PSButton(PSB_R1))
{
Serial.println("c");
}
else if(PSButton(PSB_L2))
{
Serial.println("b");
}
else if(PSButton(PSB_R2))
{
Serial.println("d");
}
else if(PSButton(PSB_GREEN))
{
Serial.println("5");
}
else if(PSButton(PSB_RED))
{
Serial.println("8");
}
else if(PSButton(PSB_PINK))
{
Serial.println("7");
}
else if(PSButton(PSB_BLUE))
{
Serial.println("6");
}
else if(PSButton(PSB_START))
{
Serial.println("z");
}
else{ Serial.println("PLS control JOY");}

}
