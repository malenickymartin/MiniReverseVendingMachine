#include <Arduino.h>
#include "globals.h"
#include "actuators.h"
#include "logic.h"

void setup() {
  printer.begin(9600);
  
  pinMode(IR_ANALOG_PIN, INPUT);
  
  while (rgb_sensor.begin() == false){
    delay(1000);
  }
  rgb_sensor.setInterrupt(true);
  
  servo_left.attach(SERVO_LEFT_PIN);
  servo_right.attach(SERVO_RIGHT_PIN);
  servos_to_pos(COLOR_UNKNOWN);
  
  pinMode(STEPPER_1_PIN, OUTPUT);
  pinMode(STEPPER_2_PIN, OUTPUT);
  pinMode(STEPPER_3_PIN, OUTPUT);
  pinMode(STEPPER_4_PIN, OUTPUT);
  stepper.setSpeed(STEPPER_RPM);
  stepper_off();
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  
  led_strip.begin();
  led_strip.setBrightness(LED_BRIGHTNESS);
}

void loop(){
  if (analogRead(IR_ANALOG_PIN) <= IR_PROXY_THR){
    bottle_detected();
    while (bottles_in_tunnel > 0){
      if (servo_current_pos_color != bottle_colors[oldest_bottle_idx]){
        servo_current_pos_color = bottle_colors[oldest_bottle_idx];
        servos_to_pos(servo_current_pos_color);
      }
      make_step(dir_newest);
      if ((analogRead(IR_ANALOG_PIN) <= IR_PROXY_THR) && !in_array(next_goal, MAX_BOTTLES_IN_TUNNEL, GOAL_RGB) && !in_array(next_goal, MAX_BOTTLES_IN_TUNNEL, GOAL_OUT)){
        bottle_detected();
      }
    }
    stepper_off();
    servos_to_pos(COLOR_UNKNOWN);
  }
  
  if ((digitalRead(BUTTON_PIN) == LOW) && (bottles_cnt[COLOR_CAN]+bottles_cnt[COLOR_PLASTIC]+bottles_cnt[COLOR_GLASS] > 0)){
    ending_seq();
  }
  delay(10);
}