// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>

// pin definition for Arduino UNO
#define cs   10
#define dc   9   
#define rst  8

// create an instance of the class TFT
TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  // put your setup code here, to run once:
  //initialize the library
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  //set the text size
  TFTscreen.setTextSize(3);
}

void loop() {
  // put your main code here, to run repeatedly:

  static int num = 0;
  char cstr[2];
  itoa(num, cstr, 10);            //convert integer to ascii char array
  
  TFTscreen.stroke(0, 0, 255);    //print number
  TFTscreen.text(cstr, 70, 57);

  delay(1000);                    //wait one second

  TFTscreen.stroke(0, 0, 0);      //delete number
  TFTscreen.text(cstr, 70, 57);  

  if(num<23) num++;               //update number
  else num=0;  
}
