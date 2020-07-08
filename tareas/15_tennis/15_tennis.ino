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
#include "tennis_images.h"
#include "tennis_draw.h"


TFT TFTscreen = TFT(CS, DC, RST);
court_t court = {COURT_Y + 1, COURT_Y + COURT_H - 1, COURT_X + 1, COURT_X + COURT_W - 1}; // 1px smaller for collisions

// ball_t(pX, pY, dX, dY);
ball_t ball = {CENTER_X, CENTER_Y, CENTER_X + 1, CENTER_Y + 1, -1, 1};

// paddle_t(pX, pY);
paddle_t left = {court.left, court.top, court.top - 1};
paddle_t right = {court.right - PADDLE_W, court.top, court.top - 1};

// Game variables
bool start = false;
bool game_over = true;
bool button = false;

const int strSize = 2;
int score_left;
int score_right;
char score_str_left[strSize] = {0};
char score_str_right[strSize] = {0};

// Signatures
int check_collision(ball_t, paddle_t);

void setup() {
  //TFTscreen.begin(INITR_BLACKTAB); //);
  TFTscreen.begin();

  // Draw court
  TFTscreen.background(COLOR_COURT_LIGHT);
  TFTscreen.fillRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_COURT_DARK);
  TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_LINE);
  draw_inner_lines(TFTscreen, COLOR_LINE);
  TFTscreen.stroke(COLOR_LINE);
  //TFTscreen.setTextSize(2);
  TFTscreen.text("AUSTRALIAN OPEN", 68, 4);
  reset_score();

  delay(10000);
}

void loop() {
  static unsigned long millis_ball = 0;
  static unsigned long millis_left = 0;
  static unsigned long millis_right = 0;

  if (millis() - millis_left > MILLIS_PADDLE) {
    millis_left = millis();
    move_paddle(&left, map(analogRead(PIN_LEFT), 1023, 0, court.top, court.bottom - PADDLE_H));
    left.draw(TFTscreen, COLOR_COURT_DARK);
  }
  if (millis() - millis_right > MILLIS_PADDLE) {
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
      reset_score();
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

int check_ranges(int dy) {
  if (dy < 2) {
    return -2;
  } else if (dy < 8) {
    return -1;
  } else if (dy < 14) {
    return 1;
  } else {	// if (dy < 20) {
    return 2;
  }
}

/* check_collision: returns the new speed of the ball */
int check_collision(ball_t b, paddle_t p) {
  if (b.posY < p.posY + p.h && b.posY + b.h > p.posY) {	// Check y-axis
    if (b.speedX > 0 && b.posX + b.w >= p.posX) {	// ball collides on the left side of the paddle
      return check_ranges(b.posY - p.posY);
    } else if (b.speedX < 0 && b.posX <= p.posX + p.w) {	// ball collides on the right side of the paddle
      return check_ranges(b.posY - p.posY);
    }
  }
  return 0; // no collision
}

bool update_score(int result) {
  if (result == -1) {	// Goal on left side
    score_right += 1;
    TFTscreen.stroke(COLOR_BLACK);
    TFTscreen.text(score_str_right, 114, 4); // Must erase previous score
    score_str_right[0] = '0' + score_right;
    TFTscreen.stroke(COLOR_WHITE);
    TFTscreen.text(score_str_right, 114, 4);
    return true;
  } else if (result == 1) {	// Goal on right side
    score_left += 1;
    TFTscreen.stroke(COLOR_BLACK);
    TFTscreen.text(score_str_left, 40, 4); // Must erase previous score
    score_str_left[0] = '0' + score_left;
    TFTscreen.stroke(COLOR_WHITE);
    TFTscreen.text(score_str_left, 40, 4);
    return true;
  }
  return false;
}

void reset_score() {
  score_left = -1;
  score_right = -1;
  update_score(-1);
  update_score(1);
}



int check_goal(court_t crt) { // Return -1 or 1 for goal on left or right ends, else 0
  if (posX <= crt.left)  {
    return -1;
  } else if (posX >= crt.right - w) {
    return 1;
  }
  return 0;
} // check_goal()
