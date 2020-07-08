/*
  Tarea 15
  Tennis

  Juan Camilo Vargas Q.
  Sergio Alejandro Vargas Q.

  Tecnologia Digital
  Universidad Nacional de Colombia
  2020-06-17
*/
#include "tennis.h"

TFT TFTscreen = TFT(CS, DC, RST);

// TODO: Refactor these booleans into a game_state enum
bool start = false;
bool game_over = true;
bool button = false;

void setup() {
  //TFTscreen.begin(INITR_BLACKTAB);
  TFTscreen.begin();

  // Draw court
  TFTscreen.background(COLOR_COURT_LIGHT);
  TFTscreen.fillRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_COURT_DARK);
  TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_LINE);
  draw_inner_lines(TFTscreen, COLOR_LINE);
  TFTscreen.stroke(COLOR_LINE);
  //TFTscreen.setTextSize(2);
  TFTscreen.text("AUSTRALIAN OPEN", 68, 4);

  //delay(10000);
}

void loop() {
  // court_t {top, bottom, left, right}. 1px smaller for collisions
  static court_t court = {COURT_Y + 1, COURT_Y + COURT_H - 1, COURT_X + 1, COURT_X + COURT_W - 1};
  // ball_t{pX, pY, dX, dY}
  static ball_t ball = {CENTER_X, CENTER_Y, CENTER_X + 1, CENTER_Y + 1, -1, 1};
  // player_t {score, side, paddle_t{pX, pY, oldY} }
  static player_t playerL = {0, left, {court.left, court.top, court.top - 1}}
  static player_t playerR = {0, right, {court.right - PADDLE_W, court.top, court.top - 1}}

  static unsigned long millis_ball = 0;
  static unsigned long millis_left = 0;
  static unsigned long millis_right = 0;

  static unsigned long millis_game = 0;

  if (millis() - millis_left > MILLIS_PADDLE) {
    // Left paddle moves
    millis_left = millis();
    move_paddle(&left, map(analogRead(PIN_LEFT), 1023, 0, court.top, court.bottom - PADDLE_H));
    left.draw(TFTscreen, COLOR_COURT_DARK);
    // Right paddle moves
    millis_right = millis();
    move_paddle(&right, map(analogRead(PIN_RIGHT), 1023, 0, court.top, court.bottom - PADDLE_H));
    draw_paddle(TFTscreen, COLOR_COURT_DARK);
  }

  if (millis() - millis_ball > MILLIS_BALL) {
    millis_ball = millis();

    if (!game_over) {
      // RUN GAME LOGIC
      ball.move(court);
      ball.draw(TFTscreen, COLOR_COURT_DARK);
      game_over = update_score(ball.check_goal(court));

      static int new_speed = 0;
      if (ball.speedX > 0) {
        new_speed = check_collision(ball, right);
      } else if (ball.speedX < 0) {
        new_speed = check_collision(ball, left);
      }
      if (new_speed != 0) {
        ball.speedY += new_speed;
        ball.speedX *= -1;
      }
      //if((new_speed = check_collision(ball, left))) {
      //	ball.speedY = new_speed;
      //}
    } else if (score_left < MAX_SCORE && score_right < MAX_SCORE) {
      // NEW ROUND: RESET BALL
      TFTscreen.fillRect(ball.posX, ball.posY, BALL_W, BALL_H, COLOR_COURT_DARK);
      ball.posX = CENTER_X;
      ball.posY = CENTER_Y;
      ball.speedX *= -1;
      game_over = false;
    } else if (start) {
      // NEW GAME: RESET BALL AND SCOREBOARD
      start = false;
      //TODO: reset_score();
      TFTscreen.fillRect(ball.posX, ball.posY, BALL_W, BALL_H, COLOR_COURT_DARK);
      ball.posX = CENTER_X;
      ball.posY = CENTER_Y;
      ball.speedX *= -1;
      ball.draw(TFTscreen, COLOR_COURT_DARK);
      game_over = false;
    } else if (game_over && digitalRead(PIN_START)) {
      start = true;
      //} else {
      //	button = ;
    }
  }
}


/* get_new_speed: Returns an integer between (-2, 2)
  depending on which part of the paddle the ball's position hits */
int get_new_speed(int dy) {
  if (dy < 2) {
    return -2;
  } else if (dy < 8) {
    return -1;
  } else if (dy < 14) {
    return 1;
  } else { // if (dy < 20) {
    return 2;
  }
}

/* check_collision: If the ball hits a paddle, return the new speed of the ball */
int check_collision(ball_t *b, paddle_t *p) {
  // Check y-axis
  if (b->posY < p->posY + p->h && b->posY + b->h > p->posY) {
    // if ball collides on the left side of the paddle
    if (b->speedX > 0 && b->posX + b->w >= p->posX) {
      return get_new_speed(b->posY - p->posY);
    // if ball collides on the right side of the paddle
    } else if (b->speedX < 0 && b->posX <= p->posX + p->w) {
      return get_new_speed(b->posY - p->posY);
    }
  }
  return 0; // no collision
}

// TODO:
//bool update_score(player_t *player) {
//  player->score += 1;
//}

// check_goal: Returns -1 or 1 for goal on left or right ends, else 0
int check_goal(ball_t *b, court_t *crt) { 
  if (b->posX <= crt->left) {
    return -1;
  } else if (b->posX >= court.right - BALL_SIZE) {
    return 1;
  }
  return 0;
} // check_goal

/* MOVE FUNTIONS */

void move_ball(ball_t *b, court_t *crt) {
  b->oldX = b->posX;
  b->oldY = b->posY;
  if (b->posY <= crt->top || b->posY >= crt->bottom - BALL_SIZE) {
    b->speedY = -b->speedY;
  }
  b->posX += b->speedX;
  b->posY += b->speedY;
} // move_ball

void move_paddle(paddle_t *p, int new_posY) {
  p->oldY = p->posY;
  p->posY = new_posY;
} // move_paddle

/* DRAW FUNCTIONS */

void draw_score(player_t *player) {
  static char *str_left;
  static char *str_right;

  if (player->side == right) {
    // Erase previous score
    TFTscreen.stroke(COLOR_BLACK);
    TFTscreen.text(str_right, SCORE_RX, 4); 
    // Draw new score
    TFTscreen.stroke(COLOR_WHITE);
    str_right = (char)player->score + '0';
    TFTscreen.text(&str_right, SCORE_RX, 4);

  } else if (player->side == left) {
    // Erase previous score
    TFTscreen.stroke(COLOR_BLACK);
    TFTscreen.text(str_left, SCORE_LX, 4);
    // Draw new score
    TFTscreen.stroke(COLOR_WHITE);
    str_left = (char)player->score + '0';
    TFTscreen.text(&str_left, SCORE_LX, 4);
  }
}


void draw_ball(ball_t *b, word bg) {
  if (b->posX != b->oldX || b->posY != b->oldY) {
    // Erase previous position
    TFTscreen.fillRect(b->oldX, b->oldY, BALL_W, BALL_H, bg);
    // Draw new position
    for (int row = 0; row < BALL_SIZE; row++) {
      for (int col = 0; col < BALL_SIZE; col++) {
        word px = pgm_read_word(img_ball + row * BALL_SIZE + col);
        TFTscreen.drawPixel(col + b->posX, row + b->posY, px);
      }
    }
  }
} // draw_ball

void draw_paddle(paddle_t *p, word bg) {
  if (p->oldY != p->posY) {
    // Erase previous position
    TFTscreen.fillRect(p->posX, p->oldY, PADDLE_W, PADDLE_H, bg);
    // Draw new position
    for (int row = 0; row < PADDLE_W; row++) {
      for (int col = 0; col < PADDLE_W; col++) {
        word px = pgm_read_word(img_paddle + row * PADDLE_W + col);
        TFTscreen.drawPixel(col + p->posX, row + p->posY, px);
      }
    }
  }
} // draw_paddle

void draw_inner_lines(word cor) {
  // Singles lines
  TFTscreen.drawFastHLine(COURT_X, SINGLES_UY, COURT_W, cor);
  TFTscreen.drawFastHLine(COURT_X, SINGLES_DY, COURT_W, cor);
  // Service lines
  TFTscreen.drawFastVLine(SERVICE_LX, SINGLES_UY, SINGLES_H, cor);
  TFTscreen.drawFastVLine(SERVICE_RX, SINGLES_UY, SINGLES_H, cor);
  // Center service line
  TFTscreen.drawFastHLine(SERVICE_LX, CENTER_Y, SERVICE_W, cor);
  // Net
  TFTscreen.drawFastVLine(CENTER_X, COURT_Y, COURT_H, cor);
} // draw_inner_lines

void draw_outer_lines(word cor) {
  TFTscreen.drawFastHLine(COURT_X, COURT_Y, COURT_W, cor);
  TFTscreen.drawFastHLine(COURT_X, COURT_Y + COURT_H, COURT_W, cor);
} // draw_outer_lines

