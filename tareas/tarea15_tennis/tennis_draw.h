void draw_inner_lines(TFT TFTscr, word cor) {
  // Singles lines
  TFTscr.drawFastHLine(COURT_X, SINGLES_UY, COURT_W, cor);
  TFTscr.drawFastHLine(COURT_X, SINGLES_DY, COURT_W, cor);
  // Service lines
  TFTscr.drawFastVLine(SERVICE_LX, SINGLES_UY, SINGLES_H, cor);
  TFTscr.drawFastVLine(SERVICE_RX, SINGLES_UY, SINGLES_H, cor);
  // Center service line
  TFTscr.drawFastHLine(SERVICE_LX, CENTER_Y, SERVICE_W, cor);
  // Net
  TFTscr.drawFastVLine(CENTER_X, COURT_Y, COURT_H, cor);
} // draw_inner_lines

void draw_outer_lines(TFT TFTscr, word cor) {
  TFTscr.drawFastHLine(COURT_X, COURT_Y, COURT_W, cor);
  TFTscr.drawFastHLine(COURT_X, COURT_Y + COURT_H, COURT_W, cor);
} // draw_outer_lines


void draw_ball(ball_t b, TFT TFTscr, word bg) {
  if (b.posX != b.oldX || b.posY != b.oldY) {
    TFTscr.fillRect(b.oldX, b.oldY, BALL_W, BALL_H, bg);
    for (int row = 0; row < BALL_H; row++) {
      for (int col = 0; col < BALL_W; col++) {
        word p = pgm_read_word(img_ball + row * BALL_W + col);
        TFTscr.drawPixel(col + b.posX, row + b.posY, p);
      }
    }
  }
} // draw_ball()

void draw_paddle(paddle_t p, TFT TFTscr, word bg) {
  if (p.oldY != p.posY) {
    TFTscr.fillRect(p.posX, p.oldY, PADDLE_W, PADDLE_H, bg);
    for (int row = 0; row < PADDLE_W; row++) {
      for (int col = 0; col < PADDLE_W; col++) {
        word p = pgm_read_word(img_paddle + row * PADDLE_W + col);
        TFTscr.drawPixel(p.posX + col, p.posY + row, p);
      }
    }
  }
} // draw()
