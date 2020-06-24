// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>

#include <TimerOne.h>

// pin definition for Arduino UNO
#define cs   10
#define dc   9   
#define rst  8

// create an instance of the class TFT
TFT TFTscreen = TFT(cs, dc, rst);

volatile bool updateMinute=true;
volatile bool updateHour=true;
volatile unsigned int halfsecond = 0;
volatile unsigned int second=0;
volatile unsigned int minute = 26;
volatile unsigned int hour = 15;

char bufH[3];
char bufM[3]; 

void setup() {
  // put your setup code here, to run once:
  TFTscreen.begin();

  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  
  itoa(hour,bufH,10);
  itoa(minute,bufM,10);
}

bool settingHourFlag=false;
bool settingMinuteFlag=false;

void loop() {
  // put your main code here, to run repeatedly:
  if(updateMinute || updateHour)
  {

    TFTscreen.setTextSize(2);
    
    if(!settingHourFlag)
    {
      if(updateHour){
        updateHour=false;
        TFTscreen.stroke(0,0,0);          //borrar
        TFTscreen.text(bufH, 52, 55);
        itoa(hour,bufH,10); 
        if(hour<10)                       //no mostrar 0 en decenas
        {
          bufH[1]=bufH[0];
          bufH[0]=' ';          
        }
        TFTscreen.stroke(255, 255, 255);  //mostrar
        TFTscreen.text(bufH, 52, 55);
      }
      
    }
    if(!settingMinuteFlag)  
    {
      if(updateMinute){
        updateMinute=false;
        TFTscreen.stroke(0,0,0);          //borrar
        TFTscreen.text(bufM, 89, 55);
        itoa(minute,bufM,10);
        if(minute<10)                     //mostrar 0 en decenas
        {
          bufM[1]=bufM[0];
          bufM[0]='0';
        }
        TFTscreen.stroke(255, 255, 255);  //mostrar
        TFTscreen.text(bufM, 89, 55);
      } 
    }
  } 
  delay(1); 
}

void TimingISR()
{
  halfsecond ++;
  if(halfsecond == 2){
    second ++;
    if(second == 60)
    {
      minute ++;
      updateMinute=true;
      if(minute == 60)
      {
        hour ++;
        updateHour=true;
        if(hour == 24)hour = 0;
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;
  }
}
