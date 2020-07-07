/*
Sergio Alejandro Vargas Q.
Tecnologia Digital
Universidad Nacional de Colombia
2020-07-06

# PARCIAL 2
  * Escribir programa en C++ para realizar en la pantalla grafica TFT la animacin mostrada en el archivo ani.wmv
  * Realizar video explicando el cdigo y mostrar la simulacin.
  * Incluir el video y el archivo .ino en la carpeta de entrega al final del examen.

*/
#include "common.h"
#include "image.h"

#define SCREEN_W 160
#define SCREEN_H 128
#define IMAGE_SIZE 32
#define COLOR_GRAY 0x632C

TFT TFTscreen = TFT(CS, DC, RST);
court_t court;

typedef enum{	s0, s1, s2, s3, s4, s5} state_t;

void fsm();
void draw_number(int, int, state_t);
void draw_image(int, int, bool);

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(COLOR_BLACK);
	TFTscreen.setTextSize(1);
	TFTscreen.stroke(COLOR_WHITE);
	// Court initialization
	court.top = COURT_Y;
	court.bottom = COURT_Y + COURT_H;
	court.left = COURT_X;
	court.right = COURT_X + COURT_W;
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}

void loop() {
	static unsigned long prev_millis = 0;
	if(millis() > prev_millis + 1000){
		prev_millis = millis();
		fsm();
	}
}

void fsm() {
	static state_t state = s0;
	static int x = 0, y = 0;
	static bool is_magenta = false;
	
	switch(state) {
		case s0:
			TFTscreen.background(COLOR_GRAY);
			TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
			is_magenta = false;
			state = s1;
			break;
		case s1:
			x = 0;
			y = 0;
			draw_image(x, y, is_magenta);
			draw_number(x + 14, y + 12, state);	
			state = s2;
			break;
		case s2:
			x = SCREEN_W - IMAGE_SIZE;
			y = 0;
			draw_image(x, y, is_magenta);
			draw_number(x + 14, y + 12, state);	
			state = s3;
			break;
		case s3:
			x = SCREEN_W - IMAGE_SIZE;
			y = SCREEN_H - IMAGE_SIZE;
			draw_image(x, y, is_magenta);
			draw_number(x + 14, y + 12, state);	
			state = s4;
			break;
		case s4:
			x = 0;
			y = SCREEN_H - IMAGE_SIZE;
			draw_image(x, y, is_magenta);
			draw_number(x + 14, y + 12, state);	
			state = s5;
			break;
		case s5:
			x = (SCREEN_W - IMAGE_SIZE)/2;
			y = (SCREEN_H - IMAGE_SIZE)/2;
			is_magenta = true;
			TFTscreen.drawFastHLine(COURT_X, SCREEN_H/2, COURT_W, COLOR_BLUE);
			TFTscreen.drawFastVLine(SCREEN_W/2, COURT_Y, COURT_H, COLOR_BLUE);
			draw_image(x, y, is_magenta);
			draw_number(x + 14, y + 12, state);	
			state = s0;
			break;
		default:
			state = s0;
	}
}//end fsm

void draw_image(int posx, int posy, bool has_filter) {
	for (int row = 0; row < IMAGE_SIZE; row++) {
		for (int col = 0; col < IMAGE_SIZE; col++) {
			word p = pgm_read_word(Image + (row*IMAGE_SIZE + col));
			if (has_filter && p == 0x7E08) {
				p = COLOR_MAGENTA;
			}
			TFTscreen.drawPixel(col + posx, row + posy, p);
		}
	}
}

void draw_number(int posx, int posy, state_t stt) {
	static char c = '\0';
	c = (char)stt+48; // state + ASCII '0'
	TFTscreen.text(&c, posx, posy);
}
