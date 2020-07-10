/*
  Tarea 12
  Mario

  Juan Camilo Vargas Q.
  Sergio Alejandro Vargas Q.

  Tecnologia Digital
  Universidad Nacional de Colombia
  2020-06-01
*/
#include "common.h"

TFT TFTscreen = TFT(CS, DC, RST);
// Images
#include "shells.h"
#include "mario_img.h"
// Mario class
#include "mario.h"

// Mario instance
Mario myMario;

// Block/shell instances
block block_g = {
  COURT_X + COURT_W - BLOCK_SIZE - 1, // posX
  COURT_Y + COURT_H - BLOCK_SIZE - 1, // posY
  COURT_X + COURT_W - BLOCK_SIZE - 1, // oldX
  COURT_Y + COURT_H - BLOCK_SIZE - 1, // oldY
  false
};

block block_r = {
  COURT_X + COURT_W - BLOCK_SIZE - 1,
  COURT_Y + COURT_H - BLOCK_SIZE - 1,
  COURT_X + COURT_W - BLOCK_SIZE - 1,
  COURT_Y + COURT_H - BLOCK_SIZE - 1,
  false
};

bool game_over = false;
int score = 0;
const int strSize = 4;
char score_str[strSize] = {0, 0, 0};

void setup() {
  TFTscreen.begin(INITR_BLACKTAB);
  TFTscreen.background(COLOR_BG);

  // Mario initialization
  myMario.posX = COURT_X + 20;
  myMario.posY = 88;
  myMario.jumpPosY = 60;
  myMario.isJumping = false;
  myMario.state = t0;
  myMario.walkingDelay = 150;

  // Draw court and ground
  TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
  //TFTscreen.drawFastHLine()
  TFTscreen.stroke(COLOR_WHITE);
  TFTscreen.text("SCORE: ", 90, 15);
  draw_score();
}

void loop() {
  static unsigned long block_ms = 0;
  if (!game_over) {
    myMario.jumping(2000, 4);
    if (millis() - block_ms >= BLOCK_MILLIS) {
      block_ms = millis();
      move_blocks();
      if (!myMario.isJumping) {
        check_collision(block_g);
        check_collision(block_r);
      }
    }
    myMario.walking();
  }
}

void move_blocks() {
  static block_state bs = s0;
  switch (bs) {
    case s0:
      block_g.posX -= BLOCK_SPEED;
      if (block_g.posX <= 80) {
        bs = s1;
      }
      break;

    case s1:
      block_g.posX -= BLOCK_SPEED;
      block_r.posX -= BLOCK_SPEED;
      if (block_g.posX <= COURT_X) {
        TFTscreen.fillRect(block_g.oldX, block_g.oldY, BLOCK_SIZE, BLOCK_SIZE, COLOR_BG);
        block_g.posX = COURT_X + COURT_W - BLOCK_SIZE - 1;
        score += 1;
        draw_score();
      }
      if (block_r.posX <= COURT_X) {
        TFTscreen.fillRect(block_r.oldX, block_r.oldY, BLOCK_SIZE, BLOCK_SIZE, COLOR_BG);
        block_r.posX = COURT_X + COURT_W - BLOCK_SIZE - 1;
        score += 1;
        draw_score();
      }
      break;

    case s2:
      // GAME OVER
      break;
  }
  // Alternate sprite
  block_g.alt = !block_g.alt;
  block_r.alt = !block_r.alt;

  if (block_g.posX != block_g.oldX) {
    draw_block(block_g);
    block_g.oldX = block_g.posX;
  }
  if (block_r.posX != block_r.oldX) {
    draw_block(block_r);
    block_r.oldX = block_r.posX;
  }
}

void check_collision(block b) {
  if (b.posX < myMario.posX + myMario.w && myMario.posX < b.posX + BLOCK_SIZE) {
    game_over = true;
    endgame();
  }
}

void endgame() {
  TFTscreen.stroke(COLOR_RED);
  TFTscreen.setTextSize(2);
  TFTscreen.text("GAME OVER", 28, 40);
}

void draw_block(block b) {
  static word *pntr = shell1;
  pntr = b.alt ? shell1 : shell2;
  // Erase difference in images
  TFTscreen.fillRect(b.posX + BLOCK_SIZE, b.oldY, b.oldX - b.posX, BLOCK_SIZE, COLOR_BG);
  // Draw new image
  for (int row = 0; row < BLOCK_SIZE; row++) {
    for (int col = 0; col < BLOCK_SIZE; col++) {
      word p = pgm_read_word(pntr + row * BLOCK_SIZE + col);
      if (p == COLOR_FILTER) p = COLOR_BG;
      TFTscreen.drawPixel(b.posX + col, b.posY + row, p);
    }
  }
}

void draw_score() {
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(COLOR_BG);
  TFTscreen.text(score_str, 135, 15); // Must erase previous score
  int_to_str(score_str, score);
  TFTscreen.stroke(COLOR_WHITE);
  TFTscreen.text(score_str, 135, 15);
}

void int_to_str(char arr[], int val) {
  static int num = 0;
  for (int i = strSize - 2; i >= 0; i--) {
    num = val % 10;
    arr[i] = '0' + num;
    val /= 10;
  }
  for (int i = 0; i < strSize - 2; i++) {
    if (arr[i] == '0') {
      arr[i] = ' ';
    }
  }
}
