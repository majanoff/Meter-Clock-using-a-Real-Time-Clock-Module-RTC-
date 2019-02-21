/*
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 The analogWrite() function uses PWM, so if
 you want to change the pin you're using, be
 sure to use another PWM capable pin. On most
 Arduino, the PWM pins are identified with 
 a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

 This example code is in the public domain.
 */
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int meterP_H9 = 9;           // the PWM pin the LED is attached to
int meterP_M3 = 3;
int meterP_S5 = 5;
// buttons for setting the time
const int sw_control = 4;
const int sw_hours = 7;
const int sw_minutes = 8;
int meter_hour;
int saved_second = 0;
int old_second = 0;
int seconds = 0;
int hours = 0;
int minutes = 0;
 // variable for reading the pushbutton status
int buttonState = 0;    
int outer;    
const int ledPin =  13; 
int count;
int meter_hour_A[12] = {29,72,115,158,201,244,287,330,373,405,437,469};
int meter_minute_A[60] = {8,14,20,26,32,38,44,50,72,80,88,96,104,112,120,128,136,144,152,160,168,176,184,192,200,208,216,224,232,240,248,256,264,272,280,288,296,304,312,320,328,336,344,352,360,368,376,384,392,400,408,416,424,432,440,448,456,464,472};

int meter_second_A[60] = {0,1,2,3,29,29,30,30,31,31,32,32,33,33,34,34,35,35,36,36,37,37,38,38,39,39,40,40,41,41,42,42,43,43,44,44,45,45,46,46,47,47,48,48,49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56};
int meter_minute;
int meter_second;
int hour_offset;
int minute_offset;
int second_offset;
int offset;
//int pulse_6=6;
//int pulse_10=10;
//int pulse_11=11;
//int fivevolt = 227;
int counts = 0;
int counter = 0;
int loop_flag = 0; 

// the setup routine runs once when you press reset:
void setup() {
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
    while (1);
  }

//  if (rtc.lostPower()) {
//    Serial.println("RTC lost power, lets set the time!");
// following line sets the RTC to the date & time this sketch was compiled
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// This line sets the RTC with an explicit date & time, for example to set
// January 21, 2014 at 3am you would call:
 rtc.adjust(DateTime(2017, 2, 3, 0, 0, 0));
// }
  

// Positive H, M, S, pins  
  pinMode(meterP_H9, OUTPUT);
  pinMode(meterP_M3, OUTPUT);
  pinMode(meterP_S5, OUTPUT);
  analogWrite(meterP_H9, 0);
  analogWrite(meterP_M3, 0); 
  analogWrite(meterP_S5, 0);  
// Setup Time Control Switches
  pinMode(ledPin,OUTPUT);
  pinMode(sw_control, INPUT);
  pinMode(sw_hours, INPUT);
  pinMode(sw_minutes, INPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("begin test");
 
   analogWrite (3,5);
   
   delay (10000);
   analogWrite (3,180);
   
   delay (10000);
   
  for (outer=0; outer<50; outer++) {
   for (count=5;  count<190; count++) {
     analogWrite(3, count);
     Serial.println(count, DEC);
     delay (100);
  }
  
  }
}

// the loop routine runs over and over again forever:
void loop() {
    buttonState = digitalRead(sw_control);
    if (buttonState == LOW) {
      DateTime now = rtc.now();
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.print(':');
      Serial.println();

// get the time from the RTC
      meter_hour = now.hour();
      meter_minute = now.minute();
      meter_second = now.second();
      saved_second= meter_second;
      if (saved_second != old_second)
      {
       old_second = saved_second;  
       second_offset = meter_second/15;
       minute_offset = meter_minute/15;
       if (meter_second>=60) {
        meter_second=0;
       }
      }
// for 12 hour meter subtrack the pm hours
      if (meter_hour >= 12) {
        meter_hour = 0;
        hours=0;
      } 
// hours and minutes plus fractions of the hour and minutes
/* 
          Serial.print("H:");
          Serial.print(meter_hour,DEC);
          Serial.print("M:");
          Serial.print(meter_minute,DEC);
          Serial.print("S:");
          Serial.print(meter_second,DEC);
          Serial.println();
*/
          write_hour();
          write_minute();
          write_second();
     
    }
    else
    {
     while (digitalRead(sw_control) == HIGH) 
         {delay (200);}
     loop_flag=1;
     while (loop_flag) 
     {
     //Hours set
      digitalWrite(ledPin, HIGH);
      buttonState = digitalRead(sw_hours);
      if (buttonState == HIGH)
      {
        while (digitalRead(sw_hours) == HIGH) 
        {delay (200);}
        minute_offset=0;
        meter_hour=meter_hour+1;
        if (meter_hour>11)
          meter_hour=0;
        Serial.print(meter_hour, DEC);
        Serial.println();
        write_hour();    
       }
//minutes set  
       buttonState = digitalRead(sw_minutes);
       if (buttonState == HIGH)
       {
       while (digitalRead(sw_minutes) == HIGH) 
         {delay (200);}
       meter_minute=meter_minute+1;
       if (meter_minute>59)
          meter_minute=0;
       Serial.print(meter_minute, DEC); 
       Serial.println();  
       write_minute();    
        }
//return button
        buttonState = digitalRead(sw_control);
       if (buttonState == HIGH)
       {
        while (digitalRead(sw_control) == HIGH) 
         {delay (200);}
        Serial.print("control button pressed to return");
        Serial.println();
        loop_flag=0;
        digitalWrite(ledPin, LOW);
        rtc.adjust(DateTime(2017, 2, 3, meter_hour, meter_minute, 0));
        }
     }
     
     }
   }

//  write to hour meter  
    void write_hour()
    {
//      analogWrite(meterP_H9,meter_hour); 
        analogWrite(meterP_H9,511);
    }
// write to minute meter  
    void write_minute()  
    {
     analogWrite(meterP_M3,485);
    }
 // write to second meter
    void write_second()
    {
   analogWrite(meterP_S5, meter_second_A[meter_second]);
 //    analogWrite(meterP_S5,62);
   
    }
   
//delay (600);
//Serial.print(meter_hour, DEC);
//Serial.print(':');
//Serial.print(meter_minute, DEC);
//Serial.print(':');
//Serial.print(meter_second, DEC);
//Serial.println();


