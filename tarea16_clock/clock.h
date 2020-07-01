// Time struct for clock and alarm
typedef struct {
  unsigned hour;
  unsigned minute;
  unsigned second;
  bool active;
} time_t;

// Enum for global clock states (modes)
typedef enum {
  set_normal,
  set_alarm_active,
  set_alarm_hour,
  set_alarm_min,
  set_hour,
  set_min,
  set_sec,
} state_t;

// Enum to select digit position
typedef enum {
  hour_tens,
  hour_units,
  minute_tens,
  minute_units,
  second_tens,
  second_units
} digit_t;
