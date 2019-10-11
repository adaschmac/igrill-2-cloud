/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * igrill_input.h - connect, authenticate with, and get temperature readings from an iGrill
 *
 */

#ifndef IG2C_IGRILL_INPUT_H
#define IG2C_IGRILL_INPUT_H

#include "igrill-config.h"
#include "input/input.h"
#include "mbedtls/aes.h"

class iGrillInput : public Input {
public:
    iGrillInput(uint8_t inputIndex, uint32_t inputPeriodMs, BleAddress address, const String& name);
    ~iGrillInput();
    virtual InputClassType Type() { return InputClassType::iGrill; }

    BleAddress GetAddress() { return m_address; }
    virtual bool IsReady();
    virtual size_t GetDeviceName(char* buffer, size_t max_len);
    virtual size_t GetChannelName(uint8_t chanIndex, char* buffer, size_t max_len);
private:
    size_t ReadInternal(CircularBuffer<Measurement>& buffer, uint32_t currentTime);

    void Authenticate();

    String m_name;
    BleAddress m_address;

public:
    /* need to be public because of callbacks */
    bool m_isReady;
    mbedtls_aes_context m_aesCtx;
    BlePeerDevice m_device;
    unsigned char m_challengeBits[16];
};

#endif // IG2C_IGRILL_SCANNER_INPUT_H