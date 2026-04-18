#include "globals.h"

// Initialize Hardware
Adafruit_TCS34725 rgb_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_240MS, TCS34725_GAIN_1X);
Servo servo_left;
Servo servo_right;
Stepper stepper(STEPS_PER_ROTATION, STEPPER_1_PIN, STEPPER_3_PIN, STEPPER_2_PIN, STEPPER_4_PIN);
Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(LED_NUM, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial printer(PRINTER_TX, PRINTER_RX);

// Initialize State Variables
int steps_to_goal[MAX_BOTTLES_IN_TUNNEL] = {0, 0, 0, 0, 0};
int next_goal[MAX_BOTTLES_IN_TUNNEL] = {GOAL_NAN, GOAL_NAN, GOAL_NAN, GOAL_NAN, GOAL_NAN};
int bottle_colors[MAX_BOTTLES_IN_TUNNEL] = {COLOR_UNKNOWN, COLOR_UNKNOWN, COLOR_UNKNOWN, COLOR_UNKNOWN, COLOR_UNKNOWN};
int oldest_bottle_idx = -1;
int newest_bottle_idx = -1;
int dir_newest = STEP_FRONT;
int servo_current_pos_color = COLOR_UNKNOWN;
int bottles_in_tunnel = 0;

int bottles_cnt[3] = {0, 0, 0};
int leading_rot_led = 0;
bool enable_led_rot = false;