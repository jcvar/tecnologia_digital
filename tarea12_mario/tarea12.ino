/*
Tarea 12
Mario

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-01
*/
#include "../common.h" // Remember to copy before submitting
#include "mario_img.h"
#include "mario.h"

TFT TFTscreen = TFT(CS, DC, RST);

// Block parameters
#define BLOCK_SIZE 10
#define BLOCK_MILLIS 100
#define BLOCK_SPEED 2

typedef struct {
	int posX;
	int posY;
	int oldX;
	int oldY;
	int w;
	int h;
	word color;
} block;

block block_g = {
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		BLOCK_SIZE,
		BLOCK_SIZE,
		COLOR_GREEN
	};

block block_r = {
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		BLOCK_SIZE,
		BLOCK_SIZE,
		COLOR_RED
	};

Mario myMario;
bool game_over = false;
int score = 0;
const int strSize = 4;
char score_str[strSize] = {0, 0, 0};
typedef enum {	s0, s1, s2}  block_state;

// Function declatarions
void check_collision(block);
void draw_block(block);
void endgame();
void draw_score();

void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	
	// Mario initialization
	myMario.posX=20;
	myMario.posY=88;
	myMario.jumpPosY=60;
	myMario.isJumping=false;
	myMario.state=t0;
	myMario.walkingDelay=150;
	
	// Draw court and ground
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	//TFTscreen.drawFastHLine()
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text("SCORE: ", 90, 15);
	draw_score();
}

void loop() {
	static unsigned long block_ms = 0;
	if(!game_over){
		myMario.walking();
		myMario.jumping(2000,4);
		if(millis() - block_ms >= BLOCK_MILLIS){
			block_ms = millis();
			move_blocks();
			if(!myMario.isJumping){
				check_collision(block_g);
				check_collision(block_r);
			}
		}
	}
}

void move_blocks() {
	static block_state bs = s0;
	switch(bs) {
		case s0:
			block_g.posX -= BLOCK_SPEED;
			if (block_g.posX <= 80){
				bs = s1;
			}
			break;
			
		case s1:
			block_g.posX -= BLOCK_SPEED;
			block_r.posX -= BLOCK_SPEED;
			if(block_g.posX <= COURT_X){
				block_g.posX = COURT_X + COURT_W - BLOCK_SIZE - 1;
				score+=1;
				draw_score();
			}
			if(block_r.posX <= COURT_X){
				block_r.posX = COURT_X + COURT_W - BLOCK_SIZE - 1;
				score+=1;
				draw_score();
			}
			break;
			
		case s2:
			// GAME OVER
			break;
	}
	
	if (block_g.posX != block_g.oldX){	// || block_g.posY != block_g.oldY){
		draw_block(block_g);
		block_g.oldX = block_g.posX;
		//block_g.oldY = block_g.posY;
	}
	if (block_r.posX != block_r.oldX){	// || block_r.posY != block_r.oldY){
		draw_block(block_r);
		block_r.oldX = block_r.posX;
		//block_r.oldY = block_r.posY;
	}
}

void draw_block(block b){
	TFTscreen.fillRect(b.oldX, b.oldY, b.w, b.h, COLOR_BLACK);
	TFTscreen.fillRect(b.posX, b.posY, b.w, b.h, b.color);
}

void check_collision(block b){
	if(b.posX < myMario.posX + myMario.w && myMario.posX < b.posX + b.w){
		game_over = true;
		endgame();
	}
}

void endgame(){
	TFTscreen.stroke(COLOR_RED);
	TFTscreen.setTextSize(2);
	TFTscreen.text("GAME OVER", 28, 40);
	//TFTscreen.drawRect(26, 40, 12, 16, COLOR_WHITE);
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
		}
	}
}

void draw_score(){
	TFTscreen.setTextSize(1);
	TFTscreen.stroke(COLOR_BLACK);
	TFTscreen.text(score_str, 135, 15); // Must erase previous score
	int_to_str(score_str, score);
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text(score_str, 135, 15);
}

