/*
Sergio Alejandro Vargas Quintero
C.C. 1026599210

Tecnología Digital - Grupo 1
Facultad de Ingeniería
Universidad Nacional de Colombia

Parcial 1
2020-05-13

1. Realizar las tres animaciones simultaneas mostradas en el video.
Ani1: círculos de radio 5 pixeles. Actualización cada 800 ms
Ani2: ancho de la barra: 10 pixeles. La longitud se incrementa en 5 pixeles cada 350 ms
Ani3: bola cuadrada de 5 x 5 pixeles, incrementos de 1 pixel en ambas direcciones, cada 30 ms.
*/

#include <TFT.h>
#include <SPI.h>
// Screen definitions and initialization
#define cs   10
#define dc   9
#define rst  8
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
// 227 400


// Court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108
// Court structure
struct court_t {
	int top;
	int bottom;
	int left;
	int right;
};
court_t court;// court instance


// ani 1
long ani1PrevMillis = 0;
long ani1Duration = 800;
enum ani_state_t {	y0, y1, y2, y3, y4};
static int size1 = 5;

//ani 2
long ani2PrevMillis = 0;
long ani2Duration = 350;
static int barH = 10;
static int barW = 0;


// ani 3
long ani3PrevMillis = 0;
long ani3Duration = 30;
// ball parameters
int initBallX=5,initBallY=96;
int ballX=initBallX, ballY=initBallY, oldBallX, oldBallY;
int ballW=5,ballH=5;
// ball speed
int ballDirectionX = 1;
int ballDirectionY = 1;
// ball move state
enum state_t {	sx, sy};
state_t ballState=sx;


void setup() {
	TFTscreen.begin(); // initialize the display
	TFTscreen.background(0, 0, 0); // black background
	
	// Court initialization
	court.top = COURT_Y;
	court.bottom = COURT_Y + COURT_H;
	court.left = COURT_X;
	court.right = COURT_X + COURT_W;
	drawCourt();
	
	delay(1000);
}

void loop() {
	if (millis() > ani1PrevMillis + ani1Duration){
		ani1PrevMillis = millis();
		ani1();
	}
	if (millis() > ani2PrevMillis + ani2Duration){
		ani2PrevMillis = millis();
		ani2();
		drawCourt();
	}
	if(millis() > ani3PrevMillis + ani3Duration){
		ani3PrevMillis = millis();
		ani3();
	}
	
}


void drawCourt(){
	TFTscreen.fillRect(0, 50, 5, 28, COLOR_BLACK);
	TFTscreen.fillRect(155, 50, 5, 28, COLOR_BLACK);
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}

void ani1() {
	static int posX = 60, posY = 20;
	static ani_state_t ani1State = y0;
	switch (ani1State) {
		case y0:
			TFTscreen.fillCircle(posX, posY, size1, COLOR_BLUE);
			TFTscreen.fillCircle(posX+10, posY, size1, COLOR_BLUE);
			TFTscreen.fillCircle(posX, posY+10, size1, COLOR_BLUE);
			TFTscreen.fillCircle(posX+10, posY+10, size1, COLOR_BLUE);
			ani1State = y1;
			break;
		case y1:
			TFTscreen.fillCircle(posX, posY, size1, COLOR_YELLOW);
			ani1State = y2;
			break;
		case y2:
			TFTscreen.fillCircle(posX, posY+10, size1, COLOR_YELLOW);
			ani1State = y3;
			break;
		case y3:
			TFTscreen.fillCircle(posX+10, posY+10, size1, COLOR_YELLOW);
			ani1State = y4;
			break;
		case y4:
			TFTscreen.fillCircle(posX+10, posY, size1, COLOR_YELLOW);
			ani1State = y0;
			break;
	}
}


void ani2(){
	if (barW >= 155) {
		TFTscreen.fillRect(5, 64, 150, barH, COLOR_BLACK);
		barW = 0;
	}
	TFTscreen.fillRect(5, 64, barW+=5, barH, COLOR_GREEN);
}


int count=0;
void ani3(){
	switch(ballState){
		case sx:
			ballX += ballDirectionX;
			if(ballX == (150)/2 - 2){
				ballState=sy;
			} else if (ballX == (160)-ballW-1){
				count = 0;
				ballX = 5;
				ballY = 96;
			}
			break;
		case sy:
			ballY += ballDirectionY;
			if(ballY > (court.bottom-ballH)-2){
				count++;
				ballDirectionY*=-1;
				if(count==3){
					ballState = sx;
				}
			} else  if(ballY < 96 - ballH){
				ballDirectionY*=-1;
			}
			break;
	}
	
	// erase the ball's previous position
	TFTscreen.fill(0, 0, 0);
	
	if (oldBallX != ballX || oldBallY != ballY) {
		TFTscreen.rect(oldBallX, oldBallY, ballW, ballH);
	}
	
	// draw the ball's current position
	TFTscreen.fill(255, 255, 0);
	TFTscreen.rect(ballX, ballY, ballW, ballH);
	
	oldBallX = ballX;
	oldBallY = ballY;
}

