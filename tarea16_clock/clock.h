// TODO: refactor
// include another button and change the state_t enum

// time_t: struct for clock's time and alarm
typedef struct {
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	bool active;
} mytime_t;

// state_t: global clock states
typedef enum {
	set_normal,
	set_alarm_active,
	set_alarm_hour,
	set_alarm_minute,
	set_hour,
	set_minute,
	set_sec,
} state_t;

// digit_t: To select a digit's position
typedef enum {
	hour_tens,
	hour_units,
	minute_tens,
	minute_units,
	second_tens,
	second_units
} digit_t;
