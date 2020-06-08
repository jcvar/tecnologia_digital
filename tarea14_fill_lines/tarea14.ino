/*
Tarea 14
Fill Lines

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-08
*/
#include "common.h"
#include "cola.h"

#define COLOR_COLA 0x3984
// Image params
#define X 72
#define Y 48
#define W 16
#define H 32

// // Fill params
#define FX 80
#define FY 48
#define FW 16
#define FH 30


#define ANALOG_PIN A0
#define MILLIS 200

const int strSize = 5;
char val_str[strSize] = {0, 0, 0, 0};
int sensor = 0;

// Function declatarions
void draw_image(int, int, int, int);
void draw_cola(int, int, int, int, int);
void print_val(char[], int);

// Fill line sizes (bottom to top)
const int fill_size[31] = {4, 5, 5, 5, 4, 4, 4, 3, 3, 3, 4, 4, 4, 5, 0, 0, 0, 0, 0, 0, 5, 4, 4, 3, 3, 2, 2, 2, 2, 3, 2};

void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	
	// Draw court and cola
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	draw_image(X, Y, W, H);
	
	// Print sensor info
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text("SENSOR: ", 80, 12);
	print_val(val_str, sensor);
}

void loop() {
	static unsigned long prev_ms = 0;
	if(millis() - prev_ms >= MILLIS){
		prev_ms = millis();
		sensor = analogRead(ANALOG_PIN);
		print_val(val_str, sensor);
		draw_cola(sensor, FX, FY, FW, FH);
		
	}
}

void draw_cola(int val, int fx, int fy, int fw, int fh){
	static int fill = 0;
	fill = map(val, 0, 1023, 0, fh);
	
	draw_image(X, Y, W, 2);
	
	for(int i = 0; i < fh; i++){
		if(i < fill){
			TFTscreen.drawFastHLine(fx-fill_size[i], fy + fh - i, fill_size[i]*2, COLOR_YELLOW);
		}else{
			TFTscreen.drawFastHLine(fx-fill_size[i], fy + fh - i, fill_size[i]*2, COLOR_BLACK);
		}
	}
	
	if(fill == fh){
		TFTscreen.drawFastHLine(fx-fill_size[29], fy + fh - 29, fill_size[29]*2, COLOR_RED);
		TFTscreen.drawFastHLine(fx-fill_size[30], fy + fh - 29, fill_size[30]*2, COLOR_RED);
	}
}

void int_to_str(char arr[], int val){
	int num;
	for(int i = strSize - 2; i >=0; i--){
		num = val%10;
		arr[i] = '0' + num;
		val/=10;
	}
	for(int i = 0; i < strSize - 2; i++){
		if(arr[i] == '0'){
			arr[i] = ' ';
		} else {
			break;
		}
	}
}

void print_val(char str[], int pr_val){
	TFTscreen.setTextSize(1);
	TFTscreen.stroke(COLOR_BLACK);
	TFTscreen.text(str, 128, 12); // Must erase previous score
	int_to_str(str, pr_val);
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text(str, 128, 12);
}

void draw_image(int x, int y, int w, int h){
	for (int row=0; row<h; row++){
		for (int col=0; col<w; col++){
			word p = pgm_read_word(cola_img + (w * row + col));
			TFTscreen.drawPixel(x + col, y + row, p);
		}
	}
}

