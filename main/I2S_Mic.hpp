#include "driver/i2s_std.h"
#include "driver/gpio.h"

class I2S_Mic
{
protected:
    i2s_chan_handle_t m_rx_chan;
    i2s_std_gpio_config_t m_gpio_cfg;
    i2s_std_config_t m_config;
    size_t r_bytes;
    void init();

    static constexpr uint32_t timeout_ms = 1000;

public:
    I2S_Mic(const i2s_std_gpio_config_t &gpio_config);
    I2S_Mic();
    ~I2S_Mic();
    void setup(const i2s_std_gpio_config_t &gpio_config);
    size_t read(uint8_t *buffer, size_t buff_size);

    static constexpr uint32_t bit_sample = 16;
    static constexpr uint32_t sample_rate = 44100;
    static constexpr uint32_t num_channels = 1;
};