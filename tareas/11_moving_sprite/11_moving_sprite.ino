/*
  Tarea 11
  Animación de un bitmap

  Juan Camilo Vargas Q.         jcvargasq@unal.edu.co
  Sergio Alejandro Vargas Q.    savargasqu@unal.edu.co

  Tecnologia Digital
  Universidad Nacional de Colombia
  2020-05-27
*/
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <pgmspace.h>

// pin definition for the Uno
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

// Court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108

// Green walls
#define FAR_LEFT   20
#define MID_LEFT   62
#define MID_RIGHT  98
#define FAR_RIGHT  140
#define FAR_TOP    20
#define MID_TOP    51
#define MID_BOTTOM 76
#define FAR_BOTTOM 108

// Sketch parameters
#define MILLIS 400

struct sprite_t {
  int w; // width
  int h; // height
  int posX;
  int posY;
  int oldX;
  int oldY;
  int offset;
  unsigned long prevMillis;
};

struct sprite_t sprite = {
  30, // width
  26, // height
  21, // posX
  21, // posY
  21, // oldX
  21, // oldY
  5,  // offset
  0   // prevMillis
};

typedef enum state_t {a0, a1, a2, a3, a4, a5};

void setup() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);

  // Outer vertical lines
  TFTscreen.drawFastVLine(FAR_LEFT, COURT_Y, COURT_H, COLOR_GREEN);
  TFTscreen.drawFastVLine(FAR_RIGHT, COURT_Y, COURT_H, COLOR_GREEN);
  // Outer horizontal lines
  TFTscreen.drawFastHLine(FAR_LEFT, FAR_TOP, FAR_RIGHT - FAR_LEFT, COLOR_GREEN);
  TFTscreen.drawFastHLine(FAR_LEFT, FAR_BOTTOM, FAR_RIGHT - FAR_LEFT, COLOR_GREEN);
  // Inner vertical lines
  TFTscreen.drawFastVLine(MID_LEFT, MID_TOP, MID_BOTTOM - MID_TOP, COLOR_GREEN);
  TFTscreen.drawFastVLine(MID_RIGHT, MID_TOP, MID_BOTTOM - MID_TOP, COLOR_GREEN);
  // Inner top lines
  TFTscreen.drawFastHLine(FAR_LEFT, MID_TOP, MID_LEFT - FAR_LEFT, COLOR_GREEN);
  TFTscreen.drawFastHLine(MID_RIGHT, MID_TOP, FAR_RIGHT - MID_RIGHT, COLOR_GREEN);
  // Inner bottom lines
  TFTscreen.drawFastHLine(FAR_LEFT, MID_BOTTOM, MID_LEFT - FAR_LEFT, COLOR_GREEN);
  TFTscreen.drawFastHLine(MID_RIGHT, MID_BOTTOM, FAR_RIGHT - MID_RIGHT, COLOR_GREEN);

  // Draw court
  TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
  draw_image();
  delay(MILLIS);
}

void loop() {
  if (millis() - sprite.prevMillis > MILLIS) {
    sprite.prevMillis = millis();
    move_sprite();
  }
}

void move_sprite() {
  static state_t ani_state = a0;
  switch (ani_state) {
    case a0: // RIGHT MOVEMENT TOP
      if (sprite.posX + sprite.w + sprite.offset > FAR_RIGHT) {
        ani_state = a1;
      } else {
        sprite.posX += sprite.offset;
      }
      break;

    case a1: // LEFT MOVEMENT TOP
      if (sprite.posX - sprite.offset <= MID_LEFT) {
        ani_state = a2;
      } else {
        sprite.posX -= sprite.offset;
      }
      break;

    case a2: // DOWN MOVEMENT
      if (sprite.posY + sprite.h + sprite.offset >= FAR_BOTTOM) {
        ani_state = a3;
      } else {
        sprite.posY += sprite.offset;
      }
      break;

    case a3: // LEFT MOVEMENT BOTTOM
      if (sprite.posX - sprite.offset <= FAR_LEFT) {
        ani_state = a4;
      } else {
        sprite.posX -= sprite.offset;
      }
      break;

    case a4: // RIGHT MOVEMENT BOTTOM
      if (sprite.posX + sprite.w + sprite.offset > FAR_RIGHT) {
        ani_state = a5;
      } else {
        sprite.posX += sprite.offset;
      }
      break;

    case a5: // HALT
      break;
  }

  if (sprite.posX != sprite.oldX || sprite.posY != sprite.oldY) {
    draw_image();
  }
}

void draw_image() {
  for (int row = 0; row < sprite.h; row++) {
    for (int col = 0; col < sprite.w; col++) {
      word p = pgm_read_word(myImage + (row * sprite.w + col));
      TFTscreen.drawPixel(col + sprite.posX, row + sprite.posY, p);
    }
  }
  // Draw rect to black out shadow (x axis)
  if (sprite.posX > sprite.oldX) {
    TFTscreen.fillRect(sprite.oldX, sprite.posY, sprite.offset, sprite.h, COLOR_BLACK);
  } else if (sprite.posX < sprite.oldX) {
    TFTscreen.fillRect(sprite.posX + sprite.w, sprite.posY, sprite.offset, sprite.h, COLOR_BLACK);
  }
  // Draw rect to black out shadow (y axis)
  if (sprite.posY > sprite.oldY) {
    TFTscreen.fillRect(sprite.posX, sprite.oldY, sprite.w, sprite.offset, COLOR_BLACK);
  } else if (sprite.posY < sprite.oldY) {
    TFTscreen.fillRect(sprite.posX, sprite.posY + sprite.h, sprite.w, sprite.offset, COLOR_BLACK);
  }
  sprite.oldX = sprite.posX;
  sprite.oldY = sprite.posY;
}
