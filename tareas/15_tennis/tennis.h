// EXTERNAL LIBRARIES
#include <SPI.h>
#include <TFT.h>
#include <avr/pgmspace.h>

#include "tennis_images.h"

// TFT DEFINITIONS
#define CS 9
#define DC 10
#define RST 8
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
#define BALL_SIZE 5
#define PADDLE_W 5
#define PADDLE_H 20

// Game logic definitions
#define MILLIS_BALL 50
#define MILLIS_PADDLE 50
#define MILLIS_GAME 50
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

// SCORE POSITIONS
#define SCORE_LX 40
#define SCORE_RX 144

// Court structure
typedef struct {
  int top;
  int bottom;
  int left;
  int right;
} court_t;

// Paddle structure
typedef struct {
  int posX;
  int posY;
  int oldY;
} paddle_t;

// Ball structure
typedef struct {
  int posX;
  int posY;
  int oldX;
  int oldY;
  int speedX;
  int speedY;
} ball_t;

typedef enum {menu, start, game_over} state_t;

typedef enum {none, left, right} player_side_t;

typedef struct {
  int score;
  player_side_t side;
  paddle_t paddle;
} player_t;

int check_collision(ball_t, paddle_t);
int check_ranges(int dy);
bool update_score(int result);
void reset_score();
int check_goal(court_t crt);
int check_collision(ball_t b, paddle_t p);

/* MOVE FUNCTIONS */

void move_ball(ball_t *, court_t *);
void move_paddle(paddle_t *, int);

/* DRAW FUNCTIONS */

void draw_score();
//void draw_ball(ball_t *b, word bg);
//void draw_paddle(paddle_t *p, word bg);
//void draw_inner_lines(word cor);
//void draw_outer_lines(word cor);

