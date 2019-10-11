/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * igrill_scanner_input.cpp - scan nearby BLE devices for iGrills
 *
 */

#include "input/igrill_scanner_input.h"
#include "input/igrill_input.h"

static const char* sScannerName = "scanner";
static const char* sChanNames[] = { "mac", "name", "rssi", "status" };

static const char* sInputStatus[] = { "not_added", "added", "not_ready", "ready" };

static BleScanResult scanResults[IG2C_BLE_SCAN_RESULT_MAX];

iGrillScannerInput::iGrillScannerInput(uint8_t inputIndex, uint32_t inputPeriodMs, Input** inputArray)
    : Input(inputIndex, inputPeriodMs),
      m_inputs(inputArray)
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

            InputStatus status = CheckAndAddInput(scanResults[i]);
            m.channel_id = 3;
            strncpy(m.data, sInputStatus[status], 16);
            chansWritten += buffer.WriteElement(m);
        }
    }
    return chansWritten;
}

iGrillScannerInput::InputStatus iGrillScannerInput::CheckAndAddInput(const BleScanResult& result) {
    int ble_devices = 0;
    uint8_t first_null_idx = -1;

    for (int i=0; i<IG2C_MAX_INPUT_DEVICES; ++i) {
        if (m_inputs[i] != nullptr && m_inputs[i]->Type() == InputClassType::iGrill) {
            iGrillInput* castInput = (iGrillInput*)m_inputs[i];
            if (result.address == castInput->GetAddress()) {
                return castInput->IsReady() ? InputStatus::Ready : InputStatus::NotReady;
            }
            else
            {
                ble_devices++;
            }
        }
        else if (m_inputs[i] == nullptr && first_null_idx == -1)
        {
            first_null_idx = i;
        }
    }

    if (ble_devices >= IG2C_MAX_BLE_DEVICES) {
        IG2C_DEBUG_F("Not adding device because there are already %d present.", ble_devices);
        return InputStatus::NotAdded;
    }

    if (first_null_idx == -1) {
        IG2C_DEBUG("Not adding device because there are no empty device slots.");
        return InputStatus::NotAdded;
    }

    m_inputs[first_null_idx] = new iGrillInput(first_null_idx, 2500, result.address, result.advertisingData.deviceName());
    return InputStatus::Added;
}