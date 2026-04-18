#pragma once
#include "globals.h"

void stepper_off();
void stepper_step(int dir);
void buzzer_play_tone(const int tone_to_play[]);
void led_fill_color(const byte color[]);
void led_off();
void led_rot_step();
void servos_to_pos(int bottle_type);