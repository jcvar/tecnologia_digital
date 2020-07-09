// COMMON DEFINITIONS
#include <SPI.h>
#include <TFT.h>
#include <avr/pgmspace.h>

// TFT DEFINITIONS
#define CS  9
#define DC   10
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

#define MILLIS_LOOP 200
#define BUZZER_PIN 3
#define MODE_PIN 4
#define NEXT_PIN 5

/* TYPES */

// time_t: struct for clock's time and alarm
typedef struct {
  unsigned short hour;
  unsigned short minute;
  unsigned short second;
  bool active;
} mytime_t;

// digit_t: To select a digit's position
typedef enum {
  hour_tens,
  hour_units,
  minute_tens,
  minute_units,
  second_tens,
  second_units
} digit_t;

// state_t: global clock states
typedef enum {
  normal,
  set_alarm_active,
  set_alarm_minute,
  set_alarm_hour,
  set_clock_second,
  set_clock_minute,
  set_clock_hour,
} state_t;


/* FUNCTIONS */

void timer1_isr();  // TimerOne Interrupt Service Routine

void update_time(); // Main clock logic. Increase values in the clk struct

state_t update_state(state_t); // Handles the clock's FSM

void update_next(state_t); // Changes values of mytime_t according to state_t

void choose_draw(state_t); // Select which mytime_t to force_draw
