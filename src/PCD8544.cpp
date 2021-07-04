#include "PCD8544.h"
#include "Arduino.h"

PCD8544::PCD8544(uint8_t reset_pin, uint8_t chip_enable_pin, uint8_t data_command_pin, uint8_t serial_data_pin, uint8_t serial_clock_pin) 
    : _reset_pin{reset_pin}
    , _chip_enable_pin{chip_enable_pin}
    , _data_command_pin{data_command_pin}
    , _serial_data_pin{serial_data_pin}
    , _serial_clock_pin{serial_clock_pin}
    {
    }

void PCD8544::send_data(uint8_t data) {

    // Enable Chip Select
    digitalWrite(_chip_enable_pin, LOW);

    for (int bitpos = 7; bitpos >= 0; bitpos--) {
        
        // Clock LOW
        digitalWrite(_serial_clock_pin, LOW);
        // Clock sync
        delayMicroseconds(1);

        // Write data
        if (data & (1 << bitpos)) {
            digitalWrite(_serial_data_pin, HIGH);
        } else {
            digitalWrite(_serial_data_pin, LOW);
        }

        if (bitpos == 0) {
            digitalWrite(_data_command_pin, HIGH);
        }

        // Clock sync
        delayMicroseconds(1);
        // Clock High
        digitalWrite(_serial_clock_pin, HIGH);
        // Clock sync
        delayMicroseconds(1);

    }
    // Disable Chip Select
    digitalWrite(_chip_enable_pin, HIGH);
}

void PCD8544::send_command(uint8_t data) {
  
    // Enable Chip Select
    digitalWrite(_chip_enable_pin, LOW);

    for (int bitpos = 7; bitpos >= 0; bitpos--) {
        
        // Clock LOW
        digitalWrite(_serial_clock_pin, LOW);
        // Clock sync
        delayMicroseconds(1);

        // Write data
        if (data & (1 << bitpos)) {
            digitalWrite(_serial_data_pin, HIGH);
        } else {
            digitalWrite(_serial_data_pin, LOW);
        }

        if (bitpos == 0) {
            digitalWrite(_data_command_pin, LOW);
        }

        // Clock sync
        delayMicroseconds(1);
        // Clock High
        digitalWrite(_serial_clock_pin, HIGH);
        // Clock sync
        delayMicroseconds(1);

    }
    // Disable Chip Select
    digitalWrite(_chip_enable_pin, HIGH);
}

void PCD8544::init() {
    pinMode(_chip_enable_pin, OUTPUT);
    pinMode(_reset_pin, OUTPUT);
    pinMode(_data_command_pin, OUTPUT);
    pinMode(_serial_data_pin, OUTPUT);
    pinMode(_serial_clock_pin, OUTPUT);

    reset();

    // setConstrast
    send_command(0x21);
    send_command(0x80 | 75);
    send_command(0x20);

    // setBias
    send_command(0x21);
    send_command(0x10 | 0x07);
    send_command(0x20);

    // Something
    send_command(0x20);
    send_command(0x08 | 0x4);
}

void PCD8544::set_xy_page(uint8_t x, uint8_t y) {
  uint8_t x_page = x;
  uint8_t y_page = y;
  if (x>83) {
      uint8_t x_page = 83;
  }
  if (y>5) {
      uint8_t y_page = 5;
  }
  send_command(0x80 | x_page);
  send_command(0x40 | y_page);
}

void PCD8544::clear_screen() {
  for (int x = 0; x < 84; x++) {
    for (int y = 0; y < 6; y++) {
      set_xy_page(x, y);
      send_data(0x00);
    }  
  }
}

void PCD8544::pixel(uint8_t x, uint8_t y, uint8_t set) {
  uint8_t y_page = y >> 3;
  uint8_t y_index = y & 0b111;
  set_xy_page(x, y_page);
  send_data((set & 1) << y_index);
}

void PCD8544::reset() {
    digitalWrite(_reset_pin, LOW);
    delay(1); // 1 ns minimum
    digitalWrite(_reset_pin, HIGH);
}
