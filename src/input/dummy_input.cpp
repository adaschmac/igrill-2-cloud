/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * dummy_input.cpp - input providing a sawtooth wave
 *
 */

#include "../igrill-config.h"
#include "dummy_input.h"

static const char* sDummyName = "dummy";
static const char* sDummyChan = "ch%d";

DummyInput::DummyInput(uint8_t inputIndex, uint32_t inputPeriodMs, uint8_t numChans)
    : Input(inputIndex, inputPeriodMs),
      m_numChans(numChans),
      m_values((uint8_t*)malloc(numChans))
{
    if (m_values == nullptr) {
        IG2C_DEBUG("Could not allocate memory for dummy input current values array");
        return;
    }

    for (uint8_t i = 0; i<numChans; ++i) {
        m_values[i] = random(255);
    }
}

DummyInput::~DummyInput() {
    free(m_values);
}

size_t DummyInput::GetDeviceName(char* buffer, size_t max_len) {
    return strlen(strncpy(buffer, sDummyName, max_len));
}

size_t DummyInput::GetChannelName(uint8_t chanIndex, char* buffer, size_t max_len) {
    snprintf(buffer, max_len, sDummyChan, chanIndex);
    return strlen(buffer);
}

size_t DummyInput::ReadInternal(CircularBuffer<Measurement>& buffer, uint32_t currentTime) {
    size_t chansWritten = 0;

    Measurement m;
    m.timestamp = currentTime;
    m.device_id = m_index;

    for (uint8_t i = 0; i<m_numChans; ++i) {
        m.channel_id = i;
        snprintf(m.data, 16, "%d", ++(m_values[i]));
        chansWritten += buffer.WriteElement(m);
    }

    return chansWritten;
}
