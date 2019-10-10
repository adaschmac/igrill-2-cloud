/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * dummy_input.h - input providing a sawtooth wave
 *
 */

#ifndef IG2C_DUMMY_INPUT_H
#define IG2C_DUMMY_INPUT_H

#include "Particle.h"
#include "string.h"

#include "input.h"

class DummyInput : public Input {
public:
    DummyInput(uint8_t inputIndex, uint32_t inputPeriodMs, uint8_t numChans);
    ~DummyInput();

    virtual size_t GetDeviceName(char* buffer, size_t max_len);
    virtual size_t GetChannelName(uint8_t chanIndex, char* buffer, size_t max_len);
private:
    size_t ReadInternal(CircularBuffer<Measurement>& buffer, uint32_t currentTime);

    uint8_t m_numChans;
    uint8_t* m_values;
};

#endif // IG2C_DUMMY_INPUT_H