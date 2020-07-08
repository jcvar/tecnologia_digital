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

// court_t {top, bottom, left, right}. 1px smaller for collisions
court_t court = {COURT_Y + 1, COURT_Y + COURT_H - 1, COURT_X + 1, COURT_X + COURT_W - 1};
// ball_t{pX, pY, dX, dY}
ball_t ball = {CENTER_X, CENTER_Y, CENTER_X + 1, CENTER_Y + 1, -1, 1};
// player_t {score, side, paddle_t{pX, pY, oldY} }
player_t playerL = {0, left, {court.left, court.top, court.top - 1}};
player_t playerR = {0, right, {court.right - PADDLE_W, court.top, court.top - 1}};

void setup() {
  //TFTscreen.begin(INITR_BLACKTAB);
  TFTscreen.begin();

  // Draw court
  TFTscreen.background(COLOR_COURT_LIGHT);
  TFTscreen.fillRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_COURT_DARK);
  TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_LINE);
  draw_lines(COLOR_LINE);
  TFTscreen.stroke(COLOR_LINE);
  //TFTscreen.setTextSize(2);
  TFTscreen.text("AUSTRALIAN OPEN", 68, 4);

  //delay(10000);
}

void loop() {
  static unsigned long prev_millis = 0;
  if (millis() - prev_millis > MILLIS_GAME) {
    prev_millis = millis();

    // Update paddles' positions
    move_paddle(&playerL.paddle, map(analogRead(PIN_LEFT), 1023, 0, court.top, court.bottom - PADDLE_H));
    move_paddle(&playerR.paddle, map(analogRead(PIN_RIGHT), 1023, 0, court.top, court.bottom - PADDLE_H));
    draw_paddle(&playerL.paddle, COLOR_COURT_DARK);
    draw_paddle(&playerL.paddle, COLOR_COURT_DARK);

    tennis_match();
  }
}

// Game logic state machine
void tennis_match() {
  static state_t game_state = new_game;
  static side_t goal_side = none;

  switch (game_state) {
    case new_game:
      // Reset players' scores
      playerL.score = 0;
      playerR.score = 0;
      draw_score(&playerL);
      draw_score(&playerR);
      // TODO: ADD START GAME BUTTON
      game_state = serve_game;
      break;

    case serve_game:
      //move ball to centre
      ball.oldX = ball.posX;
      ball.oldY = ball.posY;
      ball.posX = CENTER_X;
      ball.posY = CENTER_Y;
      draw_ball(&ball, COLOR_COURT_DARK);
      ball.speedX *= -1;
      ball.speedY = ball.speedY > 0 ? -1 : 1;
      game_state = play_game;
      break;

    case play_game:
      move_ball(&ball);
      draw_ball(&ball, COLOR_COURT_DARK);
      goal_side = check_goal(&ball, &court);
      if (goal_side == none) {
        check_collision_paddle(&ball, &playerR.paddle);
        check_collision_paddle(&ball, &playerL.paddle);
        check_collision_court(&ball, &court);
      } else {
        if (goal_side == right) {
          playerL.score += 1;
          draw_score(&playerL);
        } else if (goal_side == left) {
          playerR.score += 1;
          draw_score(&playerR);
        }
        if (playerL.score < MAX_SCORE && playerR.score < MAX_SCORE) {
          game_state = serve_game;
        } else {
          game_state = game_over;
        }
      }
      break;

    case game_over:
      // draw_win(goal_side);
      // TODO: button
      // game_state = new_game;
      break;
  }
}

side_t check_goal(ball_t *b, court_t *crt) {
  if (b->posX <= crt->left) {
    return left;
  } else if (b->posX >= court.right - BALL_SIZE) {
    return right;
  }
  return none;
} // check_goal

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

/* check_collision: If the ball hits a paddle, bounce */
void check_collision_paddle(ball_t *b, paddle_t *p) {
  // Check y-axis
  if (b->posY < p->posY + PADDLE_H && b->posY + BALL_SIZE > p->posY) {
    // if ball collides on the left side of the paddle
    if (b->speedX > 0 && b->posX + BALL_SIZE >= p->posX) {
      b->speedY += get_new_speed(b->posY - p->posY);
      b->speedX *= -1;
      // if ball collides on the right side of the paddle
    } else if (b->speedX < 0 && b->posX <= p->posX + PADDLE_W) {
      b->speedY += get_new_speed(b->posY - p->posY);
      b->speedX *= -1;
    }
  }
}

void check_collision_court(ball_t *b, court_t *crt) {
  // Check collision against the court
  if (b->posY <= crt->top || b->posY >= crt->bottom - BALL_SIZE) {
    b->speedY = -b->speedY;
  }
}

/* MOVE FUNTIONS */

void move_ball(ball_t *b) {
  b->oldX = b->posX;
  b->oldY = b->posY;
  b->posX += b->speedX;
  b->posY += b->speedY;
} // move_ball

void move_paddle(paddle_t *p, int new_posY) {
  p->oldY = p->posY;
  p->posY = new_posY;
} // move_paddle

/* DRAW FUNCTIONS */

void draw_score(player_t *player) {
  static char str_left;
  static char str_right;

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

// draw_ball: Also calls draw_lines
void draw_ball(ball_t *b, word bg) {
  if (b->posX != b->oldX || b->posY != b->oldY) {
    // Erase previous position
    TFTscreen.fillRect(b->oldX, b->oldY, BALL_W, BALL_H, bg);
    draw_lines(COLOR_LINE);
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

void draw_lines(word cor) {
  // Doubles lines
  TFTscreen.drawFastHLine(COURT_X, COURT_Y, COURT_W, cor);
  TFTscreen.drawFastHLine(COURT_X, COURT_Y + COURT_H, COURT_W, cor);
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
} // draw_lines
