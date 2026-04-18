#include "actuators.h"

void stepper_off(){
  digitalWrite(STEPPER_1_PIN, LOW);
  digitalWrite(STEPPER_2_PIN, LOW);
  digitalWrite(STEPPER_3_PIN, LOW);
  digitalWrite(STEPPER_4_PIN, LOW);
}

void stepper_step(int dir){
  static unsigned long last_step_time = 0;
  while ((micros() - last_step_time) < STEPPER_STEP_DELAY){
    delayMicroseconds(100);
  }
  stepper.step(dir);
  last_step_time = micros();
}

void buzzer_play_tone(const int tone_to_play[]){
  tone(BUZZER_PIN, tone_to_play[0], tone_to_play[1]);
}

void led_fill_color(const byte color[]){
  uint32_t c = led_strip.Color(color[0], color[1], color[2]);
  led_strip.fill(c);
  led_strip.show();
  delay(100*color[3]);
  led_off();
}

void led_off(){
  uint32_t c = led_strip.Color(0, 0, 0);
  led_strip.fill(c);
  led_strip.show();
}

void led_rot_step(){
  static unsigned long last_step_time;
  if ((micros() - last_step_time) < LED_STEP_DELAY){
    return;
  }
  leading_rot_led = (leading_rot_led+1)%LED_NUM;
  uint32_t c;
  c = led_strip.Color(0, 0, 0);
  led_strip.fill(c);
  for (int i = 0; i < LED_NUM_ROT; i++){
    c = led_strip.Color(LED_RGB_ROT[i][0], LED_RGB_ROT[i][1], LED_RGB_ROT[i][2]);
    led_strip.setPixelColor((leading_rot_led-i)%LED_NUM, c);
  }
  led_strip.show();
  last_step_time = micros();
}

void servos_to_pos(int bottle_type){
  if ((bottle_type != COLOR_UNKNOWN) && (servo_current_pos_color == COLOR_UNKNOWN)){
    servo_left.attach(SERVO_LEFT_PIN);
    servo_right.attach(SERVO_RIGHT_PIN);
  }
  switch (bottle_type){
    case COLOR_CAN:
      servo_left.write(SERVO_LEFT_CAN);
      servo_right.write(SERVO_RIGHT_CAN);
      break;
    case COLOR_PLASTIC:
      servo_left.write(SERVO_LEFT_PLASTIC);
      servo_right.write(SERVO_RIGHT_PLASTIC);
      break;
    case COLOR_GLASS:
      servo_left.write(SERVO_LEFT_GLASS);
      servo_right.write(SERVO_RIGHT_GLASS);
      break;
    case COLOR_UNKNOWN:
      servo_left.write(90);
      servo_right.write(90);
      servo_left.detach();
      servo_right.detach();
      break;
    default:
      return;
  }
}