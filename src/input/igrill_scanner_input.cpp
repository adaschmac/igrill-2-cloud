/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * igrill_scanner_input.cpp - scan nearby BLE devices for iGrills
 *
 */

#include "input/igrill_scanner_input.h"

static const char* sScannerName = "scanner";
static const char* sChanNames[] = { "mac", "name", "rssi" };

static BleScanResult scanResults[IG2C_BLE_SCAN_RESULT_MAX];

iGrillScannerInput::iGrillScannerInput(uint8_t inputIndex, uint32_t inputPeriodMs)
    : Input(inputIndex, inputPeriodMs)
{
    BLE.setScanTimeout(IG2C_BLE_SCAN_TIMEOUT_TENS_OF_MS);
}

iGrillScannerInput::~iGrillScannerInput() {}

size_t iGrillScannerInput::GetDeviceName(char* buffer, size_t max_len) {
    return strlen(strncpy(buffer, sScannerName, max_len));
}

size_t iGrillScannerInput::GetChannelName(uint8_t chanIndex, char* buffer, size_t max_len) {
    return strlen(strncpy(buffer, sChanNames[chanIndex], max_len));
}

size_t iGrillScannerInput::ReadInternal(CircularBuffer<Measurement>& buffer, uint32_t currentTime) {
    int count = BLE.scan(scanResults, IG2C_BLE_SCAN_RESULT_MAX);
    size_t chansWritten = 0;

    Measurement m;
    m.timestamp = currentTime;
    m.device_id = m_index;

    for (int i = 0; i<count; ++i) {
        String name = scanResults[i].advertisingData.deviceName();
        if (name.length() > 0 && strncmp(name.c_str(), IG2C_BLE_IGRILL_PREFIX, IG2C_BLE_IGRILL_PREFIX_LEN) == 0) {
            m.channel_id = 0;
            snprintf(m.data, 16, "%02X%02X%02X%02X%02X%02X",
                    scanResults[i].address[0], scanResults[i].address[1], scanResults[i].address[2],
                    scanResults[i].address[3], scanResults[i].address[4], scanResults[i].address[5]);
            chansWritten += buffer.WriteElement(m);

            m.channel_id = 1;
            strncpy(m.data, name.c_str(), 16);
            chansWritten += buffer.WriteElement(m);

            m.channel_id = 2;
            snprintf(m.data, 16, "%d", scanResults[i].rssi);
            chansWritten += buffer.WriteElement(m);
        }
    }
    return chansWritten;
}