#include <filesystem>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>

class I2S_Mic;

class Audio_file_recorder
{
protected:
    static constexpr uint32_t rec_time = 10;

    uint32_t m_byte_rate;
    size_t m_sample_size;
    size_t m_buffer_size;

    I2S_Mic *m_mic;
    std::vector<uint8_t> m_buffer;

    std::atomic<bool> m_recording;

    void record();

public:
    Audio_file_recorder(I2S_Mic *mic);
    void start();
    std::vector<uint8_t> stop();
};