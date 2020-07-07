/*
Test 1

Juan Camilo Vargas Q.
Tecnologia Digital
Universidad Nacional de Colombia
2020-05-04
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
#define COLOR_ORANGE 0xFC00

// Ball parameters
#define BALL_SIZE 5
#define BALL_INITX 6
#define BALL_INITY 62


// TIME PARAMETERS
#define MILLIS_A1 800
#define MILLIS_A2 350
#define MILLIS_A3 30

// Court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108

// Ball structure
struct ball_t {
	word color;
	int width;
	int height;
	float posX;
	float posY;
	float oldX;
	float oldY;
	int speedX;
	int speedY;
};
ball_t ball; // ball instance

// Court structure
struct court_t {
	int top;
	int bottom;
	int left;
	int right;
};
court_t court; // court instance

enum state_t {	s0, s1, s2, s3, s4, s5}; // ball states
state_t square_state = s0;
state_t bar_state = s0;
state_t ball_state = s0;

unsigned long prev_millis_a1 = 0;
unsigned long prev_millis_a2 = 0;
unsigned long prev_millis_a3 = 0;

int cx[4]={75, 75, 85, 85};
int cy[4]={20, 30, 30, 20};


void setup() {
	TFTscreen.begin(); // initialize the display
	TFTscreen.background(0, 0, 0); // black background

	// Court initialization
	court.top = COURT_Y;
	court.bottom = COURT_Y + COURT_H;
	court.left = COURT_X;
	court.right = COURT_X + COURT_W;
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);

	// Ball initialization
	ball.color = COLOR_MAGENTA;
	ball.width = BALL_SIZE;
	ball.height = BALL_SIZE;
	ball.posX = court.left+1;
	ball.posY = 3*(court.bottom-court.top)/4;
	ball.speedX = 1;
	ball.speedY = 1;

	TFTscreen.fillRect(ball.posX, ball.posY, ball.width, ball.height, ball.color);


	for(int i = 0; i<4; i++){
		TFTscreen.fillCircle(cx[i], cy[i], 5, COLOR_BLUE);
	}
	delay(1000);
}

void loop() {
	//TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	if(millis() > prev_millis_a1 + MILLIS_A1){
		prev_millis_a1 = millis();
		A1();
	}
	if(millis() > prev_millis_a2 + MILLIS_A2){
		prev_millis_a2 = millis();
		A2();
	}
	if(millis() > prev_millis_a3 + MILLIS_A3){
		prev_millis_a3 = millis();
		A3();
	}

}

void A1(){
	static int count = 0;
	if(count == 4){
		count = 0;
		for(int i = 0; i<4; i++){
			TFTscreen.fillCircle(cx[i], cy[i], 5, COLOR_BLUE);
		}
	}else{
		TFTscreen.fillCircle(cx[count], cy[count], 5, COLOR_YELLOW);
		count++;
	}
}

void A2(){
	static int x = 6;
	if (x < court.right - 5){
		TFTscreen.fillRect(x, 59, 5, 10, COLOR_GREEN);
		x += 5;
	} else {
		TFTscreen.fillRect(6, 59, 148, 10, COLOR_BLACK);
		x = 6;
	}
}

void A3() {
	static int count = 0;
	ball.oldX = ball.posX;
	ball.oldY = ball.posY;

	switch(ball_state) {
		case s0: // The ball moves horizontally
			ball.posX += ball.speedX;
			if(ball.posX == (160)/2 - 2){
				ball_state=s1;
			} else if (ball.posX == (court.right)-ball.width-1){
				ball.posX = court.left+1;
				ball.posY = 3*(court.bottom-court.top)/4;
			}
			break;

		case s1: // The ball moves vertically
			ball.posY += ball.speedY;
			if(ball.posY > (court.bottom-ball.height)-2){
				count++;
				ball.speedY*=-1;
				if(count==3){
					ball_state = s0;
					count=0;
				}
			} else  if(ball.posY < 3*(court.bottom-court.top)/4){
				ball.speedY*=-1;
			}
			break;
	}
	if (ball.oldX != ball.posX || ball.oldY != ball.posY) {
		drawBall(&ball);
	}
}

void drawBall(ball_t *ball) {
	TFTscreen.fillRect(ball->oldX, ball->oldY, ball->width, ball->height, COLOR_BLACK);
	TFTscreen.fillRect(ball->posX, ball->posY, ball->width, ball->height, ball->color);
}
