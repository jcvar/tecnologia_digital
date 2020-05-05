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
#define BALL_SIZE 2
#define BALL_INITX 5
#define BALL_INITY 64
#define BALL_MILLIS 50

// Court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108

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
ball_t ball; // ball instances

// Court structure
struct court_t {
	int top;
	int bottom;
	int left;
	int right;
};
court_t court;// court instance

/*
enum state_t {	s0, s1, s2, s3, s4}; // ball states
state_t ball_state = s0;
*/

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

