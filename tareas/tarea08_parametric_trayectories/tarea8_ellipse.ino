#include "../common.h"

TFT TFTscreen = TFT(CS, DC, RST);

// Ball parameters
#define BALL_SIZE 2
#define BALL_INITX 130
#define BALL_INITY 64
#define BALL_MILLIS 50

// Trayectory parameters
#define BALL_INIT_T 0
#define T_STEP 0.01

//Ellipse parametes
#define MAYOR_AXIS 50
#define MINOR_AXIS 30

// Ball structure
struct ball_t {
	word color;
	int rad;
	float posX;
	float posY;
	float oldX;
	float oldY;
	int speedX;
	int speedY;
	unsigned long prevMillis;
	float oldT;
	float t;
};

ball_t ball; // ball instance


void setup() {
	TFTscreen.begin(); // initialize the display
	TFTscreen.background(0, 0, 0); // black background

	// Ball initialization
	ball.color = COLOR_CYAN;
	ball.rad = BALL_SIZE;
	ball.posX = BALL_INITX;
	ball.posY = BALL_INITY;
	ball.speedX = 1;
	ball.speedY  = 1;
	ball.prevMillis = 0;
	ball.t = BALL_INIT_T;
	
	drawPlot();
	TFTscreen.fillCircle(ball.posX, ball.posY, ball.rad, ball.color);
	drawCourt();
	delay(500);
}

void loop() {
	if(millis() > ball.prevMillis + BALL_MILLIS){
		ball.prevMillis = millis();
		drawPlot();
		moveBall();
		drawCourt();
	}
}

void drawCourt(){
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}

void drawBall(ball_t *ball) {
	TFTscreen.fillCircle(ball->oldX, ball->oldY, ball->rad, COLOR_BLACK);
	TFTscreen.fillCircle(ball->posX, ball->posY, ball->rad, ball->color);
}

float calc_x(float t){
	return (MAYOR_AXIS * cos(t)) + 80;
}

float calc_y(float t){
	return (MINOR_AXIS * sin(t)) + 64;
}

void drawPlot(){
	TFTscreen.drawFastHLine(COURT_X, 64, COURT_W, COLOR_WHITE);
	TFTscreen.drawFastVLine(80, COURT_Y, COURT_H, COLOR_WHITE);
	for(float i = 0; i < TWO_PI; i+=T_STEP){
		TFTscreen.drawPixel(calc_x(i), calc_y(i), COLOR_BLUE);
	}
}

void moveBall() {
	ball.oldX = ball.posX;
	ball.oldY = ball.posY;
	
	ball.t += T_STEP*10;
	ball.posX = calc_x(ball.t);
	ball.posY = calc_y(ball.t);
	if(ball.t > TWO_PI){	// Technically not necessary, but T stays small
		ball.t = BALL_INIT_T;
	}
	
	if (ball.oldX != ball.posX || ball.oldY != ball.posY) {
		drawBall(&ball);
	}
}
