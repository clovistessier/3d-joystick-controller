#ifndef MCP3208_H
#define MCP3208_H

#include <Arduino.h>
#include <SPI.h>

struct MCP3208_settings_t
{
    uint8_t ss_pin;
    uint32_t clk_spd;
};

class MCP3208
{
private:
    uint8_t _ss;
    uint32_t _clkSpd;
    SPIClass _spi;
    void spiCommand(uint8_t *data, uint8_t size);

public:
    MCP3208();
    void init(uint8_t ss = 5, uint32_t clkSpd = 500000);
    uint16_t readADC(uint8_t channel);
    ~MCP3208();
};

enum adc_channel_list : uint8_t
{
    CH_RZ = 0,
    CH_RY,
    CH_RX,
    CH_LZ,
    CH_LY,
    CH_LX,
};

#endif