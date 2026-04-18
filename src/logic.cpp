#include "logic.h"
#include "actuators.h"
#include "sensors.h"
#include "printer.h"

void make_step(int dir){
  stepper_step(dir);
  if (enable_led_rot == true){
    led_rot_step();
  }
  for (int i = 0; i < MAX_BOTTLES_IN_TUNNEL; i++){
    if (next_goal[i] != GOAL_NAN){
      steps_to_goal[i] = steps_to_goal[i] - dir;
    }
    if ((next_goal[i] != GOAL_NAN) && (steps_to_goal[i] == 0)){
      if (next_goal[i] == GOAL_END){
        bottles_in_tunnel += -1;
        set_goal(i, GOAL_NAN, COLOR_UNKNOWN);
        if (bottles_in_tunnel > 0){
          oldest_bottle_idx = (oldest_bottle_idx + 1) % MAX_BOTTLES_IN_TUNNEL;
        }
        else {
          oldest_bottle_idx = -1;
          newest_bottle_idx = -1; 
        }
      }
      else if (next_goal[i] == GOAL_OUT){
        enable_led_rot = false;
        led_off();
        dir_newest = STEP_FRONT;
        bottles_in_tunnel += -1;
        set_goal(i, GOAL_NAN, COLOR_UNKNOWN);
        if (bottles_in_tunnel > 0){
          newest_bottle_idx = (newest_bottle_idx - 1) % MAX_BOTTLES_IN_TUNNEL;
        }
        else {
          oldest_bottle_idx = -1;
          newest_bottle_idx = -1; 
        }
        while (analogRead(IR_ANALOG_PIN) <= IR_FREE_THR){
          delay(50);
        }
      }
      else if (next_goal[i] == GOAL_RGB){
        led_off();
        int bottle_color = rgb_read();
        rgb_sensor.setInterrupt(true);
        if (bottle_color == COLOR_UNKNOWN){
          led_fill_color(LED_RGB_UNKNOWN_BOTTLE);
          dir_newest = STEP_BACK;
          set_goal(i, GOAL_OUT, COLOR_UNKNOWN);
        }
        else{
          enable_led_rot = false;
          led_fill_color(LED_RGB_KNOWN_BOTTLE);
          bottles_cnt[bottle_color] = bottles_cnt[bottle_color] + 1;
          dir_newest = STEP_FRONT;
          set_goal(i, GOAL_END, bottle_color);
        }
      }
    }
  }
}

void set_goal(int idx, int goal, int bottle_color){
  switch (goal) {
    case GOAL_NAN:
      next_goal[idx] = GOAL_NAN;
      bottle_colors[idx] = COLOR_UNKNOWN;
      steps_to_goal[idx] = 0;
      break;
    case GOAL_OUT:
      steps_to_goal[idx] = STEPS_RGB_OUT;
      next_goal[idx] = GOAL_OUT;
      bottle_colors[idx] = COLOR_UNKNOWN;
      break;
    case GOAL_END:
      steps_to_goal[idx] = STEPS_RGB_END;
      next_goal[idx] = GOAL_END;
      bottle_colors[idx] = bottle_color;
      break;
    case GOAL_RGB:
      steps_to_goal[idx] = STEPS_IR_RGB;
      next_goal[idx] = GOAL_RGB;
      bottle_colors[idx] = COLOR_UNKNOWN;
      break;
  }
}

void bottle_detected(){
  enable_led_rot = true;
  newest_bottle_idx = (newest_bottle_idx + 1) % MAX_BOTTLES_IN_TUNNEL;
  if (bottles_in_tunnel == 0){
    oldest_bottle_idx = newest_bottle_idx;
  }
  bottles_in_tunnel += 1;
  dir_newest = STEP_FRONT;
  while (analogRead(IR_ANALOG_PIN) < IR_FREE_THR){
    make_step(dir_newest);
  }
  rgb_sensor.setInterrupt(false);
  set_goal(newest_bottle_idx, GOAL_RGB, COLOR_UNKNOWN);
}

void ending_seq(){
  buzzer_play_tone(TONE_END);
  printer_print_receipt();
  buzzer_play_tone(TONE_END);

  bottles_cnt[COLOR_CAN] = 0;
  bottles_cnt[COLOR_PLASTIC] = 0;
  bottles_cnt[COLOR_GLASS] = 0;
}

bool in_array(int array[], int array_len, int value){
  for (int i = 0; i < array_len; i++){
    if (array[i] == value){
      return true;
    }
  }
  return false;
}