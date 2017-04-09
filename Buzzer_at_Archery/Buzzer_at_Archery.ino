#include "TM1637.h"
#define CLK 2      
#define DIO 3
TM1637 tm1637(CLK,DIO);

#define BT_pin 6
#define sound_pin 4     //Active HIGH 
#define Light_pin 5     //Active HIGH

void setup() {
    Serial.begin(9600);
    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    showLED(0000);
    
    pinMode(BT_pin,INPUT);
    pinMode(sound_pin,OUTPUT);
    pinMode(Light_pin,OUTPUT);
    
    digitalWrite(BT_pin,LOW);
    digitalWrite(sound_pin,LOW);
    digitalWrite(Light_pin,LOW);
}

void loop() {

  while(digitalRead(BT_pin) != HIGH){
    delay(10);
    digitalWrite(Light_pin,HIGH);
  }
  
  for(int i=8; i>0; i--){
      showLED(i);
      delay(1000);
      //digitalWrite(Light_pin,HIGH);
  }

  
  sound(100);
  digitalWrite(Light_pin,LOW); //ปิดไฟ
  
    for(int i=240; i>=0; i--){
      showLED(i);
      delay(1000);
      if(digitalRead(BT_pin) == HIGH){
        delay(10);
        digitalWrite(Light_pin,HIGH);
        sound(1000);
        delay(100);
        software_Reset() ;// Reset
      }
      
  }

  digitalWrite(Light_pin,HIGH); //เปิดไฟ
  sound(1000);
  delay(100);
  software_Reset() ;// Reset

}


void showLED(int num){
    tm1637.display(0,num/1000);
    tm1637.display(1,num%1000/100); 
    tm1637.display(2,num%100/10);
    tm1637.display(3,num%10);
}

void sound(int T){
    digitalWrite(sound_pin,HIGH);
    delay(T);
    digitalWrite(sound_pin,LOW);
}

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  asm volatile ("  jmp 0");
}
