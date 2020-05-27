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

typedef enum state_t {	a0, a1, a2, a3, a4, a5};


// Bitmap
const PROGMEM word  myImage[0x30C] ={
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0010 (16)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0020 (32)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0022, 0x0022, 0x0022, 0x0021,   // 0x0030 (48)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0040 (64)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0021, 0x0065, 0x012C, 0x0237, 0x0258, 0x0258, 0x0259, 0x0258, 0x0258, 0x0237, 0x01B2,   // 0x0050 (80)
0x00A6, 0x0043, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0060 (96)
0x0000, 0x0000, 0x0028, 0x01F4, 0x02DE, 0x02FF, 0x02FF, 0x031F, 0x031F, 0x031F, 0x031F, 0x031F, 0x031F, 0x02FF, 0x02DD, 0x0279,   // 0x0070 (112)
0x00CC, 0x0003, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000B,   // 0x0080 (128)
0x00BC, 0x02FF, 0x031F, 0x031F, 0x02FF, 0x031F, 0x031F, 0x031F, 0x031F, 0x031F, 0x031F, 0x031F, 0x031F, 0x031F, 0x01FF, 0x0017,   // 0x0090 (144)
0x0002, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x0017, 0x001F, 0x015F,   // 0x00A0 (160)
0x025F, 0x027F, 0x02BC, 0x025A, 0x0217, 0x0238, 0x0259, 0x0239, 0x027B, 0x023E, 0x023F, 0x021F, 0x005F, 0x001F, 0x000A, 0x0000,   // 0x00B0 (176)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0055,   // 0x00C0 (192)
0x2A50, 0x7434, 0x8410, 0x634E, 0x6390, 0x8474, 0x6C14, 0x212F, 0x1099, 0x001F, 0x001F, 0x001F, 0x001A, 0x0003, 0x0000, 0x0000,   // 0x00D0 (208)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x0015, 0x001F, 0x001F, 0x001B, 0x0854, 0x4A51, 0xCE7A, 0xCE38,   // 0x00E0 (224)
0xF7BE, 0xF7BE, 0xCE79, 0xE71C, 0xFFFF, 0xA513, 0x2107, 0x0011, 0x001E, 0x001F, 0x001C, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x00F0 (240)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x0016, 0x001F, 0x001D, 0x2954, 0xAD57, 0xC618, 0xF7BE, 0xEF7D, 0xC618, 0xE73C,   // 0x0100 (256)
0xF79E, 0xCE59, 0xD6BA, 0xFFFF, 0xCE79, 0x528B, 0x0033, 0x001F, 0x001C, 0x0009, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0110 (272)
0x0000, 0x0000, 0x0000, 0x0001, 0x0013, 0x001D, 0x10B0, 0xA537, 0xFFFF, 0xE73C, 0xC638, 0xF79E, 0xF79E, 0xCE59, 0xE71C, 0xF7BE,   // 0x0120 (288)
0xCE79, 0xCE79, 0xFFFF, 0xDEFB, 0x6330, 0x0879, 0x001A, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0130 (304)
0x0000, 0x0001, 0x000A, 0x0006, 0x39E7, 0xA533, 0xA513, 0xCE58, 0xB596, 0x8C71, 0xC638, 0xC618, 0x9CD3, 0xB596, 0xC638, 0x9CF3,   // 0x0140 (320)
0x9CF3, 0xCE79, 0x8C72, 0x086E, 0x000D, 0x0006, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0150 (336)
0x0009, 0x002A, 0x108A, 0x0029, 0x0026, 0x0025, 0x0024, 0x0004, 0x0024, 0x0024, 0x0024, 0x0024, 0x0025, 0x0024, 0x0004, 0x0028,   // 0x0160 (352)
0x002E, 0x0011, 0x084B, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0840, 0x0003,   // 0x0170 (368)
0x000D, 0x0015, 0x0013, 0x0012, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0012, 0x0014, 0x0015, 0x0009,   // 0x0180 (384)
0x0840, 0x0821, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20E2, 0x6245, 0x0000, 0x0004, 0x0010,   // 0x0190 (400)
0x000E, 0x0010, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0013, 0x000E, 0x000E, 0x002A, 0x0000, 0x3122, 0x6245,   // 0x01A0 (416)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1061, 0x3123, 0x18A1, 0x28E3, 0x7AE9, 0x8307, 0x5A07,   // 0x01B0 (432)
0x0009, 0x0008, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x20C8, 0x82E7, 0x72A6, 0x1060, 0x0000, 0x1061, 0x3123, 0x0000, 0x0000,   // 0x01C0 (448)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1061, 0x82E7, 0xED6D, 0xF58D, 0xE52C, 0x51C4, 0x1081,   // 0x01D0 (464)
0xB409, 0xBC4A, 0xBC4A, 0x6265, 0x0020, 0xB42A, 0xF58D, 0xD4EC, 0x41A4, 0x2902, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x01E0 (480)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20C2, 0x72A6, 0xCCAB, 0xED6D, 0xA3C9, 0x7AE7, 0xE54D, 0xED4D,   // 0x01F0 (496)
0xED6D, 0xB42A, 0x72A7, 0xDD0C, 0xED4D, 0xC46B, 0x49A4, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0200 (512)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1061, 0x10E3, 0x0986, 0x41E5, 0xBC2A, 0xDD0C, 0xED4D, 0xED4D, 0xED6D, 0xED6D, 0xED6D,   // 0x0210 (528)
0xDD0C, 0xD4CB, 0x8327, 0x3164, 0x0924, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0220 (544)
0x0000, 0x0000, 0x3963, 0x8B28, 0x7AE7, 0x21A5, 0x01E7, 0x1965, 0x2164, 0x7AC7, 0xABC9, 0xABE9, 0xABE9, 0xA3A9, 0x41C4, 0x1923,   // 0x0230 (560)
0x1A48, 0x1228, 0x6286, 0x9348, 0x4184, 0x1061, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18A1,   // 0x0240 (576)
0xC46B, 0xED4D, 0xED4D, 0x6A66, 0x0208, 0x138E, 0x23CF, 0x0A28, 0x09C7, 0x1A49, 0x09C7, 0x09C7, 0x1AEB, 0x0B4D, 0x134D, 0x2164,   // 0x0250 (592)
0xCC8B, 0xED6D, 0xE52C, 0x72A7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1081, 0x9B88, 0xCCAB,   // 0x0260 (608)
0xC48B, 0x5A87, 0x0AAA, 0x0B6D, 0x2C92, 0x1C30, 0x0BAE, 0x2C92, 0x1C10, 0x0BAE, 0x2CB2, 0x1BEF, 0x0B0C, 0x21E6, 0xABE9, 0xCCAB,   // 0x0270 (624)
0xBC4A, 0x51E5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0x0840, 0x1081, 0x10C2,   // 0x0280 (640)
0x11EC, 0x0AAB, 0x1BAE, 0x2450, 0x0B4C, 0x23EF, 0x240F, 0x0B4C, 0x240F, 0x23CE, 0x0A48, 0x0082, 0x0820, 0x0840, 0x0841, 0x0000,   // 0x0290 (656)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x086A, 0x08C7,   // 0x02A0 (672)
0x0929, 0x19F1, 0x0929, 0x0950, 0x19B3, 0x092B, 0x1151, 0x19B2, 0x08C9, 0x0022, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02B0 (688)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3942, 0x82A6, 0x82A6, 0x82A6, 0x82C8,   // 0x02C0 (704)
0x59E5, 0x1884, 0x1885, 0x59C5, 0x82A9, 0x82A9, 0x82A7, 0x7A85, 0x28E1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02D0 (720)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20A1, 0x51C3, 0x82C5, 0x8AE5, 0x6203, 0x3922, 0x0820,   // 0x02E0 (736)
0x0020, 0x3922, 0x7264, 0x8AE5, 0x82C5, 0x6A44, 0x28E1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02F0 (752)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20A1, 0x20C1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0300 (768)
0x1060, 0x20C1, 0x20A1, 0x1060, 0x0840, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, };


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
	if(millis() - sprite.prevMillis > MILLIS) {
		sprite.prevMillis = millis();
		move_sprite();
	}
}


void move_sprite() {
	static state_t ani_state = a0;
	
	switch(ani_state) {
		case a0: // RIGHT MOVEMENT TOP
			if(sprite.posX + sprite.w + sprite.offset > FAR_RIGHT){
				ani_state = a1;
			} else {
				sprite.posX += sprite.offset;
			}
			break;
			
		case a1: // LEFT MOVEMENT TOP
			if(sprite.posX - sprite.offset <= MID_LEFT){
				ani_state = a2;
			} else {
				sprite.posX -= sprite.offset;
			}
			break;
			
		case a2: // DOWN MOVEMENT
			if(sprite.posY + sprite.h + sprite.offset >= FAR_BOTTOM){
				ani_state = a3;
			} else {
				sprite.posY += sprite.offset;
			}
			break;
			
		case a3: // LEFT MOVEMENT BOTTOM
			if(sprite.posX - sprite.offset <= FAR_LEFT){
				ani_state = a4;
			} else {
				sprite.posX -= sprite.offset;
			}
			break;
			
		case a4: // RIGHT MOVEMENT BOTTOM
			if(sprite.posX + sprite.w + sprite.offset > FAR_RIGHT){
				ani_state = a5;
			} else {
				sprite.posX += sprite.offset;
			}
			break;
			
		case a5: // HALT
			break;
	}
	
	if(sprite.posX != sprite.oldX || sprite.posY != sprite.oldY){
		draw_image();
	}
}

void draw_image() {
	for (int row = 0; row < sprite.h; row++) {
		for (int col = 0; col < sprite.w; col++) {
			word p = pgm_read_word(myImage + (row*sprite.w + col));
			TFTscreen.drawPixel(col + sprite.posX, row + sprite.posY, p);
		}
	}
	
	// Draw rect to black out shadow (x axis)
	if(sprite.posX > sprite.oldX){
		TFTscreen.fillRect(sprite.oldX, sprite.posY, sprite.offset, sprite.h, COLOR_BLACK);
	} else if(sprite.posX < sprite.oldX){
		TFTscreen.fillRect(sprite.posX + sprite.w, sprite.posY, sprite.offset, sprite.h, COLOR_BLACK);
	}
	// Draw rect to black out shadow (y axis)
	if(sprite.posY > sprite.oldY){
		TFTscreen.fillRect(sprite.posX, sprite.oldY, sprite.w, sprite.offset, COLOR_BLACK);
	} else if(sprite.posY < sprite.oldY){
		TFTscreen.fillRect(sprite.posX, sprite.posY + sprite.h, sprite.w, sprite.offset, COLOR_BLACK);
	}
	sprite.oldX = sprite.posX;	
	sprite.oldY = sprite.posY;	
	
	//TFTscreen.drawRect(sprite.posX, sprite.posY, sprite.w, sprite.h, COLOR_MAGENTA); // For debugging
}

