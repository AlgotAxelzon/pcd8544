#include <cstdint>

#define DATA    HIGH
#define COMMAND LOW

class PCD8544 {

    public:
    PCD8544(uint8_t reset_pin, uint8_t chip_enable_pin, uint8_t data_command_pin, uint8_t serial_data_pin, uint8_t serial_clock_pin);

    void init();

    void clear_screen();

    void pixel(uint8_t x, uint8_t y, uint8_t set);

    private:
    void send_data(uint8_t data);
    void send_command(uint8_t data);

    void set_xy(uint8_t x, uint8_t y);
    void reset();

    uint8_t _reset_pin;
    uint8_t _serial_data_pin;
    uint8_t _serial_clock_pin;
    uint8_t _chip_enable_pin;
    uint8_t _data_command_pin;
};
