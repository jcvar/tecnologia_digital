#include "../common.h"

TFT TFTscreen = TFT(CS, DC, RST);

// Ball parameters
#define BALL_SIZE 2
#define BALL_INITX 5
#define BALL_INITY 64
#define BALL_MILLIS 50

// OTHER PARAMETERS
#define SINE_AMP 40
#define SINE_FRE (4.0*PI)/(150.0)

// Ball structure
struct ball_t {
	word color;
	int rad;
	int posX;
	int posY;
	int oldX;
	int oldY;
	int speedX;
	int speedY;
	unsigned long prevMillis;
};
ball_t ball; // ball instance

court_t court;// court instance

void setup() {
	TFTscreen.begin(); // initialize the display
	TFTscreen.background(0, 0, 0); // black background

	// Court initialization
	court.top = COURT_Y;
	court.bottom = COURT_Y + COURT_H;
	court.left = COURT_X;
	court.right = COURT_X + COURT_W;
	
	// Ball initialization
	ball.color = COLOR_GREEN;
	ball.rad = BALL_SIZE;
	ball.posX = BALL_INITX;
	ball.posY = BALL_INITY;
	ball.speedX = 1;
	ball.speedY  = 1;
	ball.prevMillis = 0;
	
	drawPlot(COLOR_GREEN);
	TFTscreen.fillCircle(ball.posX, ball.posY, ball.rad, ball.color);
	drawCourt();
	delay(500);
}

void loop() {
	if(millis() > ball.prevMillis + BALL_MILLIS){
		ball.prevMillis = millis();
		drawPlot(COLOR_GREEN);
		moveBall();
		drawCourt();
	}
}

float x_to_y(int x){
	return 64 - SINE_AMP * sin(SINE_FRE * (x-5));
}

void drawPlot(word c){
	TFTscreen.drawFastHLine(COURT_X, 64, COURT_W, c);
	for(int i = court.left; i < court.right; i++){
		TFTscreen.drawPixel(i, x_to_y(i), c);
	}
}

void drawCourt(){
	TFTscreen.fillRect(0, 50, 5, 28, COLOR_BLACK);
	TFTscreen.fillRect(155, 50, 5, 28, COLOR_BLACK);
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}

void drawBall(ball_t *ball) {
	TFTscreen.fillCircle(ball->oldX, ball->oldY, ball->rad, COLOR_BLACK);
	TFTscreen.fillCircle(ball->posX, ball->posY, ball->rad, ball->color);
}

void moveBall() {
	ball.oldX = ball.posX;
	ball.oldY = ball.posY;
	
	ball.posX += ball.speedX;
	if(ball.posX > (court.right)){
		ball.posX = BALL_INITX;
	}
	ball.posY = x_to_y(ball.posX);
	
	if (ball.oldX != ball.posX || ball.oldY != ball.posY) {
		drawBall(&ball);
	}
}

