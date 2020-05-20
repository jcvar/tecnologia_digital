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
#define BALL_INITY 11
#define BALL_MILLIS_A 10
#define BALL_MILLIS_B 25
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
	int posX;
	int posY;
	int oldX;
	int oldY;
	int speedX;
	int speedY;
	unsigned long prevMillis;
};
ball_t ballA, ballB; // ball instances

// Court structure
struct court_t {
	int top;
	int bottom;
	int left;
	int right;
};
court_t court; // court instance

enum state_t {	s0, s1, s2, s3, s4}; // ball states
state_t ballA_state = s0;
state_t ballB_state = s0;

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
	
	// Ball A initialization
	ballA.color = COLOR_YELLOW;
	ballA.width = BALL_SIZE;
	ballA.height = BALL_SIZE;
	ballA.posX = BALL_INITX;
	ballA.posY = BALL_INITY;
	ballA.speedX = 1;
	ballA.speedY  = 1;
	ballA.prevMillis = 0;
	
	// Ball B initialization
	ballB.color = COLOR_CYAN;
	ballB.width = BALL_SIZE;
	ballB.height = BALL_SIZE;
	ballB.posX = BALL_INITX;
	ballB.posY = BALL_INITY;
	ballB.speedX = 1;
	ballB.speedY  = 1;
	ballB.prevMillis = 0;
	
	TFTscreen.fillRect(ballA.posX, ballA.posY, ballA.width, ballA.height, ballA.color);
	TFTscreen.fillRect(ballB.posX, ballB.posY, ballB.width, ballB.height, ballB.color);
	delay(1000);
}

void loop() {
	//TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	if(millis() > ballA.prevMillis + BALL_MILLIS_A){
		ballA.prevMillis = millis();
		moveBallA();
	}
	if(millis() > ballB.prevMillis + BALL_MILLIS_B){
		ballB.prevMillis = millis();
		moveBallB();
	}
}

void drawBall(ball_t *ball) {
	TFTscreen.fillRect(ball->oldX, ball->oldY, ball->width, ball->height, COLOR_BLACK);
	TFTscreen.fillRect(ball->posX, ball->posY, ball->width, ball->height, ball->color);
}

void moveBallA() {
	ballA.oldX = ballA.posX;
	ballA.oldY = ballA.posY;
	
	switch(ballA_state) {
		case s0: // The ball moves horizontally across the top of the court
			ballA.posX += ballA.speedX;
			if(ballA.posX > (court.right - ballA.width)/3 - 2){
				ballA_state = s1;
			} else if (ballA.posX < (court.left) + 2) {
				ballA.speedX *= -1;
				ballA.speedY *= -1;
			}
			break;
			
		case s1: // The ball moves vertically across the court
			ballA.posY += ballA.speedY;
			if(ballA.posY > (court.bottom - ballA.height) - 2){
				ballA_state = s2;
			} else if(ballA.posY < (court.top) + 2) {
				ballA_state = s0;
			}
			break;
			
		case s2: // The ball moves horizontally across the second third of the court
			ballA.posX += ballA.speedX;
			if(ballA.posX > 2*(court.right-ballA.width)/3 - 2) {
				ballA_state = s3;
			} else if (ballA.posX < (court.right-ballA.width)/3 - 2) {
				ballA_state = s1;
			}
			break;
			
		case s3: // The ball moves vertically across the court
			ballA.posY -= ballA.speedY;
			if(ballA.posY < (court.top)+2) {
				ballA_state = s4;
			}else if(ballA.posY > (court.bottom-ballA.height)-2) {
				ballA_state = s2;
			}
			break;
			
		case s4: // The ball moves horizontally across the last third of the court
			ballA.posX += ballA.speedX;
			if(ballA.posX > (court.right-ballA.width) - 2){
				ballA.speedX *= -1;
				ballA.speedY *= -1;
			}else if (ballA.posX <  2*(court.right-ballA.width)/3 - 2){
				ballA_state = s3;
			}
			break;
	}
	if (ballA.oldX != ballA.posX || ballA.oldY != ballA.posY) {
		drawBall(&ballA);
	}
}


void moveBallB(){
	ballB.oldX = ballB.posX;
	ballB.oldY = ballB.posY;
	static int count = 0;
	
	switch(ballB_state) {
		case s0:
			ballB.posX += ballB.speedX;
			if(ballB.posX == (court.right)/2 - 2) {
				//count = 0;
				ballB_state = s1;
			} else if (ballB.posX == (court.right) - ballB.width - 1) {
				ballB.speedX = 0;
			}
			break;
		case s1:
			ballB.posY += ballB.speedY;
			if(ballB.posY > (court.bottom - ballB.height) - 2){
				count++;
				ballB.speedY *= -1;
				if(count == 3) {
					ballB_state = s0;
				}
			} else  if(ballB.posY < (court.top) + 2){
				ballB.speedY *= -1;
			}
			break;
	}
	if (ballB.oldX != ballB.posX || ballB.oldY != ballB.posY) {
		drawBall(&ballB);
	}
}
