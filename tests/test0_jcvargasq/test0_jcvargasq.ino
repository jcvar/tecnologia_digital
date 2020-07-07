/*
Test 0

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
#define BALL_MILLIS 10

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
	unsigned long prevMillis;
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
state_t ball_state = s0;

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
	ball.color = COLOR_GREEN;
	ball.width = BALL_SIZE;
	ball.height = BALL_SIZE;
	ball.posX = BALL_INITX;
	ball.posY = BALL_INITY;
	ball.speedX = 1;
	ball.speedY  = -1;
	ball.prevMillis = 0;
	
	TFTscreen.fillRect(ball.posX, ball.posY, ball.width, ball.height, ball.color);
	delay(1000);
}

void loop() {
	//TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	if(millis() > ball.prevMillis + BALL_MILLIS){
		ball.prevMillis = millis();
		moveBall();
	}
	
}

void drawBall(ball_t *ball) {
	TFTscreen.fillRect(ball->oldX, ball->oldY, ball->width, ball->height, COLOR_BLACK);
	TFTscreen.fillRect(ball->posX, ball->posY, ball->width, ball->height, ball->color);
}

float slope=(54.0/75.0);
void moveBall() {
	ball.oldX = ball.posX;
	ball.oldY = ball.posY;
	
	switch(ball_state) {
		case s0: // The ball moves horizontally GREEN
			ball.posX += ball.speedX;
			if(ball.posX > (court.right - ball.width) - 2){
				ball.color = COLOR_YELLOW;
				ball.speedX *= -1;
				ball_state = s1;
			}
			break;
			
		case s1: // The ball moves diagonally YELLOW
			ball.posY += slope*ball.speedY;
			ball.posX += ball.speedX;
			if(ball.posY < (court.top) + 2) {
				ball.color = COLOR_BLUE;
				ball_state = s2;
			}
			break;
			
		case s2: // The ball moves horizontally BLUE
			ball.posX += ball.speedX;
			if (ball.posX < (court.left) + 2) {
				ball.color = COLOR_CYAN;
				ball.speedX *= -1;
				ball.speedY *= -1;
				ball_state = s3;
			}
			break;
			
		case s3: // The ball moves diagonally CYAN
			ball.posY += slope*ball.speedY;
			ball.posX += ball.speedX;
			if(ball.posY > (court.bottom-ball.height)-2) {
				ball.color = COLOR_MAGENTA;
				ball.speedX *= -1;
				ball_state = s4;
			}
			break;
			
		case s4: // The ball moves horizontally MAGENTA
			ball.posX += ball.speedX;
			if (ball.posX <  (court.left) + 2){
				ball.color = COLOR_WHITE;
				ball.speedY *= -1;
				ball_state = s5;
			}
			break;
			
		case s5: // The ball moves vertically WHITE
			ball.posY += ball.speedY;
			if (ball.posY <  (128)/2 - 2){
				ball.color = COLOR_GREEN;
				ball.speedX *= -1;
				ball_state = s0;
			}
			break;
	}
	if (ball.oldX != ball.posX || ball.oldY != ball.posY) {
		drawBall(&ball);
	}
}
