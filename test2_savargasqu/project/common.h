// COMMON DEFINITIONS
#include <SPI.h>
#include <TFT.h>
#include <avr/pgmspace.h>

// TFT DEFINITIONS
#define CS  10
#define DC   9
#define RST  8

// 16-bit ('565') colors
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0x001F
#define COLOR_CYAN 0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW 0xFFE0
#define COLOR_ORANGE 0xFC00

/// Court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108

// Court structure
typedef struct {
	int top;
	int bottom;
	int left;
	int right;
} court_t;

