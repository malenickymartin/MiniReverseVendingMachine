#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <Stepper.h>
#include <SoftwareSerial.h>
#include "Adafruit_NeoPixel.h"
#include "Adafruit_TCS34725.h"
#include "params.h"
#include "pins.h"

// Hardware Objects
extern Adafruit_TCS34725 rgb_sensor;
extern Servo servo_left;
extern Servo servo_right;
extern Stepper stepper;
extern Adafruit_NeoPixel led_strip;
extern SoftwareSerial printer;

// Global State Variables
extern int steps_to_goal[MAX_BOTTLES_IN_TUNNEL];
extern int next_goal[MAX_BOTTLES_IN_TUNNEL];
extern int bottle_colors[MAX_BOTTLES_IN_TUNNEL];
extern int oldest_bottle_idx;
extern int newest_bottle_idx;
extern int dir_newest;
extern int servo_current_pos_color;
extern int bottles_in_tunnel;

extern int bottles_cnt[3];
extern int leading_rot_led;
extern bool enable_led_rot;