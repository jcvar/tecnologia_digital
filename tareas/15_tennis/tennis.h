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
#define BALL_SIZE 5
#define PADDLE_W 5
#define PADDLE_H 20

// Game logic definitions
#define MILLIS_GAME 50
#define MAX_SCORE 5 // MAX: 9

// Court parameters (Tennis court dimensions 1ft = 2px)
#define COURT_W 156 // 39 * 4
#define COURT_H 72 // 36 * 2
#define COURT_X 2 // (160-156)/2
#define COURT_Y 28 // (128-72)/2

#define CENTER_X 80
#define CENTER_Y 64

#define SINGLES_H 54 // 27 * 2
#define SINGLES_UY 37 // COURT_X + 9
#define SINGLES_DY 91 // COURT_UY + SINGLES_H
#define SERVICE_W 84 // 21 * 4
#define SERVICE_LX 38 // (39-21)*2 + 2
#define SERVICE_RX 122 // SERVICE_LX + SERVICE_W

// SCORE POSITIONS
#define SCORE_LX 4
#define SCORE_RX 136

typedef enum {	menu, new_game, serve_game, play_game, game_over} state_t;

typedef enum {  s0, s15, s30, s40, sGame, sNoAdv, sDeuce, sAdv} score_t;

typedef enum {	no_goal, left, right} side_t;

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

typedef struct {
  int score;
  side_t side;
  paddle_t paddle;
} player_t;

// Game logic
void tennis_match();
side_t check_goal(ball_t *b, court_t *crt);

// Check collisions
int get_new_speed(int dy);
void check_collision_paddle(ball_t *b, paddle_t *p);
void check_collision_court(ball_t *b, court_t *crt);

// Move functions
void move_ball(ball_t *b);
void move_paddle(paddle_t *p, int new_posY);

// Draw functions
void draw_score(player_t *player);
void draw_ball(ball_t *b, word bg);
void draw_paddle(paddle_t *p, word bg);
void draw_lines(word cor);
