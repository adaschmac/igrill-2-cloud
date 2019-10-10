/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * input.h - basic input interface
 *
 */

#ifndef IG2C_INPUT_H
#define IG2C_INPUT_H

#include "Particle.h"

#include "../igrill-config.h"

#include "../circular_buffer.h"
#include "../measurement.h"

class Input {
public:
    Input(uint8_t inputIndex, uint32_t inputPeriodMs)
        : m_index(inputIndex),
          m_period(inputPeriodMs),
          m_nextReadTime(0)
    {}

    virtual ~Input() {}

    size_t Read(CircularBuffer<Measurement>& buffer) {
        uint64_t ms = millis();
        if (ms > m_nextReadTime) {
            m_nextReadTime = ms + m_period;
            return ReadInternal(buffer, Time.now());
        }
        return 0;
    }

    virtual size_t GetDeviceName(char* buffer, size_t max_len) = 0;
    virtual size_t GetChannelName(uint8_t chanIndex, char* buffer, size_t max_len) = 0;
protected:
    uint8_t m_index;

private:
    virtual size_t ReadInternal(CircularBuffer<Measurement>& buffer, uint32_t currentTime) = 0;

    uint32_t m_period;
    uint64_t m_nextReadTime;
};

#endif // IG2C_INPUT_H