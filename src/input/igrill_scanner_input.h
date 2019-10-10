/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * igrill_scanner_input.h - scan nearby BLE devices for iGrills
 *
 */

#ifndef IG2C_IGRILL_SCANNER_INPUT_H
#define IG2C_IGRILL_SCANNER_INPUT_H

#include "igrill-config.h"
#include "input/input.h"

class iGrillScannerInput : public Input {
public:
    iGrillScannerInput(uint8_t inputIndex, uint32_t inputPeriodMs);
    ~iGrillScannerInput();

    virtual size_t GetDeviceName(char* buffer, size_t max_len);
    virtual size_t GetChannelName(uint8_t chanIndex, char* buffer, size_t max_len);
private:
    size_t ReadInternal(CircularBuffer<Measurement>& buffer, uint32_t currentTime);
};

#endif // IG2C_IGRILL_SCANNER_INPUT_H