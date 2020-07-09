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
Button btn(4);
bool btn_flag = false;

// court_t {top, bottom, left, right}. 1px smaller for collisions
court_t court = {COURT_Y + 1, COURT_Y + COURT_H, COURT_X + 1, COURT_X + COURT_W - 1};
// ball_t{pX, pY, dX, dY}
ball_t ball = {CENTER_X, CENTER_Y, CENTER_X, CENTER_Y, -1, 1};
// player_t {score, side, paddle_t{pX, pY, oldY} }
player_t playerL = {0, left, {court.left, court.top - 1, court.top}};
player_t playerR = {0, right, {court.right - PADDLE_W, court.top - 1, court.top}};

void setup() {
  TFTscreen.begin(INITR_BLACKTAB);
  //TFTscreen.begin();
  btn.begin();
  TFTscreen.stroke(COLOR_LINE);
}

void loop() {
  static unsigned long prev_millis = 0;
  if (millis() - prev_millis > MILLIS_GAME) {
    prev_millis = millis();

    // Update paddles' positions
    move_paddle(&playerL.paddle, map(analogRead(PIN_LEFT), 0, 1023, court.top, court.bottom - PADDLE_H));
    move_paddle(&playerR.paddle, map(analogRead(PIN_RIGHT), 0, 1023, court.top, court.bottom - PADDLE_H));
    draw_paddle(&playerL.paddle, COLOR_COURT_DARK);
    draw_paddle(&playerR.paddle, COLOR_COURT_DARK);

    tennis_match();
  }
}

// Game logic state machine
void tennis_match() {
  static state_t game_state = new_game;
  static side_t goal_side = no_goal;

  switch (game_state) {
    case menu:
      btn.read();
      if (btn.wasPressed()) {
        btn_flag = true;
      }
      if (btn_flag) {
        btn_flag = false;
        game_state = play_game;
      }
      break;

    case new_game:
      draw_decorations();
      // Reset players' scores
      playerL.score = 0;
      playerR.score = 0;
      draw_score(&playerL);
      draw_score(&playerR);
      reset_ball(&ball);
      game_state = menu;
      break;

    case serve_game:
      reset_ball(&ball);
      game_state = play_game;
      break;

    case play_game:
      move_ball(&ball);
      draw_ball(&ball, COLOR_COURT_DARK);

      goal_side = check_goal(&ball, &court);
      if (goal_side == no_goal) {
        if (ball.speedX > 0) {
          check_collision_paddle(&ball, &playerR.paddle);
        } else if (ball.speedX < 0) {
          check_collision_paddle(&ball, &playerL.paddle);
        }
        check_collision_court(&ball, &court);
      } else {
        if (goal_side == right) {
          update_scores(&playerL, &playerR);
        } else if (goal_side == left) {
          update_scores(&playerR, &playerL);
        }
        if (playerL.score == sGame || playerR.score == sGame) {
          game_state = game_over;
        } else {
          game_state = serve_game;
        }
      }
      break;

    case game_over:
      btn.read();
      if (btn.wasPressed()) {
        btn_flag = true;
      }
      if (btn_flag) {
        btn_flag = false;
        game_state = new_game;
      }
      break;
  }
} // tennis_match


void reset_ball(ball_t *b) {
  //move ball to centre
  b->oldX = b->posX;
  b->oldY = b->posY;
  b->posX = b->speedX > 0 ? SERVICE_RX - 2 : SERVICE_LX - 2;
  b->posY = CENTER_Y - 2;
  draw_ball(b, COLOR_COURT_DARK);
  b->speedX = b->speedX > 0 ? -1 : 1;
  b->speedY = b->speedY > 0 ? -1 : 1;
} // reset_ball

side_t check_goal(ball_t *b, court_t *crt) {
  if (b->posX <= crt->left) {
    return left;
  } else if (b->posX >= court.right - BALL_SIZE) {
    return right;
  }
  return no_goal;
} // check_goal

void update_scores(player_t * win, player_t * lose) {
  if (lose->score >= s40 && win->score >= s30) { // Deuce logic
    if (win->score == s30 or win->score == sNoAdv) { // if 30 or no adv, deuce
      win->score = sDeuce;
      lose->score = sDeuce;
    } else if (win->score == sAdv) { // if adv, game
      win->score = sGame;
    } else { // deuce, win adv, lose !adv
      win->score += 1;
      lose->score -= 1;
    }
    draw_score(lose);
  } else {
    win->score += 1;
  }
  draw_score(win);
}

/*
  COLLISSION FUNCTIONS
*/
/* get_new_speed: Returns an integer between (-2, 2)
  depending on which part of the paddle the ball's position hits */
int get_new_speed(int dy) {
  if (dy < 2) {
    return -2;
  } else if (dy < 8) {
    return -1;
  } else if (dy < 14) {
    return 1;
  } else {	// if (dy < 20) {
    return 2;
  }
} // get_new_speed

/* check_collision: If the ball hits a paddle, bounce */
void check_collision_paddle(ball_t *b, paddle_t *p) {
  // Check y-axis
  if (b->posY < p->posY + PADDLE_H && b->posY + BALL_SIZE > p->posY) {
    // if ball collides on the left side of the paddle
    if (b->speedX > 0 && b->posX + BALL_SIZE >= p->posX) {
      b->speedY += get_new_speed(b->posY - p->posY);
      b->speedX += 1;
      b->speedX *= -1;
      // if ball collides on the right side of the paddle
    } else if (b->speedX < 0 && b->posX <= p->posX + PADDLE_W) {
      b->speedY += get_new_speed(b->posY - p->posY);
      b->speedX -= 1;
      b->speedX *= -1;
    }
  }
} // check_collision_paddle

void check_collision_court(ball_t *b, court_t *crt) {
  if (b->posY <= crt->top || b->posY >= crt->bottom - BALL_SIZE) {
    b->speedY *= -1;
  }
} // check_collision_court

/*
  MOVE FUNTIONS
*/
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

/*
  DRAW FUNCTIONS
*/
void draw_score(player_t *player) {
  static const char score_str[8][3] = {" 0", "15", "30", "40", " G", "  ", "D ", "AD"};
  static int str_left = 0;
  static int str_right = 0;
  TFTscreen.setTextSize(2);

  if (player->side == right) {
    // Erase previous score
    TFTscreen.stroke(COLOR_COURT_LIGHT);
    TFTscreen.text(score_str[str_right], SCORE_RX, 4);

    str_right = player->score; //(char)player->score + '0';
    // Draw new score
    TFTscreen.stroke(COLOR_WHITE);
    TFTscreen.text(score_str[str_right], SCORE_RX, 4);
  } else if (player->side == left) {
    // Erase previous score
    TFTscreen.stroke(COLOR_COURT_LIGHT);
    TFTscreen.text(score_str[str_left], SCORE_LX, 4);

    str_left = player->score; //(char)player->score + '0';
    // Draw new score
    TFTscreen.stroke(COLOR_WHITE);
    TFTscreen.text(score_str[str_left], SCORE_LX, 4);
  }
} // draw_score

// draw_ball: Also calls draw_lines
void draw_ball(ball_t *b, word bg) {
  //if (b->posX != b->oldX || b->posY != b->oldY) {
  TFTscreen.fillRect(b->oldX, b->oldY, BALL_SIZE, BALL_SIZE, bg); // Erase previous position
  draw_lines(COLOR_LINE);
  // Draw new position
  for (int row = 0; row < BALL_SIZE; row++) {
    for (int col = 0; col < BALL_SIZE; col++) {
      word px = pgm_read_word(img_ball + row * BALL_SIZE + col);
      TFTscreen.drawPixel(col + b->posX, row + b->posY, px);
    }
  }
  //}
} // draw_ball

void draw_paddle(paddle_t *p, word bg) {
  if (p->oldY != p->posY) {
    // Erase previous position
    TFTscreen.fillRect(p->posX, p->oldY, PADDLE_W, PADDLE_H, bg);
    // Draw new position
    for (int row = 0; row < PADDLE_H; row++) {
      for (int col = 0; col < PADDLE_W; col++) {
        word px = pgm_read_word(img_paddle + row * PADDLE_W + col);
        TFTscreen.drawPixel(col + p->posX, row + p->posY, px);
      }
    }
  }
} // draw_paddle

void draw_lines(word cor) {
  // Doubles lines
  //TFTscreen.drawFastHLine(COURT_X, COURT_Y, COURT_W, cor);
  //TFTscreen.drawFastHLine(COURT_X, COURT_Y + COURT_H, COURT_W, cor);
  TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H + 1, COLOR_LINE);
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

void draw_decorations() {
  TFTscreen.background(COLOR_COURT_LIGHT);
  // Draw court
  TFTscreen.fillRect(COURT_X, COURT_Y, COURT_W, COURT_H + 1, COLOR_COURT_DARK);
  //TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H + 1, COLOR_LINE);
  draw_lines(COLOR_LINE);

  TFTscreen.setTextSize(1);
  TFTscreen.setRotation(3); // Upside down
  TFTscreen.text("AUSTRALIAN OPEN", 36, 18);
  TFTscreen.setRotation(1); // Normal orientation
  TFTscreen.text("AUSTRALIAN OPEN", 36, 19);
} // draw_decorations
