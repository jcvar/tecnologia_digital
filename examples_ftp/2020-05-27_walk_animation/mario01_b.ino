/*
	draw image img1 96x80
*/

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include "mario_img.h"

#include <pgmspace.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

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

//--------------------------------------- CLASS MARIO --------------------------
enum state_t {	t0,t1};
class Mario {
	public:
	state_t state;
	int posX;
	int posY;
	int jumpPosY;
	unsigned int walkingDelay;
	bool isJumping;
	
	void walking()
	{
		static int index=0;
		static unsigned int previousMillis=0,currentMillis;
		currentMillis=millis();
		if((currentMillis-previousMillis) > walkingDelay  && isJumping==false)
		{
			previousMillis=currentMillis; 
			//image reading:
			int w = 16, h = 28, row, col, buffidx=0;  //w:width , h:height
			for (row=0; row<h; row++) 
			{
				for (col=0; col<w; col++) 
				{
					//word p;
					switch(index)
					{
						case 0:
							word p=pgm_read_word(m0 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;
						case 1:
							word p=pgm_read_word(m1 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;  
						case 2:
							word p=pgm_read_word(m2 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;      
						case 3:
							word p=pgm_read_word(m3 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;
					}   
					buffidx++;
				}
			} 
			
			if (index<3)
			{
				++index;
			}
			else
			{
				index=0;
			} 
		}   
	}
	
	void jumping(unsigned int jumpTime, int jumpButtonPin)
	{
		static unsigned long previousMillis=0;
		static unsigned long previousMillisA=0;
		unsigned long currentMillis;
		currentMillis=millis();   
		if((currentMillis-previousMillis) > walkingDelay)
		{ 
			previousMillis=currentMillis;
			switch(state)
			{   
				case t0:
					if (digitalRead(jumpButtonPin)==HIGH)
					{
						isJumping=true;
						
						//walking image erase:
						int w = 16, h = 28, row, col, buffidx=0;  //w:width , h:height
						for (row=0; row<h; row++) 
						{
							for (col=0; col<w; col++) 
							{
								TFTscreen.drawPixel(col+posX,row+posY,COLOR_BLACK);
								buffidx++;
							}
						}
						
						//jump image reading:
						w = 16; h = 28;buffidx=0;  //w:width , h:height
						for (row=0; row<h; row++) 
						{
							for (col=0; col<w; col++) 
							{
								word p=pgm_read_word(m1 + buffidx);
								TFTscreen.drawPixel(col+posX,row+jumpPosY,p);
								buffidx++;
							}
						}
						previousMillisA=millis();
						state=t1;   
					}
					
					break;
				case t1:
					currentMillis=millis(); 
					if((currentMillis-previousMillisA) > jumpTime)
					{
						//jump image erase:
						int w = 16, h = 28, row, col, buffidx=0;  //w:width , h:height
						for (row=0; row<h; row++) 
						{
							for (col=0; col<w; col++) 
							{
								TFTscreen.drawPixel(col+posX,row+jumpPosY,COLOR_BLACK);
								buffidx++;
							}
						}     
						isJumping=false;      
						state=t0;
					}
					break;
			} 
			
		}
	}
};

//-------------------------------------------------------------------------------------------

Mario myMario;

//========================= SETUP ==========================

void setup() {
	// initialize the display
	TFTscreen.begin();
	// white background
	TFTscreen.background(0, 0, 0);
	
	// draw court
	TFTscreen.drawRect(courtX, courtY, courtW,courtH,0xF800);
	
	// court limits
	courtTopLine=courtY;
	courtBottomLine=courtY+courtH;
	courtLeftLine=courtY;
	courtRightLine=courtX+courtW;
	
	myMario.posX=72;
	myMario.posY=50;
	myMario.jumpPosY=21;
	myMario.isJumping=false;
	myMario.state=t0;
	myMario.walkingDelay=150;
	
}

//========================= LOOP ==========================
void loop() {
	myMario.walking();
	myMario.jumping(2000,4);		
}
//=========================================================





