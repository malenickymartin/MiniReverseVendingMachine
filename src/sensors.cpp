#include "sensors.h"

int rgb_read(){
  float red, green, blue;
  rgb_sensor.getRGB(&red, &green, &blue);

  int res_can = abs(ETALON_CAN[0]-red) + abs(ETALON_CAN[1]-green) + abs(ETALON_CAN[2]-blue);
  int res_plastic = abs(ETALON_PLASTIC[0]-red) + abs(ETALON_PLASTIC[1]-green) + abs(ETALON_PLASTIC[2]-blue);
  int res_glass = abs(ETALON_GLASS[0]-red) + abs(ETALON_GLASS[1]-green) + abs(ETALON_GLASS[2]-blue);

  int color = COLOR_UNKNOWN;
  if ((res_can < res_glass) && (res_can < res_plastic) && (res_can < ETALON_THR)){
    color = COLOR_CAN;
  }
  else if ((res_plastic < res_can) && (res_plastic < res_glass) && (res_plastic < ETALON_THR)){
    color = COLOR_PLASTIC;
  }
  else if ((res_glass < res_can) && (res_glass < res_plastic) && (res_glass < ETALON_THR)){
    color = COLOR_GLASS;
  }

  return color;
}