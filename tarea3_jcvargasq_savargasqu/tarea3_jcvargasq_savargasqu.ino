/*
    Tarea 3: Semaforo con display TFT
    Tecnologia Digital
    Universidad Nacional de Colombia

    Juan Camilo Vargas Q. - jcvargasq@unal.edu.co
    Sergio Alejandro Vargas Q. - savargasqu@unal.edu.co

    2020-04-20
*/
#include <TFT.h>
#include <SPI.h>

// Display parameters
#define cs  10
#define ds   9
#define rst  8
// Declare display
TFT TFTscreen = TFT(cs, ds, rst);

//  RGB 565 = RRRR RGGG GGGB BBBB
const int R = 0xF800, iR = 0;
const int Y = 0xFFE0, iY = 1;
const int G = 0x07E0, iG = 2;
// Background color
const int K = 0x0000;
const int W = 0xFFFF;
const int O = 0xFB00;
const int light_colors[3] = {R, Y, G};

// Traffic lights state
enum traffic_state_t {
	sR, sRY, sG, sY
};

// Traffic light duration
const int dG  = 4000;
const int dY  = 2000;
const int dR  = 7000;
const int dRY = 2000;

// Walk lights state
enum walk_state_t {
	sDont, sWalk_on, sWalk_off
};

// Walk light duration
const int w_on  = 250;
const int w_off = 250;

// Size of circle representing the LED
#define light_size    16
#define light_pos     40
#define light_offsetx 40
#define light_offsety 24

// Sketch variables
traffic_state_t traffic_light = sG;
walk_state_t walk_light = sDont;

// Turn off all LED circles except for one. Pass color as argument
void draw_lights(traffic_state_t light) {
	static traffic_state_t old_light = sRY;
	
	// Reset all lights
	for (int i = 0; i < 3; i++) {
		TFTscreen.fillCircle(light_offsetx, i*light_pos + light_offsety,  light_size, K);
		TFTscreen.drawCircle(light_offsetx, i*light_pos + light_offsety, light_size, light_colors[i]);
	}
	
	// Light up specific lights
	if(light == sR || light == sRY){
		TFTscreen.fillCircle(light_offsetx, iR*light_pos + light_offsety, light_size, light_colors[iR]);
	}
	if(light == sRY || light == sY){
		TFTscreen.fillCircle(light_offsetx, iY*light_pos + light_offsety, light_size, light_colors[iY]);
	}
	if(light == sG){
		TFTscreen.fillCircle(light_offsetx, iG*light_pos + light_offsety, light_size, light_colors[iG]);
	}
}

void draw_walk(walk_state_t light) {
	static walk_state_t old_light = sWalk_off;
	
	if(light == sDont){
		TFTscreen.stroke(O);
		TFTscreen.text("WALK", 87, 67);
		TFTscreen.text("DONT", 87, 47);
	}
	if(light == sWalk_on){
		TFTscreen.stroke(K);
		TFTscreen.text("DONT", 87, 47);
		TFTscreen.stroke(G);
		TFTscreen.text("WALK", 87, 67);
	}
	if(light == sWalk_off){
		TFTscreen.stroke(K);
		TFTscreen.text("WALK", 87, 67);
	}
}

void traffic_state_machine(){
	static unsigned long traffic_millis = millis();
	switch(traffic_light){
		case sG:
			if((millis() - traffic_millis) >= dG){
				traffic_millis = millis();
				traffic_light = sY;
				draw_lights(traffic_light);
			}
			break;
			
		case sY:
			if((millis() - traffic_millis) >= dY){
				traffic_millis = millis();
				traffic_light = sR;
				draw_lights(traffic_light);
				walk_light = sWalk_on;
				draw_walk(walk_light);
			}
			break;
			
		case sR:
			static unsigned long walk_millis = millis();
			switch(walk_light){
				case sWalk_on:
					if((millis() - walk_millis) >= w_on){
						walk_millis = millis();
						walk_light = sWalk_off;
						draw_walk(walk_light);
					}
					break;
					
				case sWalk_off:
					if((millis() - walk_millis) >= w_off){
						walk_millis = millis();
						walk_light = sWalk_on;
						draw_walk(walk_light);
					}
					break;
			}
			
			if((millis() - traffic_millis) >= dR){
				traffic_millis = millis();
				traffic_light = sRY;
				draw_lights(traffic_light);
				walk_light = sDont;
				draw_walk(walk_light);
			}
			break;
			
		case sRY:
			if((millis() - traffic_millis) >= dRY){
				traffic_millis = millis();
				traffic_light = sG;
				draw_lights(traffic_light);
			}
			break;
	}
}

void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	TFTscreen.setTextSize(2);
	
	TFTscreen.stroke(255, 255, 255);
	for(int i = 0; i < 3; i++){
		// (x, y, w, h, r, c)
		TFTscreen.drawRoundRect(20, 4+40*i, 41, 41, 4, W);
	}
	TFTscreen.drawRoundRect(80, 40, 60, 48, 8, W);
	
	draw_lights(traffic_light);
	draw_walk(walk_light);
}

void loop() {
	traffic_state_machine();
}
