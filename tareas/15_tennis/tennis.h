// EXTERNAL LIBRARIES
#include <SPI.h>
#include <TFT.h>
#include <avr/pgmspace.h>

// TFT DEFINITIONS
#define CS  10
#define DC   9
#define RST  8
// PIN DEFINITIONS
#define PIN_LEFT A0
#define PIN_RIGHT A1
#define PIN_START 4

// 16-bit ('565') colors
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0x001F
#define COLOR_CYAN 0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW 0xFFE0
#define COLOR_ORANGE 0xFC00
// Court color palette
#define COLOR_LINE 0xEFBF
#define COLOR_COURT_LIGHT 0x1C7A
#define COLOR_COURT_DARK 0x33F7

// Game definitions
#define BALL_W 5
#define BALL_H 5
#define PADDLE_W 5
#define PADDLE_H 20

// Game logic definitions
#define MILLIS_BALL 50
#define MILLIS_PADDLE 50
#define MAX_SCORE 5 // MAX: 9

// Court parameters (Tennis court dimensions 1ft = 2px)
#define COURT_X 2
#define COURT_Y 28
#define COURT_W 156
#define COURT_H 72
#define CENTER_X 80
#define CENTER_Y 64
#define SINGLES_UY 37
#define SINGLES_DY 91
#define SINGLES_H 54
#define SERVICE_LX 38
#define SERVICE_RX 122
#define SERVICE_W 84

// Court structure
typedef struct {
  int top;
  int bottom;
  int left;
  int right;
} court_t;

/*
  PADDLE STUFF
*/
typedef struct {
  int posX;
  int posY;
  int oldY;
} paddle_t;  // struct paddle_t

typedef struct {
  int posX;
  int posY;
  int oldX;
  int oldY;
  int speedX;
  int speedY;
} ball_t; // struct ball_t


void move_paddle(paddle_t *p, int t_posY) {
  p.oldY = posY;
  p.posY = t_posY;
}  // move_paddle()

void move_ball(ball_t *b, court_t *crt) {
  b.oldX = b.posX;
  b.oldY = b.posY;
  if (b.posY <= crt.top || b.posY >= crt.bottom - BALL_H) {
    b.speedY = -b.speedY;
  }
  b.posX += b.speedX;
  b.posY += b.speedY;
} // move_paddle()
