#include "I2S_Mic.hpp"

/**
 * @brief Constructor
 *
 * @param gpio_config
 */
I2S_Mic::I2S_Mic(const i2s_std_gpio_config_t &gpio_config)
    : m_gpio_cfg(gpio_config)
{
    init();
}

/**
 * @brief Constructor
 *
 */
I2S_Mic::I2S_Mic()
{
}

/**
 * @brief Destructor
 *
 */
I2S_Mic::~I2S_Mic()
{
    ESP_ERROR_CHECK(i2s_channel_disable(m_rx_chan));
}

/**
 * @brief Sets up the I2S mic
 *
 * @param gpio_config
 */
void I2S_Mic::setup(const i2s_std_gpio_config_t &gpio_config)
{
    m_gpio_cfg = gpio_config;
    init();
}

/**
 * @brief Reads data from the I2S mic
 *
 * @param buffer - the pointer to receiving data buffer
 * @param buff_size - the size of the buffer
 *
 * @return size_t - the number of bytes actually read
 */
size_t I2S_Mic::read(uint8_t *buffer, size_t buff_size)
{
    i2s_channel_read(m_rx_chan, buffer, buff_size, &r_bytes, timeout_ms);
    return r_bytes;
}

/**
 * @brief Initializes the I2S mic
 *
 */
void I2S_Mic::init()
{
    m_config = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = m_gpio_cfg};
    m_config.slot_cfg.slot_mask = I2S_STD_SLOT_RIGHT;

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &m_rx_chan));

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(m_rx_chan, &m_config));

    ESP_ERROR_CHECK(i2s_channel_enable(m_rx_chan));
}