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
#define BALL_INITX 5
#define BALL_INITY 64
#define BALL_MILLIS_A 10
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
ball_t ballA; // ball instances

// Court structure
struct court_t {
	int top;
	int bottom;
	int left;
	int right;
};
court_t court; // court instance

enum state_t {	s0, s1, s2, s3, s4, s5}; // ball states
state_t ballA_state = s0;

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
	ballA.color = COLOR_GREEN;
	ballA.width = BALL_SIZE;
	ballA.height = BALL_SIZE;
	ballA.posX = BALL_INITX;
	ballA.posY = BALL_INITY;
	ballA.speedX = 1;
	ballA.speedY  = 1;
	ballA.prevMillis = 0;
	
	TFTscreen.fillRect(ballA.posX, ballA.posY, ballA.width, ballA.height, ballA.color);
	delay(1000);
}

void loop() {
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	if(millis() > ballA.prevMillis + BALL_MILLIS_A){
		ballA.prevMillis = millis();
		moveBall();
	}
}

void drawBall(ball_t *ball) {
	TFTscreen.fillRect(ball->oldX, ball->oldY, ball->width, ball->height, COLOR_BLACK);
	TFTscreen.fillRect(ball->posX, ball->posY, ball->width, ball->height, ball->color);
}

void moveBall() {
	ballA.oldX = ballA.posX;
	ballA.oldY = ballA.posY;
	
	switch(ballA_state) {
		case s0: // The ball moves horizontally across the middle
			ballA.color = COLOR_GREEN;
			ballA.posX += ballA.speedX;
			if(ballA.posX > (court.right - ballA.width)) {
				ballA_state = s1;
			}
			break;
			
		case s1: // The ball moves diagonally to the middle of the top
			ballA.color = COLOR_YELLOW;
			ballA.posX -= 1;
			ballA.posY -= 54/75;
			if(ballA.posY < (court.top + ballA.height)) {
				ballA_state = s2;
			}
			break;
			
		case s2: // The ball moves horizontally to the top left corner
			ballA.color = COLOR_BLUE;
			ballA.posX -= ballA.speedX;
			if(ballA.posX < (court.left + ballA.width)) {
				ballA_state = s3;
			}
			break;
			
		case s3: // The ball moves diagonally to the bottom right corner
			ballA.color = COLOR_CYAN;
			ballA.posX += 1;
			ballA.posY += 54/72;
			if(ballA.posY > (court.bottom + ballA.height)) {
				ballA_state = s2;
			}
			break;
		case s4: // The ball moves horizontally across to the bottom left corner
			ballA.color = COLOR_MAGENTA;
			ballA.posX -= ballA.speedX;
			if(ballA.posX < (court.left + ballA.width)) {
				ballA_state = s5;
			}
			break;
			
		case s5: // The ball moves vertically to the middle left
			ballA.color = COLOR_WHITE;
			ballA.posY -= ballA.speedY;
			if(ballA.posY < (court.bottom)/2 + ballA.height) {
				ballA_state = s0;
			}
			break;
	}
	if (ballA.oldX != ballA.posX || ballA.oldY != ballA.posY) {
		drawBall(&ballA);
	}
}

