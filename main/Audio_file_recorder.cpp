#include "Audio_file_recorder.hpp"
#include "I2S_Mic.hpp"
#include "format_wav.h"

#include <filesystem>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>

Audio_file_recorder::Audio_file_recorder(I2S_Mic *mic) : m_mic(mic)
{
    m_byte_rate = (m_mic->sample_rate * (m_mic->bit_sample / 8)) * m_mic->num_channels;
    m_sample_size = (m_mic->bit_sample * 1024);
    m_buffer_size = rec_time * m_byte_rate + sizeof(wav_header_t);
}

void Audio_file_recorder::start()
{
    if (!m_recording)
    {
        m_recording = true;
        std::thread t(&Audio_file_recorder::record, this, m_buffer);
        t.detach();
    }
}

std::vector<uint8_t> Audio_file_recorder::stop()
{
    m_recording = false;
    return m_buffer;
}

void Audio_file_recorder::record()
{

    m_buffer.resize(m_buffer_size);

    const wav_header_t wav_header =
        WAV_HEADER_PCM_DEFAULT(m_buffer_size, m_mic->bit_sample, m_mic->sample_rate, m_mic->num_channels);
    memcpy(&m_buffer[0], &wav_header, sizeof(wav_header));

    uint8_t *buff1 = &m_buffer[0] + sizeof(wav_header);
    uint32_t data_size = 0;
    auto start = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::seconds(rec_time);

    while (m_recording && data_size < m_buffer_size && (std::chrono::high_resolution_clock::now() - start) < duration)
    {
        data_size += m_mic->read(buff1, m_sample_size);
        buff1 += m_sample_size;
    }
}