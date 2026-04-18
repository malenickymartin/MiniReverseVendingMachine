#pragma once
#include "globals.h"

void make_step(int dir);
void set_goal(int idx, int goal, int bottle_color);
void bottle_detected();
void ending_seq();
bool in_array(int array[], int array_len, int value);