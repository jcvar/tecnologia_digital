// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>

#include <JC_Button.h>

Button selectButton(4);
Button enterButton(5);

#include <TimerOne.h>

// pin definition for Arduino UNO
#define cs   9
#define dc   10   
#define rst  8

// create an instance of the class TFT
TFT TFTscreen = TFT(cs, dc, rst);

// Some ready-made 16-bit ('565') color settings:
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0x001F
#define COLOR_CYAN 0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW 0xFFE0
#define COLOR_ORANGE 0xFC00

// court parameters
int courtX=5,courtY=10,courtW=150,courtH=108;
int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;

#include "Clock.h"
Clock myClock;


//============================ SETUP  ==============================

void setup() {
  // put your setup code here, to run once:
  TFTscreen.begin(INITR_BLACKTAB);     //inicializar TFT
  Serial.begin(9600);    //inicializar puerto serial

  //configurar interrupcion cada medio segundo
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  //inicializar botones
  enterButton.begin();
  selectButton.begin();

  //inicializar datos publicos de myClock
  myClock.updateMinute=true;
  myClock.updateHour=true;
  myClock.halfsecond=0;
  myClock.second=0;
  myClock.minute=0;
  myClock.hour=12;
  myClock.settingHourFlag=false;
  myClock.settingMinuteFlag=false;

  //draw court
  TFTscreen.drawRect(courtX, courtY, courtW,courtH,COLOR_BLUE);
  word c=TFTscreen.Color565(0,0,0);
  TFTscreen.fillRect(courtX+2, courtY+2, courtW-4,courtH-4,c);
}

//============================ LOOP  ==============================
void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long previousMillis_SetTime=0;
  static bool flagSelect=false;
  static bool flagEnter=false;

  enterButton.read();
  selectButton.read();

  if(selectButton.wasPressed())
  {
    flagSelect=true;
  }

  if(enterButton.wasPressed())
  {
    flagEnter=true;
  }
  
  if (millis()-previousMillis_SetTime>200)
  { 
    previousMillis_SetTime=millis();
    myClock.setTime(flagSelect,flagEnter);
  } 
  myClock.displayTime();
  delay(1); 
}

//==========================  TIMING_ISR  ===============================

void TimingISR()
{
  myClock.halfsecond ++;
  myClock.updateHalfSecond= true;
  if(myClock.halfsecond == 2){
    myClock.second ++;
    if(myClock.second == 60)
    {
      myClock.minute ++;
      myClock.updateMinute= true;
      if(myClock.minute == 60)
      {
        myClock.hour ++;
        myClock.updateHour = true;
        if(myClock.hour == 24)myClock.hour = 0;
        myClock.minute = 0;
      }
      myClock.second = 0;
    }
    myClock.halfsecond = 0;
  }
}
