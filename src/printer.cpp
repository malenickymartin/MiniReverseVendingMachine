#include "printer.h"

// Internal Helper Functions
static void printer_align_left() {
  printer.write(0x1B);
  printer.write(0x61);
  printer.write((byte) 0x00);
}

static void printer_align_center() {
  printer.write(0x1B);
  printer.write(0x61);
  printer.write(0x01);
}

static void printer_bold_on() {
  printer.write(0x1B);
  printer.write(0x45);
  printer.write(0x01);
}

static void printer_bold_off() {
  printer.write(0x1B);
  printer.write(0x45);
  printer.write((byte) 0x00);
}

static void printer_init() {
  printer.write(0x1B);
  printer.write(0x40);
  delay(100);
}

static void printer_wake() {
  printer.write(0x1B);
  printer.write(0x3D);
  printer.write(0x01);
}

static void printer_sleep() {
  printer.write(0x1B);
  printer.write(0x3D);
  printer.write((byte) 0x00);
}

static void printer_print_bitmap(int width, int height, const uint8_t bitmap[]){
  int width_bytes = (width + 7) / 8;
  printer.write(0x1D);
  printer.write(0x76);
  printer.write(0x30);
  printer.write((byte) 0x00);
  printer.write(width_bytes);
  printer.write((byte) 0x00);
  printer.write(height & 0xFF);
  printer.write((height >> 8) & 0xFF);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width_bytes; x++) {
      uint8_t b = pgm_read_byte(&bitmap[y * width_bytes + x]);
      printer.write(b);
    }
  }
}

// Main Public Function
void printer_print_receipt(){
  printer_wake();
  printer_init();

  printer_align_center();
  printer_print_bitmap(LOGO_W, LOGO_H, LOGO_MATTONI_BITMAP);
  printer.println("\n");
  printer_print_bitmap(QR_W, QR_H, QR_MATTONI_BITMAP);

  printer_align_left();
  printer.println("--------------------------------");

  printer_bold_on();
  printer.println("Beverage packages returned:");
  printer_bold_off();
  printer.print(bottles_cnt[COLOR_CAN]); printer.println("x Can");
  printer.print(bottles_cnt[COLOR_PLASTIC]); printer.println("x PET");
  printer.print(bottles_cnt[COLOR_GLASS]); printer.println("x Glass");

  printer.println("--------------------------------");

  printer_bold_on();
  printer.print("Total: ");
  printer.print(bottles_cnt[COLOR_CAN]+bottles_cnt[COLOR_PLASTIC]+bottles_cnt[COLOR_GLASS]);
  printer.println("x");
  printer_bold_off();
  printer.println("--------------------------------");

  printer.println("\nThank you for supporting");
  printer_bold_on();
  printer.println("\nDeposit-Refund System");
  printer_bold_off();
  printer.println("\n\n\n");
  printer_sleep();
}